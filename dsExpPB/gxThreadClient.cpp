#include "gxThreadClient.h"
#include "gxFunc.h"
#include "gxPBInterface.h"
#include "DSMsg.pb.h"

#pragma warning(disable:4996)
#pragma warning(disable:4251)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CgxThreadClient::CgxThreadClient()
{
	m_IP = "127.0.0.1";
	m_Port = 6666 ;
}

CgxThreadClient::~CgxThreadClient()
{

}
void CgxThreadClient::Run()
{
	Connect() ; 
	for( ;m_RunFlag;)
	{
		Recv() ; 
		Send() ;
		Sleep(10) ;
	}
}
int CgxThreadClient::OnInit()
{
	m_Serial = 0 ; 
	AfxSocketInit() ; 
return 0 ;
}
int CgxThreadClient::Connect()
{
    memset(&m_ServerSockAddr, 0, sizeof(m_ServerSockAddr)); 
    m_ServerSockAddr.sin_port			= htons(m_Port);	
    m_ServerSockAddr.sin_family			= AF_INET;		
    m_ServerSockAddr.sin_addr.s_addr	= 0 ; 

    if((m_Sock=socket(AF_INET,SOCK_STREAM, 0)) == INVALID_SOCKET)
        return -1;
    m_ServerSockAddr.sin_addr.s_addr =	inet_addr(m_IP) ;		
	if( connect(m_Sock,(sockaddr*)&m_ServerSockAddr,sizeof(m_ServerSockAddr)) !=0)
		return -3 ;
	m_Client.SetSock(m_Sock) ; 
return 0 ; 
}
int CgxThreadClient::Send() 
{
	CgxSockMsg * Msg = (CgxSockMsg*)m_SendList.GetItem() ;
	if( Msg == NULL ) 
		return 0 ; 
	int Ret = Msg->Send() ; 
	if(( Ret ==0) ||(Ret<-3))
	{
		Msg->ReleaseRef() ; 
	}else
	{
		m_SendList.AddItem(Msg) ; 
	}
return Ret ;
}
int CgxThreadClient::OnAckInsertMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead)
{
	if(( _pMsg == NULL )  || (_pHead == NULL))
		return -1 ;
	InterfaceInsertAck Ack;
	Ack.ParseFromArray(_pMsg,_nLen) ;
	gxWriteLog("Insert AckMsg MsgID=%d ,Len = %d,Serier =%d , Ret = %d" , 
		_pHead->MsgID,
		_pHead->MsgLen,
		_pHead->MsgSerial,
		Ack.ret()) ;
return 0 ; 
}
int CgxThreadClient::OnAckSelectMsg(char *_pMsg , int _nLen,EXP_HEAD*_pHead)
{
	if(( _pMsg == NULL )  || (_pHead == NULL))
		return -1 ;
	InterfaceSelectAck Ack;
	Ack.ParseFromArray(_pMsg,_nLen) ;
	gxWriteLog("Select AckMsg MsgID=%d , MsgLen=%d,Serial=%d,RetCount=%d" , 
		_pHead->MsgID,
		_pHead->MsgLen,
		_pHead->MsgSerial,
		Ack.cfgitems_size()  ) ;
	int Count = Ack.cfgitems_size() ;
	for(int i = 0;i < Count;i ++ ) 
	{
		SysInterfaceStruct* pItem = Ack.mutable_cfgitems(i) ;
		if( pItem == NULL ) 
			continue; 
			gxWriteLog("	ID=%d ,DSSvrID=%d,%s,%s,Enable=%d",
				pItem->interfaceid(),
				pItem->dssvrid(),
				pItem->ifname().c_str(),
				pItem->ifpath().c_str(),
				pItem->isenable()) ;
	}
	gxWriteLog("Select End!") ;
return 0 ; 
}
int CgxThreadClient::OnAckUpdateMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead)
{
	if(( _pMsg == NULL )  || (_pHead == NULL))
		return -1 ;
	InterfaceUpdateAck Ack;
	Ack.ParseFromArray(_pMsg,_nLen) ;
	gxWriteLog("Update AckMsg MsgID=%d ,Len = %d,Serier =%d , Ret = %d" , 
		_pHead->MsgID,
		_pHead->MsgLen,
		_pHead->MsgSerial,
		Ack.ret()) ;
return 0 ; 
}
int CgxThreadClient::OnAckDeleteMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead)
{
	if(( _pMsg == NULL )  || (_pHead == NULL))
		return -1 ;
	InterfaceDeleteAck Ack;
	Ack.ParseFromArray(_pMsg,_nLen) ;
	gxWriteLog("Delete AckMsg MsgID=%d ,Len = %d,Serier =%d , Ret = %d" , 
		_pHead->MsgID,
		_pHead->MsgLen,
		_pHead->MsgSerial,
		Ack.ret()) ;
return 0 ; 
}
int CgxThreadClient::Recv() 
{
	struct timeval tv ;
	tv.tv_sec = 0 ; 
	tv.tv_usec = 0 ;
	FD_SET fdRead;
	memset(&fdRead,0,sizeof(fdRead)) ;
	FD_SET(m_Sock , &fdRead) ; 
	int RetRead =select(-1,&fdRead,NULL,0,&tv) ;
	int ReadCount = fdRead.fd_count ; 
	if( ReadCount)
	{
		char szBuf[8096];
		memset(szBuf , 0 , sizeof(szBuf)) ;
		int Len = sizeof(szBuf) ;
		if( m_Client.RecvMsg(szBuf , Len)  >=0)
		{
			if( Len >= sizeof(EXP_HEAD) )
			{
				EXP_HEAD * pHead = (EXP_HEAD*) szBuf;
				int MsgID = ntohl(pHead->MsgID);
				int MsgLen = ntohl(pHead->MsgLen) ;
				char * p = szBuf ; 
				p+= sizeof(EXP_HEAD);
				pHead->ntoh() ;
				switch( MsgID) 
				{
				case EXP_MSG_PB_ACK_INSERT :
					OnAckInsertMsg(p , MsgLen,pHead) ;
					break;
				case EXP_MSG_PB_ACK_SELECT :
					OnAckSelectMsg(p , MsgLen,pHead) ;
					break;
				case EXP_MSG_PB_ACK_UPDATE :
					OnAckUpdateMsg(p , MsgLen,pHead) ;
					break;
				case EXP_MSG_PB_ACK_DELETE :
					OnAckDeleteMsg(p , MsgLen,pHead) ;
					break;
				default:
					gxWriteLog("接收到未知的消息ID！%d,%d" , MsgID,MsgLen) ;
					break;
				}
			}
		}
	}
return 0 ; 
}
int CgxThreadClient::SendMsg(char *_pBuf,int _nLen)
{
	if( _pBuf == NULL ) 
		return -1 ;
	CgxSockMsg * pMsg = new CgxSockMsg() ; 
	pMsg->Copy(_pBuf ,_nLen) ;
	pMsg->m_Client = &m_Client ; 
	m_SendList.AddItem(pMsg) ;
	pMsg->ReleaseRef() ;
return 0 ;
}

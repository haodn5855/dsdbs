#include "stdafx.h"
#include "gxThreadClient.h"

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
    memset(&m_ServerSockAddr, 0, sizeof(m_ServerSockAddr)); // Clear all
    m_ServerSockAddr.sin_port			= htons(m_Port);	// Port
    m_ServerSockAddr.sin_family			= AF_INET;		// Internet family
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
int CgxThreadClient::OnAckInsertMsg(char *_pMsg , int _nLen)
{
	if( _pMsg == NULL )
		return -1 ;
	EXP_INSERT_ACK Ack;
	if ( _nLen == sizeof(Ack) )
	{
		memcpy(&Ack , _pMsg , sizeof(Ack)) ;
		Ack.Head.MsgID		= ntohl(Ack.Head.MsgID		) ;
		Ack.Head.MsgLen		= ntohl(Ack.Head.MsgLen		) ;
		Ack.Head.MsgSerial	= ntohl(Ack.Head.MsgSerial) ;
		Ack.nRet = ntohl(Ack.nRet) ;
		gxWriteLog("Insert AckMsg MsgID=%d ,Len = %d,Serier =%d , Ret = %d" , 
			Ack.Head.MsgID,
			Ack.Head.MsgLen,
			Ack.Head.MsgSerial,
			Ack.nRet) ;
	}
return 0 ; 
}
int CgxThreadClient::OnAckSelectMsg(char *_pMsg , int _nLen)
{
	if( _pMsg == NULL )
		return -1 ;
	EXP_SELECT_ACK Ack;
	if ( _nLen >= sizeof(Ack) )
	{
		memcpy(&Ack , _pMsg , sizeof(Ack) ) ;
		Ack.Head.MsgID		= ntohl(Ack.Head.MsgID		) ;
		Ack.Head.MsgLen		= ntohl(Ack.Head.MsgLen		) ;
		Ack.Head.MsgSerial	= ntohl(Ack.Head.MsgSerial) ;
		Ack.nRetCount		= ntohl(Ack.nRetCount) ;
		gxWriteLog("Select AckMsg MsgID=%d , MsgLen=%d,Serial=%d,RetCount=%d" , 
			Ack.Head.MsgID,
			Ack.Head.MsgLen,
			Ack.Head.MsgSerial,
			Ack.nRetCount) ;
		char * p = _pMsg ;
		p+=sizeof(Ack) ;
		EXP_DATA * pExpData = (EXP_DATA*)p ;
		for(int i = 0;i < Ack.nRetCount;i ++ ) 
		{
			if( pExpData == NULL ) 
				continue;
			pExpData->IntType	= ntohl(pExpData->IntType) ;
			pExpData->ShortType = ntohs(pExpData->ShortType) ;
			pExpData->SuffixType.dwOffSet	= ntohl(pExpData->SuffixType.dwOffSet) ;
			pExpData->SuffixType.dwSize		= ntohl(pExpData->SuffixType.dwSize) ;
			p = _pMsg + pExpData->SuffixType.dwOffSet + sizeof(EXP_HEAD);

			gxWriteLog("	c(%02X),s(%d),i(%d),64(%I64d),f(%f),d(%f),Str(%s),Suf(%s)",
				pExpData->CharType,
				pExpData->ShortType,
				pExpData->IntType,
				pExpData->Int64Type,
				pExpData->FloatType,
				pExpData->DoubleType,
				pExpData->StringType,
				p) ;
			pExpData++ ;
		}
		gxWriteLog("Select End!") ;
	}
return 0 ; 
}
int CgxThreadClient::OnAckUpdateMsg(char *_pMsg , int _nLen)
{
	if( _pMsg == NULL )
		return -1 ;
	EXP_UPDATE_ACK Ack;
	if ( _nLen >= sizeof(Ack) )
	{
		memcpy(&Ack , _pMsg , sizeof(Ack)) ;
		Ack.Head.MsgID		= ntohl(Ack.Head.MsgID		) ;
		Ack.Head.MsgLen		= ntohl(Ack.Head.MsgLen		) ;
		Ack.Head.MsgSerial	= ntohl(Ack.Head.MsgSerial) ;
		Ack.nRetCount		= ntohl(Ack.nRetCount) ;
		gxWriteLog("Update AckMsg MsgID=%d ,Len = %d,Serier =%d , RetCount = %d" , 
			Ack.Head.MsgID,
			Ack.Head.MsgLen,
			Ack.Head.MsgSerial,
			Ack.nRetCount) ;
	}
return 0 ; 
}
int CgxThreadClient::OnAckDeleteMsg(char *_pMsg , int _nLen)
{
	if( _pMsg == NULL )
		return -1 ;
	EXP_DELETE_ACK Ack;
	if ( _nLen >= sizeof(Ack) )
	{
		memcpy(&Ack , _pMsg , sizeof(Ack)) ;
		Ack.Head.MsgID		= ntohl(Ack.Head.MsgID		) ;
		Ack.Head.MsgLen		= ntohl(Ack.Head.MsgLen		) ;
		Ack.Head.MsgSerial	= ntohl(Ack.Head.MsgSerial) ;
		Ack.nRetCount		= ntohl(Ack.nRetCount) ;
		gxWriteLog("Delete AckMsg MsgID=%d ,Len = %d,Serier =%d , RetCount = %d" , 
			Ack.Head.MsgID,
			Ack.Head.MsgLen,
			Ack.Head.MsgSerial,
			Ack.nRetCount) ;
	}
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
				switch( MsgID) 
				{
				case EXP_MSG_ACK_INSERT :
					OnAckInsertMsg(szBuf , Len) ;
					break;
				case EXP_MSG_ACK_SELECT :
					OnAckSelectMsg(szBuf , Len) ;
					break;
				case EXP_MSG_ACK_UPDATE :
					OnAckUpdateMsg(szBuf , Len) ;
					break;
				case EXP_MSG_ACK_DELETE :
					OnAckDeleteMsg(szBuf , Len) ;
					break;
				default:
					gxWriteLog("接收到未知的消息ID！" , MsgID) ;
					break;
				}
			}
		}
	}
return 0 ; 
}

int CgxThreadClient::SendMsg(char *_pBuf , int _nLen)
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

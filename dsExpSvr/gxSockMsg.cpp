#include "StdAfx.h"
#include "gxSockMsg.h"
#include "gxMap.h"
#include "gxDsApp.h"

////////////////////////////////////////////////////////////////////////////////
CgxSockMsg::CgxSockMsg()
{
	m_Data = NULL ; 
	m_Len = 0 ; 
	m_Client = NULL ; 
	m_SendFlag = 0 ; 
	m_dwMsgID = 0 ;
}

CgxSockMsg::~CgxSockMsg()
{
	ReleaseAll(); 
}
int CgxSockMsg::Copy(char * _Buf , int _Len) 
{
	if( _Buf == NULL ) 
		return -1 ; 
	ReleaseAll() ; 
	m_Data = new char[_Len+1] ; 
	memset(m_Data , 0 , sizeof(char) * (_Len+1) ) ; 
	memcpy(m_Data , _Buf , _Len) ; 
	m_Len = _Len ; 
return 0 ; 
}

char * CgxSockMsg::GetData() 
{
	return m_Data ; 
}
void CgxSockMsg::ReleaseAll() 
{
	if(m_Data!=NULL ) 
		delete []m_Data ; 
	m_Data = NULL ; 
	m_Len = 0; 
}
int CgxSockMsg::Send(char *_pBuf , int _nLen) 
{
	if( m_Client == NULL ) 
		return -99 ;
	int Ret = send(m_Client->GetSock(),_pBuf,_nLen, 0) ;
	
	if( Ret !=_nLen) 
	{
		if( WSAENOTSOCK!= WSAGetLastError())
			TRACE("\r\n Send %d" ,WSAGetLastError() );
		m_SendFlag ++ ;
		return m_SendFlag*(-1) ;
	}
return 0 ; 
}

////////////////////////////////////////////////////////////////////////////////
CgxClient::CgxClient()
{
	Init() ;	
}
CgxClient::~CgxClient() 
{
	ReleaseAll() ; 
}
int CgxClient::Init()
{
	memset(m_User, 0 ,sizeof(m_User)) ; 
	m_Port = 0 ; 
	m_Sock = INVALID_SOCKET;  
	m_RecvLen = 0 ; 
	m_DataLen = 4*1024 ; 
	m_Data = new char[m_DataLen];
	memset(m_Data , 0 , sizeof(char) * m_DataLen) ; 
	m_IsOnLine = FALSE ;
	m_TickCount = GetTickCount();
return 0 ;
}

void CgxClient::ReleaseAll()
{
	if( m_Data!=NULL ) 
		delete []m_Data ; 
	m_Data = NULL ; 
	m_RecvLen = 0 ; 
}
SOCKET CgxClient::GetSock()  
{
return m_Sock ; 
}
int CgxClient::SetSock(SOCKET _Sock)
{
	if( _Sock == INVALID_SOCKET)
		m_IsOnLine = FALSE ;
	if( m_Sock == _Sock) 
		return 1 ;
	m_Sock = _Sock ; 
	m_RecvLen = 0 ; 
	memset(m_Data , 0 , sizeof(char) * m_DataLen) ; 
return 0; 
}

extern CgxDSApp g_DSApp;

int CgxClient::OnRecv(CgxListObj* _List)
{
	static int HeadLen = g_DSApp.GetHeadLen(APP_PROTO_TYPE,APP_MSGHEAD_ID) ;
	if( HeadLen <2 ) 
		return -3 ;

	if(_List== NULL )
		return -999 ;
	int Len = ::recv(m_Sock,m_Data+m_RecvLen ,m_DataLen -m_RecvLen,0) ;
	if( Len < 0 ) 
	{
		int Error = WSAGetLastError() ; 
		return -1 ;
	}
	if( Len >0)
	{
		m_RecvLen +=Len ;
	}
	char szHeadBuf[128];
	for( ;m_RecvLen>=HeadLen;)
	{
		memset(szHeadBuf , 0 , sizeof(szHeadBuf)) ;
		memcpy(szHeadBuf , m_Data,HeadLen) ;

		EXP_HEAD *pHead = (EXP_HEAD*) szHeadBuf; 

		EXP_HEAD tohHead;
		memset(&tohHead , 0 , sizeof(tohHead) );
		tohHead.MsgID = ntohl(pHead->MsgID) ;
		tohHead.MsgLen = ntohl(pHead->MsgLen) ;
		int MsgLen = g_DSApp.GetMsgLen(szHeadBuf ,HeadLen,APP_PROTO_TYPE,APP_MSGHEAD_ID) ;
		if ( MsgLen <0)
		{
			//取消息长度失败，向后移一个包头位置
			memmove(m_Data,m_Data+HeadLen ,m_RecvLen - HeadLen) ;
			m_RecvLen -=HeadLen;
			return 0 ;
		}
		if( MsgLen >m_RecvLen )
			return -2 ;
		DWORD MsgID = g_DSApp.GetMsgType(szHeadBuf ,HeadLen,APP_PROTO_TYPE,APP_MSGHEAD_ID) ;
		CgxSockMsg * Msg = new CgxSockMsg() ; 
		Msg->m_Client = this ;
		Msg->Copy(m_Data,MsgLen) ;
		_List->AddItem(Msg) ; 
		SetTickCount();
		Msg->m_dwMsgID = MsgID;
		Msg->ReleaseRef() ;

		memmove(m_Data,m_Data+MsgLen ,m_RecvLen - MsgLen) ;
		m_RecvLen -=MsgLen;
	}
return 0; 
}
int CgxClient::CloseSock() 
{
	closesocket(m_Sock) ;
	m_IsOnLine = false ;
	m_Sock = INVALID_SOCKET ;
return 0 ; 
}
DWORD CgxClient::SetTickCount()
{
	m_TickCount = GetTickCount();
return m_TickCount;
}

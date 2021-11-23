#include "StdAfx.h"
#include "gxSockMsg.h"
#include "gxMap.h"

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
int CgxSockMsg::Send()
{
	if( m_Client == NULL ) 
		return -99 ;
	int Ret = send(m_Client->GetSock(),m_Data,m_Len, 0) ;
	if( Ret !=m_Len) 
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
	m_Port = 0 ; 
	m_Sock = INVALID_SOCKET;  
	m_RecvLen = 0 ; 
	m_DataLen = 4*1024 ; 
	m_Data = new char[m_DataLen];
	memset(m_Data , 0 , sizeof(char) * m_DataLen) ; 
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
		return -1 ;
	if( m_Sock == _Sock) 
		return 1 ;
	m_Sock = _Sock ; 
	m_RecvLen = 0 ; 
	memset(m_Data , 0 , sizeof(char) * m_DataLen) ; 
return 0; 
}
int CgxClient::RecvMsg(char *_pBuf , int &_Len)
{
	if( _pBuf == NULL ) 
		return  -1 ;
	_Len = 0 ; 
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
	EXP_HEAD Head ;
	int HeadLen = 0;
	for( ;m_RecvLen>=16;)
	{
		memset(&Head , 0,sizeof(Head)) ; 
		memcpy(&Head ,m_Data,sizeof(Head)) ;
		HeadLen = ntohl(Head.MsgLen);
		if( HeadLen >m_RecvLen )
			return -2 ;

		memcpy(_pBuf , m_Data , Len) ;
		_Len = Len ;

		memmove(m_Data,m_Data+HeadLen ,m_RecvLen - HeadLen) ;
		m_RecvLen -=HeadLen;
	}
return 0; 
}
int CgxClient::CloseSock() 
{
	closesocket(m_Sock) ;
	m_Sock = INVALID_SOCKET ;
return 0 ; 
}
DWORD CgxClient::SetTickCount()
{
	m_TickCount = GetTickCount();
return m_TickCount;
}
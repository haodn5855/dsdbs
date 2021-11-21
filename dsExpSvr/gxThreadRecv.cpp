#include "StdAfx.h"
#include "gxThreadRecv.h"

CgxThreadRecv::CgxThreadRecv(void)
{
	Init() ; 
}

CgxThreadRecv::~CgxThreadRecv(void)
{
	ReleaseAll() ; 
}
int CgxThreadRecv::Lock()
{
	m_Lock.Lock() ; 
return 0 ;
}
int CgxThreadRecv::Unlock()
{
	m_Lock.Unlock() ;
return 0 ;
}
int CgxThreadRecv::Init()  
{
	m_MaxCount = 3*1024 ; 
	m_CurCount = 0 ;
	m_ClientList = NULL ; 
	m_Recv = NULL ; 
	m_DBRecv = NULL ; 
	m_Send = NULL ;
	m_ClientList= new CgxClient*[m_MaxCount] ; 
	memset(m_ClientList, 0 , sizeof(CgxClient*) * m_MaxCount) ;
return 0 ; 
}
int CgxThreadRecv::ReleaseAll() 
{
	Lock() ;
	CgxClient * Client = NULL ; 
	for( int i =0 ;i < m_CurCount ;i ++ ) 
	{
		Client = m_ClientList[i] ; 
		if( Client!=NULL)
			Client->ReleaseRef() ;
	}
	if( m_ClientList!=NULL)
		delete []m_ClientList ; 
	m_ClientList =NULL ;
	m_CurCount = 0 ; 
	Unlock() ;
return 0 ; 
}
int CgxThreadRecv::AddItem(CgxClient * _Client) 
{
	if( _Client == NULL ) 
		return -1 ;
	if( m_CurCount >m_MaxCount) 
		return -2 ; 
	_Client->AddRef() ;
	Lock() ; 
	m_ClientList[m_CurCount] = _Client ; 
	m_CurCount ++ ; 
	Unlock() ; 
	_Client->m_IsOnLine =1 ;
return 0 ; 
}
int CgxThreadRecv::DelItem(CgxClient * _Client) 
{
	if( _Client == NULL ) 
		return -1 ; 
	Lock() ; 
	CgxClient * Client = NULL ;
	for( int i = 0 ;i < m_CurCount ;i ++ ) 
	{
		Client = m_ClientList[i] ;
		if( Client ==NULL ) 
			continue ; 
		if( Client == _Client) 
		{
			DelItem(i) ; 
			Unlock() ;
			return 0 ; 
		}
	}
	Unlock() ;
return 0 ; 
}
int CgxThreadRecv::DelItem(int _Idx) 
{
	if(( _Idx <0) || (_Idx >= m_MaxCount) )
		return -1 ;
	CgxClient * Client = NULL ; 
	Lock() ; 
	Client = m_ClientList[_Idx];
	if( Client !=NULL ) 
	{
		Client->CloseSock() ;
		Client->ReleaseRef() ; 
	}
	m_CurCount -- ; 
	m_ClientList[_Idx] = m_ClientList[m_CurCount] ;
	if( _Idx == 0) 
		m_ClientList[0] = NULL ; 
	Unlock() ;
return 0 ; 
}
SOCKET CgxThreadRecv::GetItemSock(int _Idx) 
{
	if(( _Idx < 0 ) || (_Idx >=m_CurCount))	
		return -1 ;
	CgxClient * Client = m_ClientList[_Idx]	 ; 
	if(Client !=NULL)
		return Client->GetSock() ;
return -1 ;
}
int CgxThreadRecv::SetList(CgxListObj*_DBRecv,CgxListObj*_Recv,CgxListObj*_Send)
{
	m_DBRecv = _DBRecv ; 
	m_Recv = _Recv ; 
	m_Send = _Send ; 
return 0 ; 
}
int CgxThreadRecv::GetFdset(fd_set &_fdset)
{
	FD_ZERO(&_fdset);
	int fdCount = m_CurIdx+FD_SETSIZE;
	int Count = 0 ;
	Lock() ;
	m_MapSock.ReleaseAll() ;
	CgxClient * Client = NULL ;
	for( int i = m_CurIdx ;(i< fdCount)  &&( i < m_CurCount );i ++ ) 
	{
		Client = m_ClientList[i] ; 
		if( Client==NULL)
			continue ;
		if( Client->GetSock() == INVALID_SOCKET)
		{//
			DelItem(i) ;
			continue ;
		}
		FD_SET(Client->GetSock() ,&_fdset) ;
		m_MapSock.AddItem(Client->GetSock(),Client) ;
		Count ++ ;
	}
	Unlock();
return Count ;
}
int CgxThreadRecv::FdRecv(SOCKET _Sock)
{
	CgxClient * Client = (CgxClient*)m_MapSock.GetItem(_Sock) ; 
	if(Client == NULL ) 
		return -1 ;
	int Ret =Client->OnRecv(m_Recv) ;
	if( Ret == -1 ) 
	{
		OnError(Client,Ret) ;
	}
return 0 ;
}
int CgxThreadRecv::CloseSock(CgxClient * _Client)
{
	if( _Client == NULL)
		return -1; 
	_Client->CloseSock() ;
return 0 ;
}
int CgxThreadRecv::OnError(CgxClient * _Client , int _Error)
{
	if( _Client == NULL)
		return 0 ; 
	_Client->AddRef() ; 
	_Client->ReleaseRef();
return 0 ; 
}
void CgxThreadRecv::Run()
{
	struct timeval tv ;
	tv.tv_sec = 0 ; 
	tv.tv_usec = 0 ;

	for( ;m_RunFlag;) 
	{
		DWORD	Tick = GetTickCount() ; 
		m_CurIdx = 0 ; 
		for( ;;)
		{
			FD_SET fdRead;
			int Count = GetFdset(fdRead) ; 
			if( !Count )
				break; 
			m_CurIdx+=Count ;
			int RetRead =select(-1,&fdRead,NULL,0,&tv) ;
			int ReadCount = fdRead.fd_count;		
			for( int i = 0 ;i < ReadCount ;i ++ ) 
				FdRecv(fdRead.fd_array[i])	;
		}
		DWORD m_Time = GetTickCount() - Tick ;
		if( m_Time >100)
			Sleep(0) ;
		else
			Sleep(100-m_Time) ;
		Sleep(10) ;
	}
}


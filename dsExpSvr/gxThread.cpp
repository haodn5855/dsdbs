#include "StdAfx.h"
#include "gxThread.h"

////////////////////////////////////////////////////////////////////////////////
CgxThread::CgxThread() 
{
	m_RunFlag = 0 ; 
	m_Handle = NULL ; 
}
CgxThread::~CgxThread() 
{
	m_RunFlag = 0 ;
	WaitThread() ; 
}
void CgxThread::Run() 
{

}

DWORD WINAPI gxClassThread(LPVOID _p)
{
	CgxThread * Thread = (CgxThread*)_p ; 
	if ( Thread == NULL ) 
		return -1 ;
	Thread->Run() ;
	return 0; 
}
int CgxThread::StartThread()
{
	DWORD d ;
	OnInit() ;
	m_RunFlag = 1 ;
	TRACE("\r\n------------Ïß³ÌÆô¶¯------------") ;
	m_Handle =CreateThread(NULL,0,gxClassThread,this,0,&d);
	if( m_Handle == NULL) 
	{
		DWORD  Error = GetLastError() ;
	}
return 0 ;
}
int CgxThread::WaitThread() 
{
	if( m_Handle !=NULL ) 
		WaitForSingleObject(m_Handle,1000) ;
return 0 ;
}
int CgxThread::StopThread(int _WaitFlag)
{
	m_RunFlag = 0 ;
	if( _WaitFlag )
		WaitThread() ; 
return 0 ; 
}
int CgxThread::OnInit()
{
return 0 ; 
}





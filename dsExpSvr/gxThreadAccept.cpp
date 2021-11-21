#include "StdAfx.h"
#include "gxThreadRecv.h"
#include "gxThreadAccept.h"

CgxThreadAccept::CgxThreadAccept(void)
{
	m_ThreadRecv = NULL ; 
	m_Sock = INVALID_SOCKET ;
}

CgxThreadAccept::~CgxThreadAccept(void)
{

}
void CgxThreadAccept::Run() 
{
	ConnectToPort() ; 
	for( ;m_RunFlag;)
	{
		OnAccept() ; 
		Sleep(10) ; 
	}
}

int	CgxThreadAccept::ConnectToPort(int queSize,short family,unsigned long address )
{
    memset(&m_serverSockAddr, 0, sizeof(m_serverSockAddr)); // Clear all
    m_serverSockAddr.sin_port			= htons(m_Port);	// Port
    m_serverSockAddr.sin_family			= family;		// Internet family
    m_serverSockAddr.sin_addr.s_addr	= address;		// Address (any)

    if((m_Sock=socket(AF_INET,SOCK_STREAM, 0)) == INVALID_SOCKET)
        return -1;
    if(bind(m_Sock,(LPSOCKADDR)&m_serverSockAddr,sizeof(m_serverSockAddr))== SOCKET_ERROR)
        return -2;			// 

    if( listen(m_Sock, queSize ) == SOCKET_ERROR)
        return -3 ;
return 0 ; 
}
int CgxThreadAccept::SetThreadRecv(CgxThreadRecv * _Recv)
{
	m_ThreadRecv = _Recv ; 
return 0 ; 
}
int CgxThreadAccept::OnAccept()
{
	SOCKADDR_IN     clientSockAddr;
	int             addrLen = sizeof(SOCKADDR_IN);
	SOCKET  s = accept(m_Sock,(sockaddr*)&clientSockAddr, &addrLen);
	if( s ==INVALID_SOCKET)
	{
		closesocket(s) ;
		return -1 ;
	}

	CgxClient * Client = new CgxClient() ; 
	Client->SetSock(s) ; 
	Client->m_Port = clientSockAddr.sin_port ; 
	if( m_ThreadRecv!=NULL)
		m_ThreadRecv->AddItem(Client) ; 
	Client->ReleaseRef() ;
return 0 ; 
}

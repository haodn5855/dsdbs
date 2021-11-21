#pragma once
#include "gxobj.h"
class CgxThreadRecv ; 

class CgxThreadAccept :	public CgxThread
{
public:
	CgxThreadAccept(void);
	virtual ~CgxThreadAccept(void);
	void Run() ; 
	int m_Port ; 
	int SetThreadRecv(CgxThreadRecv * _Recv);
private:
	CgxThreadRecv * m_ThreadRecv ; 
	int	ConnectToPort(int queSize = 5, short family = AF_INET, unsigned long address = htonl(INADDR_ANY));
	int OnAccept() ; 
	SOCKADDR_IN	m_serverSockAddr;	
	SOCKET m_Sock ; 
};

#pragma once
#include <afxmt.h>
#include "gxobj.h"
#include "gxSockMsg.h"
#include "gxThread.h"

class CgxThreadRecv :	public CgxThread
{
public:
	CgxThreadRecv();
	virtual ~CgxThreadRecv();
	void Run() ; 
	int Init() ; 
	int ReleaseAll() ; 
	int AddItem(CgxClient * _Client) ; 
	int DelItem(CgxClient * _Client) ; 
	int DelItem(int _Idx) ; 
	SOCKET GetItemSock(int _Idx) ; 
	int SetList(CgxListObj*_DBRecv,CgxListObj*_Recv , CgxListObj*_Send) ;
	int CloseSock(CgxClient * _Client);	
	int GetCount() {return m_CurCount;} ;
private:
	int GetFdset(fd_set &_fdset);
	int FdRecv(SOCKET _Sock);
	int OnError(CgxClient * _Client , int _Error);
private:
	CgxClient **m_ClientList ;
	int m_MaxCount ; 
	int m_CurCount ; 
	int m_CurIdx ; 
	CCriticalSection m_Lock ;
	int Lock() ; 
	int Unlock() ;
	CgxListObj * m_DBRecv; 
	CgxListObj * m_Recv; 
	CgxListObj * m_Send ;
	CgxMapInt m_MapSock ;			//最大64个 快速索引

};


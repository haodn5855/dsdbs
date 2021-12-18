#pragma once
#include <afxsock.h>
#include "gxObj.h"
#include "gxMap.h"
#include "gxExpMsgPbDef.h"

class CgxClient ; 

class CgxSockMsg :public CgxObj
{
public:
	CgxSockMsg(void);
	virtual ~CgxSockMsg(void);
	int Copy(char *_Buf,int _Len) ; 
	char * GetData() ; 
	void ReleaseAll() ;
	int Send() ; 
	CgxClient * m_Client ; 
	int m_Len ; 
	DWORD m_dwMsgID;
private:
	int m_SendFlag ; 
	char * m_Data; 
};

class CgxClient :public CgxObj
{
public :
	CgxClient() ; 
	virtual ~CgxClient() ;
	int Init() ; 
	virtual void ReleaseAll() ; 
	SOCKET GetSock() ; 
	int SetSock(SOCKET _Sock) ; 
	int RecvMsg(char *_pBuf , int &_Len);
	int CloseSock() ;
	DWORD SetTickCount();
private:
	DWORD m_TickCount ; 
	char * m_Data ; 
	int m_RecvLen ;
	int m_DataLen  ; 
public :
	int m_Port ; 
	SOCKET m_Sock ; 
}; 


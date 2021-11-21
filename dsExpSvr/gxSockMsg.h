#pragma once

#include "gxObj.h"
#include "gxMap.h"

#define APP_PROTO_TYPE   20001 
#define APP_MSGHEAD_ID   3

typedef struct tagExpHead
{
	int MsgFlag;
	int MsgID;
	int MsgLen;
	int MsgSerial;
}EXP_HEAD;

class CgxClient ; 

class CgxSockMsg :public CgxObj
{
public:
	CgxSockMsg(void);
	virtual ~CgxSockMsg(void);
	int Copy(char *_Buf,int _Len) ; 
	char * GetData() ; 
	void ReleaseAll() ;
	int Send(char *_pBuf , int _nLen) ;
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
	int OnRecv(CgxListObj * _List) ; 
	int m_IsOnLine ;
	int CloseSock() ;
	DWORD SetTickCount();
private:
	DWORD m_TickCount ; 
	char * m_Data ; 
	int m_RecvLen ;
	int m_DataLen  ; 
public :
	char m_User[64] ; 
	int m_Port ; 
	SOCKET m_Sock ; 
}; 


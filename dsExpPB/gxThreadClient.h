// gxThreadClient.h: interface for the CgxThreadClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GXTHREADCLIENT_H__D7F49109_AF99_47F7_813C_CA96DEE23FD6__INCLUDED_)
#define AFX_GXTHREADCLIENT_H__D7F49109_AF99_47F7_813C_CA96DEE23FD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gxObj.h"
#include "gxSockMsg.h"
#include "gxThread.h"
#include "gxExpMsgPbDef.h"

extern int gxWriteLog(const char *_pFmt ,...) ; 
class CgxThreadClient : public CgxThread  
{
public:
	CgxThreadClient();
	virtual ~CgxThreadClient();
	virtual void Run() ;
	virtual int OnInit() ;
	CString m_IP ; 
	int m_Port ;
	int SendMsg(char *_pBuf , int _nLen);
	HWND m_Wnd ;
private:
	int m_Serial ; 
	int Connect() ; 
	SOCKET m_Sock ; 
	SOCKADDR_IN m_ServerSockAddr ; 
	CgxListObj m_SendList ;	
	int Recv() ; 
	int Send(); 
	CgxClient m_Client ; 
private:
	int OnAckInsertMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead);
	int OnAckSelectMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead);
	int OnAckUpdateMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead);
	int OnAckDeleteMsg(char *_pMsg,int _nLen,EXP_HEAD*_pHead);
};

#endif // !defined(AFX_GXTHREADCLIENT_H__D7F49109_AF99_47F7_813C_CA96DEE23FD6__INCLUDED_)

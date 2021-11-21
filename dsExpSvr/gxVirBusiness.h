#pragma once
#include "gxobj.h"
#include "gxMap.h"
#include "gxSockMsg.h"
#include "gxThread.h"

class CgxVirBusiness :	public CgxThread
{
public:
	CgxVirBusiness(void);
	virtual ~CgxVirBusiness(void);
public :
	virtual int OnInit() ;
	virtual void Run() ; 
	int SetList(CgxListObj * _DBList , CgxListObj *_Recv , CgxListObj*_Send) ;  
	int OnMsg();
public:
	CgxListObj * m_RecvList ; 
	CgxListObj * m_SendList ;
private:
protected:
};

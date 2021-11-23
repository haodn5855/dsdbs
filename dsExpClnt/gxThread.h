#pragma once
#include <gxObj.h>
using namespace zsGame;

class CgxThread :public CgxObj
{
public:
	CgxThread() ; 
	virtual ~CgxThread() ; 
	virtual void Run() ;
	virtual int OnInit() ; 
	int StartThread() ; 
	virtual int StopThread(int _WaitFlag=1) ;
	int WaitThread() ;
	int m_RunFlag ;
private:
	HANDLE m_Handle ; 
};

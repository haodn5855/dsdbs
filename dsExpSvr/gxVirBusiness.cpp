#include "StdAfx.h"
#include "gxVirBusiness.h"
#include "gxDSApp.h"

extern int g_Serial ;
CgxVirBusiness::CgxVirBusiness(void)
{
	m_RecvList = NULL ; 
	m_SendList = NULL ;
}

CgxVirBusiness::~CgxVirBusiness(void)
{
	
}
int CgxVirBusiness::OnInit()
{
return 0 ; 
}
void CgxVirBusiness::Run()
{
	for(;m_RunFlag ;)
	{
		if( OnMsg() == -1 )  
			Sleep(20) ; 
		else
			Sleep(0) ;
	}
}
int CgxVirBusiness::SetList(CgxListObj * _DBList,CgxListObj *_Recv,CgxListObj*_Send) 
{
	m_RecvList = _Recv ; 
	m_SendList = _Send ; 
	return 0 ; 
}
extern CgxDSApp g_DSApp ;
int CgxVirBusiness::OnMsg()
{
	CgxSockMsg * pSockMsg = (CgxSockMsg*)m_RecvList->GetItem() ; 
	if( pSockMsg == NULL ) 
		return -1 ; 
	char * pMsg = pSockMsg->GetData() ; 

	std::string strReq(pMsg , pSockMsg->m_Len ) ;
	std::string strAck ;
	g_DSApp.OnMsg(pSockMsg->m_dwMsgID ,strReq,strAck) ;
	pSockMsg->Send((char*)strAck.c_str() , strAck.length() );
	pSockMsg->ReleaseRef() ; 
return 0 ; 
}

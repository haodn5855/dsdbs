#pragma once
#include <afxtempl.h>
#include <afxmt.h>
#include <afxsock.h>
#include "gxObj.h"

using namespace zsGame;

typedef CList<CgxObj* ,CgxObj*>CgxList_Obj ; 
class CgxListObj: public CgxObj
{
public :
	CgxListObj() ; 
	virtual ~CgxListObj() ; 
	int AddItem(CgxObj * _Obj) ;		
	CgxObj * GetItem() ;				
	virtual void ReleaseAll() ; 
	int GetCount() ; 
private:
	int Lock() ; 
	int Unlock() ; 
	CCriticalSection m_Lock ;
	CgxList_Obj m_List_Obj; 
};

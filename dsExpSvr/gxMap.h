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
typedef CMap<SOCKET ,SOCKET ,CgxObj*,CgxObj*>CgxMap_ObjInt ;

class CgxMapInt:public CgxObj
{
public:
	CgxMapInt() ; 
	virtual ~CgxMapInt() ; 
	int AddItem(int _Sock,CgxObj*_Obj) ;
	CgxObj* GetItem(int _Sock);
	int DelItem(int _Sock) ; 
	virtual void ReleaseAll() ; 
private:
	int Lock() ; 
	int Unlock() ; 
	CCriticalSection m_Lock ;
	CgxMap_ObjInt m_Map_ObjInt ;
};


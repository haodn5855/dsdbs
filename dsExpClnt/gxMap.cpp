#include "StdAfx.h"
#include "gxMap.h"

CgxListObj::CgxListObj()
{

}
CgxListObj::~CgxListObj() 
{
	ReleaseAll() ; 
}
int CgxListObj::AddItem(CgxObj * _Obj) 
{
	if( _Obj == NULL ) 
		return -1 ; 
	Lock() ;
	_Obj->AddRef() ; 
	m_List_Obj.AddTail(_Obj) ; 
	Unlock() ;
return 0 ; 
}
CgxObj * CgxListObj::GetItem()
{
	CgxObj * Obj = NULL ; 
	Lock() ; 
	int Count = m_List_Obj.GetCount()  ; 
	if( Count )
		Obj = m_List_Obj.RemoveHead() ; 
	Unlock() ; 
return Obj ; 	
}
int CgxListObj::GetCount() 
{
	Lock() ; 
	int Count = m_List_Obj.GetCount()  ; 
	Unlock() ;
return Count ;
}
void CgxListObj::ReleaseAll() 
{
	Lock() ; 
	for( ;!m_List_Obj.IsEmpty();)
	{
		CgxObj * Obj = m_List_Obj.RemoveHead() ; 
		if( Obj!=NULL ) 
			Obj->ReleaseRef() ; 
	}
	Unlock() ;
}
int CgxListObj::Lock() 
{
	m_Lock.Lock() ;
	return 0 ;
}
int CgxListObj::Unlock() 
{
	m_Lock.Unlock() ;
	return 0 ;
}

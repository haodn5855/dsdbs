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
////////////////////////////////////////////////////////////////////////////////
CgxMapInt::CgxMapInt(void)
{
}

CgxMapInt::~CgxMapInt(void)
{
}
int CgxMapInt::AddItem(int _Sock,CgxObj *_Obj) 
{
	if(( _Sock == -1 ) || (_Obj == NULL ) )
		return -1 ;
	Lock() ;
	m_Map_ObjInt.SetAt(_Sock,_Obj) ;
	_Obj->AddRef() ;
	Unlock() ;
	return 0 ; 
}
CgxObj * CgxMapInt::GetItem(int _Sock) 
{
	CgxObj * Obj = NULL ; 
	if( _Sock == -1 ) 
		return NULL ;
	Lock() ;
	m_Map_ObjInt.Lookup(_Sock,Obj) ;
	Unlock() ;	
return Obj ; 
}
int CgxMapInt::DelItem(int _Sock) 
{
	if( _Sock  == -1 ) 
		return -1 ;
	CgxObj * Obj = GetItem(_Sock) ;
	if( Obj==NULL)
		return -2 ;
	Obj->ReleaseRef();
	Lock() ;
	if( !m_Map_ObjInt.RemoveKey(_Sock) )
	{
		Unlock() ;
		return -3; 
	}
	Unlock() ;
return 0 ;
}
void CgxMapInt::ReleaseAll() 
{
	m_Lock.Lock() ;
	int Count = m_Map_ObjInt.GetCount() ;
	POSITION ps = m_Map_ObjInt.GetStartPosition() ;
	for( int i = 0; i < Count ;i ++ ) 
	{
		SOCKET  Sock=-1 ;
		CgxObj* Item = NULL ;
		m_Map_ObjInt.GetNextAssoc(ps ,Sock,Item) ;
		if( Item !=NULL)
			Item->ReleaseRef() ;
	}
	m_Map_ObjInt.RemoveAll() ;
	m_Lock.Unlock() ;
}
int CgxMapInt::Lock() 
{
	m_Lock.Lock() ;
	return 0 ;
}
int CgxMapInt::Unlock() 
{
	m_Lock.Unlock() ;
	return 0 ;
}

////////////////////////////////////////////////////////////////////////////////

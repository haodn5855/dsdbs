/******************************************************************************
**************************鼎石数据*********************************************
*******************************************************************************
	*版权：2013-2021 郝多年 版权所有 
	*版本：1.8
	*作者：郝多年
	*微信：19843355855
	*邮箱：han_dn@sohu.com
	*免责及版权声明：
	1、使用本开源产品时必须完整保留此版权及免责声明，不能对此版权及免责声明进行
	任何增加、修改、删除等操作。
	2、使用本产品或享受相关技术支持服务等需遵守中国相关法律及所在国法律。若将产
	 品或服务用于非法目地与作者无关。作者不负连带责任，所有责任由使用者自行承担。
	3、本产品无法做到零Bug。故不承担因本产品Bug或缺陷导致的任何后果。因此,在使
	 用本产品前请务必做好相关测试。
	4、本开源产品现在或将来会提供与多个开源产品如My Sql,protobuf,redis等的接入示
	例，若接入示例存在对相应开源产品侵权请及时告知，将进行相应修改或删除;使用者
	在使用本产品时用到相应的开源产品，请自行获得相关产品的使用许可。
	5、公司或组织少于100人且服务器少于50台以及个人使用者在符合中国相关法律及所
	 在国法律，并保留本产品完整免责及版权声明。可任意复制、修改、使用、学习相关
	 代码。
	6、公司或组织大于100人或服务器大于50台的使用本产品均需获得使用许可，不论是
	直接或间接使用或何种渠道从第三方获得含有本产品的源代码或经过编译发布的产品
	均需获得使用许可。
*******************************************************************************
**************************鼎石数据*********************************************
******************************************************************************/

#include "gxDBSysMsgList.h"
#pragma warning(disable:4996)

CgxDBSysMsgMember::CgxDBSysMsgMember()
{
	m_nDSMsgMemberID=-1;
	m_nDSMsgID =1;
	memset(m_szMemberAttribute,0,sizeof(m_szMemberAttribute));
	memset(m_szMemberName,0,sizeof(m_szMemberName));
	m_nMemberClass=0;
	m_nMemberType=0;
	m_nMemberLen=0;
	m_nMemberOrder=0;
	memset(m_szDefaultStr,0,sizeof(m_szDefaultStr));
	m_nHeadMemberType = 0 ;
	memset(m_szComment,0,sizeof(m_szComment));
}
CgxDBSysMsgMember::~CgxDBSysMsgMember()
{

}
CgxDBSysStruct::CgxDBSysStruct()
{
	m_nDSSTID =-1;
	memset(m_szStructName,0,sizeof(m_szStructName));
	m_nDSSVRID=-1;
	memset(m_szComment,0,sizeof(m_szComment));
	m_nOriginalDSSTID = -1;
}
CgxDBSysStruct::~CgxDBSysStruct()
{
	ReleaseAll();	
}
void CgxDBSysStruct::ReleaseAll()
{
	m_DBMemberItems.DeleteAll() ;
}
CgxDBSysMsgMember * CgxDBSysStruct::GetMemberItem(int _nIdx)
{
	CgxDBSysMsgMember * pItem = NULL ; 
	pItem = m_DBMemberItems.GetAt(_nIdx);
return pItem ;
}
int CgxDBSysStruct::AddMemberItem(CgxDBSysMsgMember *_pItem)
{
	if( _pItem == NULL ) 
		return -1 ; 
	m_DBMemberItems.AddTail(_pItem) ; 
return 0 ; 
}
int CgxDBSysStruct::GetMemberCount()
{
return m_DBMemberItems.GetCount();
}
void CgxDBSysStruct::WriteLog() 
{
	int Count = m_DBMemberItems.GetCount();
	gxWriteLog("----	Id = %3d ,[%s]	---------------",
		m_nDSSTID,m_szStructName);
	CgxDBSysMsgMember * pItem = NULL ;
	for( int i = 0 ;i < Count ;i ++ )
	{
		pItem =m_DBMemberItems.GetAt(i);
		if( pItem == NULL)
			continue;
		gxWriteLog("		MemberId=%3d ,MsgID=%3d ,Type=%3d,Len=%3d,[%30s]" , 
			pItem->m_nDSMsgMemberID ,
			pItem->m_nDSMsgID,
			pItem->m_nMemberType,
			pItem->m_nMemberLen,
			pItem->m_szMemberName);
	}
	gxWriteLog("");
}

CgxDBSysMsg::CgxDBSysMsg()
{
	m_nDSMsgID=-1;
	m_nMsgHeadID=-1;
	m_nMsgHeadProto=0;
	m_nMsgBodyProto=0;
	m_nHeadLenInMsg=0;
	m_nSuffixStartPos=0;
	m_nIsLoginMsg=0;
	memset(m_szMsgName,0,sizeof(m_szMsgName));
	memset(m_szSuffixCount,0,sizeof(m_szSuffixCount));
	memset(m_szSuffixSumLen,0,sizeof(m_szSuffixSumLen));
	m_nSuffixStructID=-1;
	m_nDSSVRID=0;
	memset(m_szComment,0,sizeof(m_szComment));
	m_nHeadLen = 0 ;
}
CgxDBSysMsg::~CgxDBSysMsg()
{
	ReleaseAll() ;
}
int CgxDBSysMsg::AddStruct(CgxDBSysStruct *_pItem)
{
	if( _pItem == NULL ) 
		return -1 ; 
	m_DBStructItems.AddTail(_pItem) ; 
return 0 ; 
}
CgxDBSysStruct * CgxDBSysMsg::AddStruct(int _nID )
{
	CgxDBSysStruct *pItem  = NULL ;
	pItem = new CgxDBSysStruct() ;
	pItem->m_nDSSTID = _nID ;
	m_DBStructItems.AddTail(pItem) ; 
return pItem ;
}
CgxDBSysStruct *CgxDBSysMsg::GetStruct(int _nIdx)
{
	CgxDBSysStruct * pStruct = NULL ; 
	pStruct = m_DBStructItems.GetAt(_nIdx);
return  pStruct ;
}
int CgxDBSysMsg::GetStructCount() 
{
return m_DBStructItems.GetCount() ;
}
void CgxDBSysMsg::Clear()
{
	m_DBStructItems.RemoveAll() ;
}
void CgxDBSysMsg::ReleaseAll()
{
	m_DBStructItems.DeleteAll();
}
void CgxDBSysMsg::WriteLog()
{
	gxWriteLog("-----------ID = %8d ,MsgName=%s   --------------------------"  ,
		m_nDSMsgID,
		m_szMsgName );
	int Count = m_DBStructItems.GetCount();
	CgxDBSysStruct * pStruct = NULL ;
	for( int i =0 ;i < Count;i ++ )
	{
		pStruct = m_DBStructItems.GetAt(i);
		if( pStruct == NULL )
			continue;
		pStruct->WriteLog() ;
	}
	gxWriteLog("              ----DBSysMsg 结束 -----		");
	gxWriteLog("");
}
CgxDBSysMsgList::CgxDBSysMsgList(void)
{

}
CgxDBSysMsgList::~CgxDBSysMsgList(void)
{
	ReleaseAll();	
}
void CgxDBSysMsgList::ReleaseAll()
{
	m_DBMsgItems.DeleteAll() ;
	m_DBStructItems.DeleteAll() ; 
}
int CgxDBSysMsgList::CompFuncDBSysMsg(const void *_pItem1, const void *_pItem2)
{
	CgxDBSysMsg * pItem1 = *(CgxDBSysMsg **)_pItem1;
	CgxDBSysMsg * pItem2 = *(CgxDBSysMsg **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nDSMsgID  - (__int64)pItem2->m_nDSMsgID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0 ;
}
int CgxDBSysMsgList::CompFuncDBSysStruct(const void *_pItem1, const void *_pItem2)
{
	CgxDBSysStruct * pItem1 = *(CgxDBSysStruct **)_pItem1;
	CgxDBSysStruct * pItem2 = *(CgxDBSysStruct **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nDSSTID  - (__int64)pItem2->m_nDSSTID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0 ;
}
CgxDBSysMsg * CgxDBSysMsgList::GetMsg(int _nIdx)
{
return m_DBMsgItems.GetAt(_nIdx)	;
}
CgxDBSysMsg * CgxDBSysMsgList::GetMsg(char *_pMsgName) 
{
	if(_pMsgName == NULL ) 
		return NULL ;
	CgxDBSysMsg * pMsg = NULL ;
	int Count = m_DBMsgItems.GetCount() ; 
	for( int i = 0 ;i < Count;i ++ ) 
	{
		pMsg = m_DBMsgItems.GetAt(i) ; 
		if( pMsg == NULL ) 
			continue ;
		if( strcmp(pMsg->m_szMsgName , _pMsgName) ==0 )
			return pMsg ; 
	}
return NULL ;
}
CgxDBSysMsg *CgxDBSysMsgList::GetMsgOfID(int _nID)
{
	CgxDBSysMsg * pMsg = NULL ;
	CgxDBSysMsg Tmp ; 
	Tmp.m_nDSMsgID = _nID ; 
	pMsg = m_DBMsgItems.Find(&Tmp , CompFuncDBSysMsg ) ;
return pMsg ;
}
CgxDBSysStruct * CgxDBSysMsgList::GetStructOfID(int _nID)
{
	CgxDBSysStruct * pItem = NULL ; 
	CgxDBSysStruct Tmp ; 
	Tmp.m_nDSSTID = _nID ; 
	pItem =  m_DBStructItems.Find(&Tmp , CompFuncDBSysStruct) ; 
return pItem ; 
}
int CgxDBSysMsgList::InsertMsg(CgxDBSysMsg * _pMsg)
{
	if( _pMsg == NULL ) 
		return -1;
	m_DBMsgItems.Insert(_pMsg,CompFuncDBSysMsg) ; 
return 0 ; 
}
int CgxDBSysMsgList::InsertStruct(CgxDBSysStruct *_pStruct)
{
	if( _pStruct == NULL ) 
		return -1 ; 
	m_DBStructItems.Insert(_pStruct,CompFuncDBSysStruct) ; 
return 0 ; 
}
int CgxDBSysMsgList::GetMsgCount()
{
return m_DBMsgItems.GetCount() ;
}
int CgxDBSysMsgList::GetStructCount()
{
return m_DBStructItems.GetCount();
}
int CgxDBSysMsgList::SetDBSysMsgMember(CgxDBSysMsgMember *_pMember)
{
	if( _pMember == NULL ) 
		return -1;

	CgxDBSysMsg *pMsg = GetMsgOfID(_pMember->m_nDSMsgID) ;
	if( pMsg == NULL ) 
	{
		gxWriteLog("找不到相应的消息,MsgID %d" , _pMember->m_nDSMsgID) ; 
		return -2 ; 
	}
	CgxDBSysStruct *pStruct = NULL ; 
	pStruct = pMsg->AddStruct( _pMember->m_nMemberOrder) ;
	if( _pMember->m_nMemberType<2000)
	{
		CgxDBSysMsgMember * pMember = new CgxDBSysMsgMember(*_pMember);
		pStruct->AddMemberItem(pMember) ; 
	}else
	{
				pStruct->m_nOriginalDSSTID = _pMember->m_nMemberType;
		strcpy(pStruct->m_szStructName,_pMember->m_szMemberName) ;
		CgxDBSysStruct *pFind = GetStructOfID(_pMember->m_nMemberType) ;
		if( pFind == NULL ) 
		{
			gxWriteLog("无法为结构添加字段 StructID = %d" , _pMember->m_nMemberType) ; 
			return -3;
		}
		int Count = pFind->GetMemberCount() ; 
		for( int i = 0 ;i < Count ;i ++ ) 
		{
			CgxDBSysMsgMember *pTmpMember = pFind->GetMemberItem(i) ; 
			if( pTmpMember == NULL ) 
				continue; 
			CgxDBSysMsgMember * pAddMember = new CgxDBSysMsgMember(*pTmpMember);
			pStruct->AddMemberItem(pAddMember) ;
		}
	}
return 0 ; 
}
CgxDBSysStruct * CgxDBSysMsgList::GetStruct(int _nIdx)
{
return m_DBStructItems.GetAt(_nIdx);
}
void CgxDBSysMsgList::WriteLog()
{
	int Count = m_DBMsgItems.GetCount();
	CgxDBSysMsg * pMsg = NULL ;
	gxWriteLog("--------DBSysMsgList 开始-----------") ;
	for( int i = 0 ;i < Count;i ++ )
	{
		pMsg = m_DBMsgItems.GetAt(i);
		if( pMsg == NULL)
			continue;
		pMsg->WriteLog() ; 
	}
	gxWriteLog("--------DBSysMsgList 结束-----------") ;
}

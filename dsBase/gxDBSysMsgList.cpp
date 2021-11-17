/******************************************************************************
**************************��ʯ����*********************************************
*******************************************************************************
	*��Ȩ��2013-2021 �¶��� ��Ȩ���� 
	*�汾��1.8
	*���ߣ��¶���
	*΢�ţ�19843355855
	*���䣺han_dn@sohu.com
	*���𼰰�Ȩ������
	1��ʹ�ñ���Դ��Ʒʱ�������������˰�Ȩ���������������ܶԴ˰�Ȩ��������������
	�κ����ӡ��޸ġ�ɾ���Ȳ�����
	2��ʹ�ñ���Ʒ��������ؼ���֧�ַ�����������й���ط��ɼ����ڹ����ɡ�������
	 Ʒ��������ڷǷ�Ŀ���������޹ء����߲����������Σ�����������ʹ�������ге���
	3������Ʒ�޷�������Bug���ʲ��е��򱾲�ƷBug��ȱ�ݵ��µ��κκ�������,��ʹ
	 �ñ���Ʒǰ�����������ز��ԡ�
	4������Դ��Ʒ���ڻ������ṩ������Դ��Ʒ��My Sql,protobuf,redis�ȵĽ���ʾ
	����������ʾ�����ڶ���Ӧ��Դ��Ʒ��Ȩ�뼰ʱ��֪����������Ӧ�޸Ļ�ɾ��;ʹ����
	��ʹ�ñ���Ʒʱ�õ���Ӧ�Ŀ�Դ��Ʒ�������л����ز�Ʒ��ʹ����ɡ�
	5����˾����֯����100���ҷ���������50̨�Լ�����ʹ�����ڷ����й���ط��ɼ���
	 �ڹ����ɣ�����������Ʒ�������𼰰�Ȩ�����������⸴�ơ��޸ġ�ʹ�á�ѧϰ���
	 ���롣
	6����˾����֯����100�˻����������50̨��ʹ�ñ���Ʒ������ʹ����ɣ�������
	ֱ�ӻ���ʹ�û���������ӵ�������ú��б���Ʒ��Դ����򾭹����뷢���Ĳ�Ʒ
	������ʹ����ɡ�
*******************************************************************************
**************************��ʯ����*********************************************
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
	gxWriteLog("              ----DBSysMsg ���� -----		");
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
		gxWriteLog("�Ҳ�����Ӧ����Ϣ,MsgID %d" , _pMember->m_nDSMsgID) ; 
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
			gxWriteLog("�޷�Ϊ�ṹ����ֶ� StructID = %d" , _pMember->m_nMemberType) ; 
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
	gxWriteLog("--------DBSysMsgList ��ʼ-----------") ;
	for( int i = 0 ;i < Count;i ++ )
	{
		pMsg = m_DBMsgItems.GetAt(i);
		if( pMsg == NULL)
			continue;
		pMsg->WriteLog() ; 
	}
	gxWriteLog("--------DBSysMsgList ����-----------") ;
}

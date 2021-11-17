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

#include "gxByte1Msg.h"
#pragma warning(disable:4996)
CgxByte1Member::CgxByte1Member()
{
	m_nIdx=-1;			
	memset(m_szMemberName,0,sizeof(m_szMemberName));
	m_nType=0;		
	m_nLen=0;			
	m_nSize=0;		
	m_nOffSet=0;		
	m_nFlag=0;		
	m_nHeadMemberType = 0;
}
CgxByte1Member::CgxByte1Member(CgxDBSysMsgMember *_pDBSysMember)
{
	m_nIdx=-1;			
	memset(m_szMemberName,0,sizeof(m_szMemberName));
	memset(m_szDefaultStr, 0 , sizeof(m_szDefaultStr)) ;
	m_nType=0;		
	m_nLen=0;			
	m_nSize=0;		
	m_nOffSet=0;		
	m_nFlag=0;		
	m_nHeadMemberType = 0;

	if(_pDBSysMember==NULL)
		return ;

	m_nDSMsgMemberID=_pDBSysMember->m_nDSMsgMemberID;
	m_nDSMsgID	=_pDBSysMember->m_nDSMsgID;
	strcpy(m_szMemberName,_pDBSysMember->m_szMemberName);
	m_nType		=_pDBSysMember->m_nMemberType;
	m_nLen		=_pDBSysMember->m_nMemberLen;
	m_nIdx		= _pDBSysMember->m_nMemberOrder;			
	strcpy(m_szDefaultStr,_pDBSysMember->m_szDefaultStr);
	m_nHeadMemberType = _pDBSysMember->m_nHeadMemberType;
}
CgxByte1Member::~CgxByte1Member()
{
}
int GetFieldTypeLen(int _nType)
{
	int nFieldLen = 0 ;
	switch (_nType)
	{
	case GX_MSGBUF_DATATYPE_CHAR :
		nFieldLen = sizeof(char);
		break; 
	case GX_MSGBUF_DATATYPE_BYTE:
		nFieldLen = sizeof(BYTE);
		break ; 
	case GX_MSGBUF_DATATYPE_SWORD:
	case GX_MSGBUF_DATATYPE_WORD:
		nFieldLen = sizeof(WORD);
		break; 
	case GX_MSGBUF_DATATYPE_INT :
	case GX_MSGBUF_DATATYPE_UINT :
		nFieldLen = sizeof(int);
		break; 
	case GX_MSGBUF_DATATYPE_LONGLONG :
		nFieldLen = sizeof(__int64);
		break; 
	case GX_MSGBUF_DATATYPE_STR:
		nFieldLen = sizeof(char) ; 
		break ;
	case GX_MSGBUF_DATATYPE_FLOAT:
		nFieldLen = sizeof(float) ; 
		break; 
	case GX_MSGBUF_DATATYPE_DOUBLE :
		nFieldLen = sizeof(double) ; 
		break; 
	case GX_MSGBUF_DATATYPE_SUFFIX :
		nFieldLen = sizeof(MSG_SUFFIX) ;
		break; 
	default:
		break ; 
	}	
return nFieldLen;
}
CgxByte1Struct::CgxByte1Struct() 
{
	m_nID =-1;					
	m_nOffSet=0 ;				
	m_nBlackSize =0;			
	m_nItemTypeMaxSize=0;		
	memset(m_szStructName, 0 , sizeof(m_szStructName)) ;
}
CgxByte1Struct::CgxByte1Struct(CgxDBSysStruct *_pDBSysStruct) 
{
	m_nID =-1;					
	m_nOffSet=0 ;				
	m_nBlackSize =0;			
	m_nItemTypeMaxSize=0;		
	memset(m_szStructName, 0 , sizeof(m_szStructName)) ;
	SetStruct(_pDBSysStruct) ;
}

CgxByte1Struct::~CgxByte1Struct() 
{
	ReleaseAll();
}
void CgxByte1Struct::ReleaseAll()
{
	m_MemberItems.DeleteAll();
}
int CgxByte1Struct::SetStruct(CgxDBSysStruct *_pDBSysStruct)
{
	if( _pDBSysStruct == NULL ) 
		return -1 ; 
	m_nID = _pDBSysStruct->m_nDSSTID;
	strcpy(m_szStructName, _pDBSysStruct->m_szStructName);

	int Count = _pDBSysStruct->GetMemberCount() ;
	CgxByte1Member * pMember = NULL ;
	CgxDBSysMsgMember *pDBSysMember= NULL ;
	for( int i =0 ;i <Count;i ++ ) 
	{
		pDBSysMember= _pDBSysStruct->GetMemberItem(i) ;
		if( pDBSysMember == NULL ) 
			continue; 
		pMember= new CgxByte1Member(pDBSysMember);
		ResetMemberLen(pMember) ;
		m_MemberItems.AddTail(pMember);
	}
return 0 ; 
}
int CgxByte1Struct::GetSuffixTypeSize(char *_pBuf)
{
	if( _pBuf == NULL)
		return 0 ; 
	int nSize = 0 ; 
	int Count =  m_MemberItems.GetCount();
	CgxByte1Member * pItem = NULL; 
	MSG_SUFFIX Suffix ;
	for( int i = 0 ;i < Count;i ++ )
	{
		pItem = m_MemberItems.GetAt(i);
		if( pItem == NULL)
			continue ;
		if( pItem->m_nType == GX_MSGBUF_DATATYPE_SUFFIX)
		{
			memset(&Suffix , 0,sizeof(Suffix))	;
			memcpy(&Suffix, _pBuf +pItem->m_nOffSet ,sizeof(Suffix) );
			nSize += Suffix.dwSize;
		}
	}
return nSize;
}
int CgxByte1Struct::GetStaticDataLen()
{
	int nSize = 0 ; 
	int Count = m_MemberItems.GetCount(); 
	CgxByte1Member *pItem = NULL ;
	for( int i = 0 ;i < Count;i ++ )
	{
		pItem = m_MemberItems.GetAt(i);
		if( pItem == NULL)
			continue; 
		nSize += pItem->m_nSize;
	}
return nSize +m_nBlackSize; 
}
int CgxByte1Struct::InitStructSize()
{
		int Count = m_MemberItems.GetCount();
	CgxByte1Member *pItem = NULL ; 
	CgxByte1Member *pPrevItem = NULL ;
	int OffSet = 0 ;
	m_nDataLen = 0 ; 
	int nTypeOffSet = 0 ; 
	int nTypeSize = 0 ;
	for( int i = 0 ;i < Count;i ++)
	{
		pItem = m_MemberItems.GetAt(i);
		if( pItem == NULL)
			continue;
		pItem->m_nSize = pItem->m_nLen ;
		nTypeOffSet = GetTypeOffset(pItem->m_nType);
				nTypeOffSet = 1;

		m_nItemTypeMaxSize = max(m_nItemTypeMaxSize ,nTypeOffSet);
		nTypeSize = GetFieldTypeLen(pItem->m_nType);
		if(( nTypeOffSet>1) && (m_nDataLen  % nTypeOffSet !=0 ))
		{
			OffSet = m_nDataLen  % nTypeOffSet ;
			m_nDataLen +=   (nTypeOffSet - OffSet);
			if( pPrevItem!=NULL)
				pPrevItem->m_nSize += (nTypeOffSet - OffSet) ;
		}
		pItem->m_nOffSet = m_nDataLen;
		pItem->m_nSize = pItem->m_nLen;
		m_nDataLen += pItem->m_nSize;
		pPrevItem = pItem ;
	}
	if( m_nItemTypeMaxSize >0)
		OffSet = m_nDataLen %m_nItemTypeMaxSize ;
	m_nDataLen += OffSet;
return 0 ; 
}
int CgxByte1Struct::ResetMemberLen(CgxByte1Member *_pItem) 
{
	if( _pItem == NULL ) 
		return -1 ; 
		int Count = _pItem->m_nLen ;
	if( Count <1)
		Count = 1;
		_pItem->m_nLen = GetFieldTypeLen(_pItem->m_nType) * Count;
return 0 ; 
}
CgxByte1Member * CgxByte1Struct::GetMemberItem(int _nIdx)
{
	CgxByte1Member * pItem = NULL ; 
	pItem = m_MemberItems.GetAt(_nIdx);
return pItem ;
}
int CgxByte1Struct::GetMemberCount()
{
return m_MemberItems.GetCount();
}
void CgxByte1Struct::WriteLog() 
{
	int Count = m_MemberItems.GetCount();
	gxWriteLog("	Id = %3d ,Offset =%3d , BlackSize=%3d, DataLen =%3d [%s]----------",
		m_nID,m_nOffSet,m_nBlackSize, m_nDataLen,m_szStructName);
	CgxByte1Member * pItem = NULL ;
	for( int i = 0 ;i < Count ;i ++ )
	{
		pItem =m_MemberItems.GetAt(i);
		if( pItem == NULL)
			continue;
		gxWriteLog("		Idx= %3d ,Type=%3d ,Len = %3d ,Size =%3d , Offset=%3d,[%30s]" , 
			pItem->m_nIdx ,
			pItem->m_nType,
			pItem->m_nLen,
			pItem->m_nSize,
			pItem->m_nOffSet,
			pItem->m_szMemberName);
	}
	gxWriteLog("");
}
int CgxByte1Struct::GetMemberType(char *_pType)
{
	if( _pType == NULL)
		return -1 ;
	 char * p = _strupr(_pType);
	 if( strcmp(p  , "CHAR" ) ==0)
		 return GX_MSGBUF_DATATYPE_CHAR  ;
	 else if( strcmp(p  , "BYTE" ) ==0)
		 return GX_MSGBUF_DATATYPE_BYTE  ;
	 else if( strcmp(p  , "SWORD" ) ==0)
		 return GX_MSGBUF_DATATYPE_WORD  ;
	 else if( strcmp(p  , "WORD" ) ==0)
		 return GX_MSGBUF_DATATYPE_WORD  ;
	 else if( strcmp(p  , "INT" ) ==0)
		 return GX_MSGBUF_DATATYPE_INT  ;
	 else if( strcmp(p  , "UINT" ) ==0)
		 return GX_MSGBUF_DATATYPE_UINT  ;
	 else if( strcmp(p  , "DWORD" ) ==0)
		 return GX_MSGBUF_DATATYPE_UINT  ;
	 else if( strcmp(p  , "TIME_T" ) ==0)
		 return GX_MSGBUF_DATATYPE_INT  ;
	 else if( strcmp(p  , "LONG" ) ==0)
		 return GX_MSGBUF_DATATYPE_INT  ;
	 else if( strcmp(p  , "__INT64" ) ==0)
		 return GX_MSGBUF_DATATYPE_LONGLONG  ;
	 else if ( strcmp(p , "FLOAT") ==0)
		 return GX_MSGBUF_DATATYPE_FLOAT ;
	 else if ( strcmp(p , "DOUBLE") ==0)
		 return GX_MSGBUF_DATATYPE_DOUBLE ;
	 else if ( strcmp(p , "SUFFIXIDX") ==0)
		 return GX_MSGBUF_DATATYPE_SUFFIX ;
return 0 ; 
}

int CgxByte1Struct::GetTypeOffset(int _Type)
{
	int nSize = 0 ; 
	switch(_Type)
	{
	case	GX_MSGBUF_DATATYPE_CHAR			:
		nSize = sizeof(char ) ;
		break; 
	case	GX_MSGBUF_DATATYPE_BYTE			:
		nSize = sizeof(BYTE ) ;
		break; 
	case	GX_MSGBUF_DATATYPE_SWORD			:
		nSize = sizeof(short ) ;
		break; 
	case	GX_MSGBUF_DATATYPE_WORD			:
		nSize = sizeof(WORD ) ;
		break; 
	case	GX_MSGBUF_DATATYPE_INT			:
	case	GX_MSGBUF_DATATYPE_UINT			:
		nSize = sizeof( int) ;
		break; 
	case	GX_MSGBUF_DATATYPE_LONGLONG		:
		nSize = sizeof(LONGLONG ) ;
		break; 
	case	GX_MSGBUF_DATATYPE_STR			:
		nSize = sizeof(char ) ;
		break; 
	case	GX_MSGBUF_DATATYPE_BINARY		:
		nSize = sizeof( char) ;
		break; 
	case	GX_MSGBUF_DATATYPE_FLOAT :
		nSize = sizeof(float);
		break;  
	case GX_MSGBUF_DATATYPE_DOUBLE :
		nSize = sizeof(double);
		break; 
	case	GX_MSGBUF_DATATYPE_SUFFIX		:
		nSize = sizeof( DWORD) ;
		break; 
	}
return nSize;
}
CgxByte1Msg::CgxByte1Msg()
{
	m_nID =-1;				
	memset(m_szMsgName , 0,sizeof(m_szMsgName)); 
	m_SuffixStructID =-1;	
	m_SuffixCountIdx=-1;		
	m_SuffixCountSubIdx =-1;
	m_SuffixSumSizeIdx=-1;	
	m_SuffixStructSizeIdx=-1;
	m_SuffixSumSizeSubIdx =-1 ;
	m_nStructSize =0;		
	m_nStaticStructSize = 0 ; 

	m_nMsgHeadID=0;
	m_nMsgHeadProto=0;
	m_nMsgBodyProto=0;
	m_nHeadLenInMsg=0;
	m_nSuffixStartPos=0;
}

CgxByte1Msg::CgxByte1Msg(CgxDBSysMsg *_pDBSysMsg)
{
	m_nID =-1;				
	memset(m_szMsgName , 0,sizeof(m_szMsgName)); 
	m_SuffixStructID =-1;	
	m_SuffixCountIdx=-1;		
	m_SuffixCountSubIdx =-1;
	m_SuffixSumSizeIdx=-1;	
	m_SuffixStructSizeIdx=-1;
	m_SuffixSumSizeSubIdx =-1 ;
	m_nStructSize =0;		
	m_nStaticStructSize = 0 ; 

	m_nMsgHeadID=0;
	m_nMsgHeadProto=0;
	m_nMsgBodyProto=0;
	m_nHeadLenInMsg=0;
	m_nSuffixStartPos=0;

	if(_pDBSysMsg==NULL)
		return ;
	m_nID =_pDBSysMsg->m_nDSMsgID;				
	strcpy(m_szMsgName, _pDBSysMsg->m_szMsgName);
	m_SuffixStructID =_pDBSysMsg->m_nSuffixStructID;;	

	m_SuffixCountIdx=-1;		
	m_SuffixCountSubIdx=-1;
	GetMemberAndSubIdx(_pDBSysMsg->m_szSuffixCount,m_SuffixCountIdx,m_SuffixCountSubIdx);

	m_SuffixSumSizeIdx=-1;	
	m_SuffixSumSizeSubIdx =-1 ;
	GetMemberAndSubIdx(_pDBSysMsg->m_szSuffixSumLen,m_SuffixSumSizeIdx,m_SuffixSumSizeSubIdx);

		m_SuffixStructSizeIdx=-1;

	m_nStructSize =0;		
	m_nStaticStructSize = 0 ; 

	m_nMsgHeadID=_pDBSysMsg->m_nMsgHeadID;
	m_nMsgHeadProto=_pDBSysMsg->m_nMsgHeadProto;
	m_nMsgBodyProto=_pDBSysMsg->m_nMsgBodyProto;
	m_nHeadLenInMsg=_pDBSysMsg->m_nHeadLenInMsg;
	m_nSuffixStartPos=_pDBSysMsg->m_nSuffixStartPos;

	int Count = _pDBSysMsg->GetStructCount() ; 
	CgxDBSysStruct * pDBSysStruct = NULL ;
	CgxByte1Struct * pStruct = NULL ;
	for( int i =0;i <Count;i ++ ) 
	{
		pDBSysStruct = _pDBSysMsg->GetStruct(i) ; 
		if( pDBSysStruct == NULL ) 
			continue ; 
		pStruct = new CgxByte1Struct(pDBSysStruct) ; 
		if( pStruct !=NULL )
			AddStruct(pStruct) ;
	}
}
CgxByte1Msg::~CgxByte1Msg()
{
	ReleaseAll();
}

int CgxByte1Msg::GetDataLen(char *_pBuf)
{
return m_nStructSize + GetSuffixTypeSize(_pBuf);
}
int CgxByte1Msg::GetSuffixTypeSize(char *_pBuf)
{
	if( _pBuf == NULL)
		return 0 ; 
	 int nSize = 0 ;
	 int Count = m_StructItems.GetCount();
	 CgxByte1Struct * pStruct = NULL; 
	 for( int i = 0 ;i < Count;i ++ )
	 {
		pStruct = m_StructItems.GetAt(i);
		if( pStruct == NULL)
			continue;
		nSize += pStruct->GetSuffixTypeSize(_pBuf) ;
	 }
return  nSize;
}
void CgxByte1Msg::Clear()
{
	m_StructItems.RemoveAll() ;
}
void CgxByte1Msg::ReleaseAll()
{
	m_StructItems.DeleteAll();
}
CgxByte1Member * CgxByte1Msg::GetMember(int _nMember,int _Add,int _nSub)
{
	CgxByte1Struct *pStruct = NULL ;
	if(_nMember>=0)
		pStruct = GetStruct(_nMember);
	else
		pStruct = &m_SuffixStruct ; 

	if( pStruct == NULL ) 
		return NULL ;
	CgxByte1Member *pMember = pStruct->GetMemberItem(_nSub);
return pMember;
}
int CgxByte1Msg::GetMemberOffset(int _nMember,int _nAdd,int _nSub)
{
	CgxByte1Struct *pStruct = NULL ;
	if( _nMember>=0 ) 
	{
		pStruct = GetStruct(_nMember);
		if( pStruct == NULL ) 
			return -1 ;
	}else
	{
		pStruct = &m_SuffixStruct ; 
	}
	CgxByte1Member *pMember = pStruct->GetMemberItem(_nSub);
	if( pMember== NULL ) 
		return -2 ;
	int OffSet = 0 ;
	OffSet= pStruct->m_nOffSet;
	if( pMember!=NULL) 
		OffSet+=pMember->m_nOffSet;
	if(( _nMember<0) && (_nAdd>=0))
	{
		int Len = pStruct->GetStaticDataLen();
		Len = Len * _nAdd;
		OffSet +=Len ;
	}
return OffSet ;
}
int CgxByte1Msg::GetStaticDataLen()
{
	int nSize = 0 ; 
	int Count = m_StructItems.GetCount();
	CgxByte1Struct * pStruct = NULL ; 
	for( int i = 0 ;i < Count ;i ++ )
	{
		pStruct = m_StructItems.GetAt(i);
		if( pStruct == NULL)
			continue; 
		nSize += pStruct->GetStaticDataLen() ; 
	}
return nSize ; 
}
int CgxByte1Msg::AddStruct(CgxByte1Struct *_pItem)
{
	if( _pItem == NULL ) 
		return -1 ; 
	m_StructItems.AddTail(_pItem) ; 
return 0 ; 
}
CgxByte1Struct * CgxByte1Msg::AddStruct(int _nID )
{
	CgxByte1Struct *pItem  = NULL ;
	pItem = new CgxByte1Struct() ;
	pItem->m_nID = _nID ;
	m_StructItems.AddTail(pItem) ; 
return pItem ;
}
CgxByte1Struct *CgxByte1Msg::GetStruct(int _nIdx)
{
	CgxByte1Struct * pStruct = NULL ; 
	pStruct = m_StructItems.GetAt(_nIdx);
return  pStruct ;
}
int CgxByte1Msg::InitMsgSize()
{
	m_SuffixStruct.InitStructSize() ;

	int Count = m_StructItems.GetCount();
	CgxByte1Struct * pStruct = NULL ; 
	for( int i = 0;i < Count;i ++ )
	{
		pStruct = m_StructItems.GetAt(i);
		if( pStruct != NULL)
			pStruct->InitStructSize();
	}
	pStruct = NULL ; 
	CgxByte1Struct *pPrevStruct = NULL ;
	int OffSet = 0 ;
	int MsgLen = 0 ;
	for( int i = 0 ;i < Count;i ++)
	{
		pStruct = m_StructItems.GetAt(i);
		if(pStruct == NULL)
			continue;
		if(pStruct->m_nItemTypeMaxSize<=0 )
			pStruct->m_nItemTypeMaxSize = 1 ;

		OffSet = MsgLen % pStruct->m_nItemTypeMaxSize ;
		if((  pStruct->m_nItemTypeMaxSize > 1) && (OffSet  != 0))
		{
			if( pPrevStruct!=NULL)
				pPrevStruct->m_nBlackSize = pStruct->m_nItemTypeMaxSize - OffSet ;
			MsgLen += pStruct->m_nItemTypeMaxSize - OffSet ;
		}

		pStruct->m_nOffSet = MsgLen ; 
		MsgLen += pStruct->GetStaticDataLen();
		pPrevStruct = pStruct ;
	}
	if( pPrevStruct!=NULL)
	{
		if( pPrevStruct->m_nItemTypeMaxSize <=0)
			pPrevStruct->m_nItemTypeMaxSize =1 ;
		OffSet = MsgLen % pPrevStruct->m_nItemTypeMaxSize ;

		if( OffSet != 0)
		{
			pPrevStruct->m_nBlackSize =  pPrevStruct->m_nItemTypeMaxSize - OffSet ;
			MsgLen +=  pPrevStruct->m_nItemTypeMaxSize- OffSet;
		}
	}
	m_nStaticStructSize = MsgLen ;
return 0 ; 
}
int CgxByte1Msg::GetStructCount()
{
return m_StructItems.GetCount();;
}
void CgxByte1Msg::WriteLog()
{
	int StaticLen = GetStaticDataLen();
	gxWriteLog("-----Byte1-ID = %d ,MsgName=%s   Static Len = %d--------------------------"  ,
		m_nID,m_szMsgName ,StaticLen );
	int Count = m_StructItems.GetCount();
	CgxByte1Struct * pStruct = NULL ;
	for( int i =0 ;i < Count;i ++ )
	{
		pStruct = m_StructItems.GetAt(i);
		if( pStruct == NULL )
			continue;
		pStruct->WriteLog() ;
	}
	gxWriteLog("           ------------ 结束！-----------");
	gxWriteLog("");
}
CgxByte1MsgList::CgxByte1MsgList()
{
}
CgxByte1MsgList::~CgxByte1MsgList()
{
	ReleaseAll();
}
void CgxByte1MsgList::ReleaseAll()
{
	m_StructItems.DeleteAll() ; 
	m_MsgItems.DeleteAll() ;
}
int CgxByte1MsgList::Init(CgxDBSysMsgList *_pDBSysMsgList)
{
	ReleaseAll();
	if(_pDBSysMsgList == NULL ) 
		return -1 ;
	int Count = _pDBSysMsgList->GetMsgCount() ; 
	CgxDBSysMsg *pDBSysMsg = NULL ;
	CgxByte1Msg *pByte1Msg = NULL ;
	for( int i =0 ;i <Count ;i ++ ) 
	{
		pDBSysMsg = _pDBSysMsgList->GetMsg(i) ; 
		if( pDBSysMsg==NULL ) 
			continue ;
		pByte1Msg = new CgxByte1Msg(pDBSysMsg) ;
		if( pByte1Msg ==NULL ) 
			continue; 
		SetSuffixStruct(_pDBSysMsgList ,pByte1Msg) ;
		InsertMsg(pByte1Msg) ;
	}

	Count = _pDBSysMsgList->GetStructCount() ; 
	CgxDBSysStruct *pDBSysStruct = NULL ;
	CgxByte1Struct *pByte1Struct = NULL ;
	for( int i = 0 ;i <Count; i ++ ) 
	{
		pDBSysStruct = _pDBSysMsgList->GetStruct(i) ;
		if( pDBSysStruct == NULL ) 
			continue ;
		pByte1Struct = new CgxByte1Struct(pDBSysStruct) ;
		if( pByte1Struct == NULL ) 
			continue ; 
		InsertStruct(pByte1Struct) ; 
	}
	InitMsgList() ;
return 0 ; 
}
CgxByte1Msg * CgxByte1MsgList::GetMsg(int _nIdx)
{
return m_MsgItems.GetAt(_nIdx)	;
}
int CgxByte1MsgList::CompFuncByte1Msg(const void *_pItem1,const void *_pItem2)
{
	CgxByte1Msg * pItem1 = *(CgxByte1Msg **)_pItem1;
	CgxByte1Msg * pItem2 = *(CgxByte1Msg **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nID - (__int64)pItem2->m_nID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0;
}
int CgxByte1MsgList::CompFuncByte1Struct(const void *_pItem1,const void *_pItem2)
{
	CgxByte1Struct * pItem1 = *(CgxByte1Struct **)_pItem1;
	CgxByte1Struct * pItem2 = *(CgxByte1Struct **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nID - (__int64)pItem2->m_nID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0;
}

CgxByte1Msg * CgxByte1MsgList::GetMsg(char *_pMsgName) 
{
	if(_pMsgName == NULL ) 
		return NULL ;
	CgxByte1Msg * pMsg = NULL ;
	int Count = m_MsgItems.GetCount() ; 
	for( int i = 0 ;i < Count;i ++ ) 
	{
		pMsg = m_MsgItems.GetAt(i) ; 
		if( pMsg == NULL ) 
			continue ;
		if( strcmp(pMsg->m_szMsgName,_pMsgName) ==0 )
			return pMsg ; 
	}
return NULL ;
}
CgxByte1Msg *CgxByte1MsgList::GetMsgOfID(int _nID)
{
	CgxByte1Msg * pMsg = NULL ;
	CgxByte1Msg Tmp ; 
	Tmp.m_nID = _nID ; 
	pMsg = m_MsgItems.Find(&Tmp , CompFuncByte1Msg ) ;
return pMsg ;
}
CgxByte1Struct * CgxByte1MsgList::GetStructOfID(int _nID)
{
	CgxByte1Struct * pItem = NULL ; 
	CgxByte1Struct Tmp ; 
	Tmp.m_nID = _nID ; 
	pItem =  m_StructItems.Find(&Tmp , CompFuncByte1Struct) ; 
return pItem ; 
}
int CgxByte1MsgList::InsertMsg(CgxByte1Msg * _pMsg)
{
	if( _pMsg == NULL ) 
		return -1;
	m_MsgItems.Insert(_pMsg,CompFuncByte1Msg) ; 
return 0 ; 
}
int CgxByte1MsgList::InsertStruct(CgxByte1Struct *_pStruct)
{
	if( _pStruct == NULL ) 
		return -1 ; 
	m_StructItems.Insert(_pStruct,CompFuncByte1Struct) ; 
return 0 ; 
}

CgxByte1Struct * CgxByte1MsgList::GetStruct(int _nIdx)
{
return m_StructItems.GetAt(_nIdx);
}
void CgxByte1MsgList::WriteLog()
{
	int Count = m_MsgItems.GetCount();
	CgxByte1Msg * pMsg = NULL ;
	for( int i = 0 ;i < Count;i ++ )
	{
		pMsg = m_MsgItems.GetAt(i);
		if( pMsg == NULL)
			continue;
		pMsg->WriteLog() ; 
	}
}
int CgxByte1MsgList::InitMsgList()
{
	int Count = m_MsgItems.GetCount();
	CgxByte1Msg * pMsg = NULL ; 
	for( int i = 0 ;i < Count ;i ++ )
	{
		pMsg = m_MsgItems.GetAt(i);
		if( pMsg == NULL)
			continue;
		pMsg->InitMsgSize();
	}
return 0 ; 
}
int CgxByte1MsgList::SetSuffixStruct(CgxDBSysMsgList* _pDBSysMsgList,CgxByte1Msg* _pByte1Msg) 
{
	if(( _pDBSysMsgList == NULL ) ||(_pByte1Msg == NULL))
		return -1 ;
	_pByte1Msg->m_SuffixStruct.ReleaseAll() ;
	CgxDBSysStruct *pDBStruct = _pDBSysMsgList->GetStructOfID(_pByte1Msg->m_SuffixStructID) ;
	if( pDBStruct == NULL ) 
		return -2 ;
	_pByte1Msg->m_SuffixStruct.SetStruct(pDBStruct) ;
return 0 ; 
}

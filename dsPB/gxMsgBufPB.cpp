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

#include "gxMsgBufPB.h"
#include "gxDBSysMsgList.h"
#pragma warning(disable:4996)

void * WINAPI DSCreateMsgBuf(int _nID)
{
	if( _nID != DS_MSG_TYPE_GOOGLE_PROTOBUF) 
		return NULL ;
	void * pMsgBuf = new CgxMsgBufProtobuf() ;
return pMsgBuf ;
}

CgxDBSysMsgList *CgxMsgBufProtobuf::m_pSysMsgList=NULL;
CgxMultiFileErrorCollector CgxMsgBufProtobuf::m_ErrorCollector;
compiler::DiskSourceTree CgxMsgBufProtobuf::m_SourceTree;
compiler::Importer CgxMsgBufProtobuf::m_Importer(&m_SourceTree,&m_ErrorCollector) ;
DynamicMessageFactory *CgxMsgBufProtobuf::m_dynamicMessageFactory=NULL;

CgxMultiFileErrorCollector::CgxMultiFileErrorCollector(void)
{
}
CgxMultiFileErrorCollector::~CgxMultiFileErrorCollector(void)
{
}
void CgxMultiFileErrorCollector::AddError(const string& _FileName,int _nLine,int _nColumn,const string& _Message)
{
	gxWriteLog("加载 %s:%d:%d %s",_FileName.c_str(),_nLine,_nColumn,_Message.c_str()) ;
}

CgxMsgBufProtobuf::CgxMsgBufProtobuf(void)
{
	m_pMsg = NULL ;
	m_pDBSysMsg = NULL ; 
}
CgxMsgBufProtobuf::~CgxMsgBufProtobuf(void)
{
	m_pDBSysMsg = NULL ;
	if( m_pMsg !=NULL )
		delete m_pMsg ;
	m_pMsg = NULL ;
}
int CgxMsgBufProtobuf::InitProtocol(CgxDBSysMsgList *_pSysMsgList)
{
	if( _pSysMsgList ==NULL ) 
		return -1 ;
	m_pSysMsgList = _pSysMsgList;
	m_dynamicMessageFactory=new DynamicMessageFactory();
	
	char szFileName[MAX_PATH];
	memset(szFileName ,0,sizeof(szFileName));
	GetPathName(szFileName) ;
	strcat(szFileName, "dsMsg.proto") ; 
	
	StructToFile(_pSysMsgList , szFileName) ; 
	MsgToFile(_pSysMsgList , szFileName) ;
	int nRet = LoadProtoFile("dsMsg.proto");
return nRet ; 
}
int CgxMsgBufProtobuf::LoadProtoFile(char *_pFileName)
{
	if( _pFileName == NULL) 
		return -1 ;
	if( strlen(_pFileName)<1)
		return -2 ;

	char Path[MAX_PATH];
	memset(Path , 0,sizeof(Path)) ; 
	GetPathName(Path) ; 

	m_SourceTree.MapPath("",Path) ;
	if( m_Importer.Import(_pFileName) ==NULL)
		gxWriteLog("Load %s Faild!",_pFileName) ; 
	else
		gxWriteLog("Load %s OK!", _pFileName) ; 
return 0 ; 
}
int CgxMsgBufProtobuf::MsgToFile(CgxDBSysMsgList *_pSysMsgList,char *_pFileName)
{
	if( (_pSysMsgList == NULL ) || (_pFileName == NULL))
		return -1 ;
	int Count = _pSysMsgList->GetMsgCount() ; 
	CgxDBSysMsg *pMsg = NULL ;
	CgxDBSysStruct * pStruct = NULL ;
	FILE * fp = fopen(_pFileName , "a+") ;
	if( fp == NULL ) 
		return -2 ;
	char szBuf[1024];
	memset(szBuf, 0 , sizeof(szBuf)) ;
	CgxDBSysMsgMember * pMember = NULL ;
	for( int i = 0 ;i <Count;i ++ ) 
	{
		pMsg = _pSysMsgList->GetMsg(i) ; 
		if( pMsg == NULL ) 
			continue; 
		if( pMsg->m_nMsgBodyProto != DS_MSG_TYPE_GOOGLE_PROTOBUF )
			continue; 

		memset(szBuf , 0 , sizeof(szBuf)) ; 
		_snprintf(szBuf , sizeof(szBuf) , "\r\n// %s 消息ID:%d[0x%08X] %s \r\n",
			pMsg->m_szMsgName , pMsg->m_nDSMsgID,pMsg->m_nDSMsgID ,pMsg->m_szComment ) ; 
		fputs(szBuf , fp) ; 

		memset(szBuf,0,sizeof(szBuf)) ;
		_snprintf(szBuf,sizeof(szBuf),"message %s \r\n{\r\n" ,pMsg->m_szMsgName) ;
		fputs(szBuf,fp) ;
		int StructCount = pMsg->GetStructCount() ;
		for( int j = 0 ;j < StructCount ;j ++ )
		{
			pStruct = pMsg->GetStruct(j) ;
			if( pStruct == NULL ) 
				continue; 
			if( pStruct->m_nOriginalDSSTID >0) 
			{
				memset(szBuf, 0 , sizeof(szBuf)) ;
				FormatMemberStr(szBuf,sizeof(szBuf),pStruct,_pSysMsgList) ;
				fputs(szBuf,fp) ;
				continue;
			}
			int MemberCount = pStruct->GetMemberCount() ; 
			for( int n = 0 ;n < MemberCount ; n++ ) 
			{
				pMember = pStruct->GetMemberItem(n) ;
				if( pMember == NULL ) 
					continue ;
				memset(szBuf, 0 , sizeof(szBuf)) ;
				FormatMemberStr(szBuf,sizeof(szBuf),pMember,_pSysMsgList) ;
				fputs(szBuf,fp) ;
			}
		}
		fputs("}\r\n",fp) ;
	}
	if(fp != NULL) 
		fclose(fp) ;
return 0 ; 
}
int CgxMsgBufProtobuf::StructToFile(CgxDBSysMsgList *_pSysMsgList,char *_pFileName)
{
	if( (_pSysMsgList == NULL )||(_pFileName == NULL) )
		return -1 ;
	CgxList<CgxDBSysStruct> StructList;
	int Count = _pSysMsgList->GetMsgCount() ; 
	CgxDBSysMsg *pMsg = NULL ;
	CgxDBSysStruct * pStruct = NULL ;
	for( int i = 0 ;i <Count;i ++ ) 
	{
		pMsg = _pSysMsgList->GetMsg(i) ; 
		if( pMsg == NULL ) 
			continue; 
		if( pMsg->m_nMsgBodyProto!=DS_MSG_TYPE_GOOGLE_PROTOBUF ) 
			continue; 
		int StructCount = pMsg->GetStructCount() ;
		for( int j = 0 ;j < StructCount ;j ++ )
		{
			pStruct = pMsg->GetStruct(j) ;
			if( pStruct == NULL ) 
				continue; 
			if( pStruct->m_nOriginalDSSTID >0 )
			{
				CgxDBSysStruct *pTmp = _pSysMsgList->GetStructOfID(pStruct->m_nOriginalDSSTID) ;
				if( pTmp !=NULL ) 
					StructList.Insert(pTmp ,CgxDBSysMsgList::CompFuncDBSysStruct) ;
			}
		}
	}
return StructToFile(StructList,_pFileName,_pSysMsgList) ; 
}
int CgxMsgBufProtobuf::StructToFile(CgxList<CgxDBSysStruct> &_StructList,char *_pFileName,CgxDBSysMsgList *_pSysMsgList)
{
	if(_pFileName == NULL)
		return -1 ;
	int Count = _StructList.GetCount() ; 
	CgxDBSysStruct * pStruct = NULL ;
	int OldID = -2 ;
	int nSum = 0 ;
	FILE * fp = fopen(_pFileName , "wb") ;
	if( fp == NULL ) 
		return -3 ;
	for( int i = 0 ;i <Count;i ++ ) 
	{
		pStruct = _StructList.GetAt(i) ;
		if( pStruct == NULL) 
			continue;
		if( pStruct->m_nDSSTID==OldID)
			continue;
		OldID = pStruct->m_nDSSTID ;
		StructToFile(pStruct , fp,_pSysMsgList) ;
		nSum ++ ;
	}
	fclose(fp) ;
return nSum ; 
}
int CgxMsgBufProtobuf::StructToFile(CgxDBSysStruct * _pStruct ,FILE *_fp,CgxDBSysMsgList *_pSysMsgList) 
{
	if( ( _pStruct == NULL )||(_fp == NULL) )
		return -1 ;
	char szBuf[1024];
	memset(szBuf,0,sizeof(szBuf)) ;
	_snprintf(szBuf,sizeof(szBuf),"message %s \r\n{\r\n",_pStruct->m_szStructName); 
	fputs(szBuf,_fp) ;
	int Count = _pStruct->GetMemberCount() ; 
	CgxDBSysMsgMember *pMember = NULL;
	for( int i = 0 ;i <Count ;i ++ ) 
	{
		pMember = _pStruct->GetMemberItem(i) ; 
		if( pMember == NULL ) 
			continue; 
		memset(szBuf,0,sizeof(szBuf)) ;
		FormatMemberStr(szBuf,sizeof(szBuf),pMember,_pSysMsgList) ;
		fputs(szBuf,_fp) ;
	}
	fputs("}\r\n",_fp) ;
return 0 ; 
}
int CgxMsgBufProtobuf::FormatMemberStr(char *_pBuf,int _nSize,CgxDBSysStruct *_pStruct,CgxDBSysMsgList *_pSysMsgList)
{
	if( (_pBuf  == NULL )||(_pStruct == NULL) )
		return -1 ;
	char szType[64];
	memset(szType ,0, sizeof(szType));
	char szDefStr[256];
	memset(szDefStr , 0 , sizeof(szDefStr));
	GetTypeStr(szType,sizeof(szType),_pStruct->m_nOriginalDSSTID,_pSysMsgList) ;

	char szAttribute[128];
	memset(szAttribute ,0 , sizeof(szAttribute)) ;
	if( _pStruct->m_nOriginalDSSTID < 1000)
		strncpy(szAttribute,"optional",strlen("optional")) ;
	else
		strncpy(szAttribute,"repeated",strlen("optional")) ;
	_snprintf(_pBuf,_nSize,"\t%s %s %s =%d %s;\r\n",
		szAttribute,
		szType,
		_pStruct->m_szStructName,
		_pStruct->m_nDSSTID,
		szDefStr) ;
return 0 ;
}
int CgxMsgBufProtobuf::FormatMemberStr(char *_pBuf,int _nSize,CgxDBSysMsgMember *_pMember,CgxDBSysMsgList *_pSysMsgList)
{
	if( (_pBuf  == NULL )||(_pMember == NULL) )
		return -1 ;
	char szType[64];
	memset(szType,0,sizeof(szType));
	char szDefStr[256];
	memset(szDefStr,0,sizeof(szDefStr));
	if( strlen(_pMember->m_szDefaultStr ) >0)
		_snprintf(szDefStr , sizeof(szDefStr),"[default= %s]",_pMember->m_szDefaultStr) ;
	GetTypeStr(szType,sizeof(szType),_pMember->m_nMemberType,_pSysMsgList) ;
	char szAttribute[128];
	memset(szAttribute,0,sizeof(szAttribute)) ;
	_snprintf(szAttribute ,sizeof(szAttribute),"%s",_pMember->m_szMemberAttribute) ;
	if( strlen(szAttribute) <1) 
		strncpy(szAttribute,"optional",strlen("optional")) ;
	_snprintf(_pBuf,_nSize,"\t%s %s %s =%d %s;\r\n",
		szAttribute,
		szType,
		_pMember->m_szMemberName,
		_pMember->m_nMemberOrder,
		szDefStr) ;
return 0 ;
}
int CgxMsgBufProtobuf::GetTypeStr(char *_pStr,int _nLen,int _nType,CgxDBSysMsgList *_pSysMsgList)
{
	switch(_nType)
	{
	case GX_MSGBUF_DATATYPE_CHAR		 :
		_snprintf(_pStr,_nLen,"%s","int32") ; 
		break;
	case GX_MSGBUF_DATATYPE_BYTE		 :
		_snprintf(_pStr,_nLen,"%s","uint32") ; 
		break;
	case GX_MSGBUF_DATATYPE_SWORD	 :
		_snprintf(_pStr,_nLen,"%s","int32") ; 
		break;
	case GX_MSGBUF_DATATYPE_WORD		 :
		_snprintf(_pStr,_nLen,"%s","uint32") ; 
		break;
	case GX_MSGBUF_DATATYPE_INT		 :
		_snprintf(_pStr,_nLen,"%s","int32") ; 
		break;
	case GX_MSGBUF_DATATYPE_UINT		 :
		_snprintf(_pStr,_nLen,"%s","uint32") ; 
		break;
	case GX_MSGBUF_DATATYPE_STR		 :
		_snprintf(_pStr,_nLen,"%s","string") ; 
		break;
	case GX_MSGBUF_DATATYPE_RAWSTR	 :
	case GX_MSGBUF_DATATYPE_BINARY	 :
		_snprintf(_pStr,_nLen,"%s","string") ; 
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG	 :
		_snprintf(_pStr,_nLen,"%s","int64") ; 
		break;
	case GX_MSGBUF_DATATYPE_FLOAT	 :
		_snprintf(_pStr,_nLen,"%s","float") ; 
		break;
	case GX_MSGBUF_DATATYPE_DATE		 :
		_snprintf(_pStr,_nLen,"%s","暂不支持") ; 
		return -2;
	case GX_MSGBUF_DATATYPE_DECIMAL	 :
		_snprintf(_pStr,_nLen,"%s","暂不支持") ; 
		return -2;
	case GX_MSGBUF_DATATYPE_DOUBLE	 :
		_snprintf(_pStr,_nLen,"%s","double") ; 
		break;
	case GX_MSGBUF_DATATYPE_SUFFIX	 :
		_snprintf(_pStr,_nLen,"%s","string") ; 
		break;
	default  :
		if( (_nType > GX_MSGBUF_DATATYPE_MAX )&&(_pSysMsgList !=NULL) )
		{
			CgxDBSysStruct * pStruct = _pSysMsgList->GetStructOfID(_nType) ;
			if( pStruct != NULL ) 
				_snprintf(_pStr,_nLen,"%s",pStruct->m_szStructName) ;
		}else
			return -1 ;
		break;
	}
return 0 ;
}
int CgxMsgBufProtobuf::InitMsg(int _nMsgID,char *_pMsgBuf,int _nMsgLen) 
{
	if( m_pSysMsgList == NULL ) 
		return -1 ;
	m_pDBSysMsg = m_pSysMsgList->GetMsgOfID(_nMsgID) ;
	if( m_pDBSysMsg == NULL ) 
		return -2 ;
	m_pMsg = CreateMessage(m_pDBSysMsg->m_szMsgName) ; 
	if( m_pMsg == NULL ) 
		return -3 ;
	if( (_pMsgBuf!=NULL ) && (_nMsgLen>0))
	{
		bool GetRet =m_pMsg->ParseFromArray(_pMsgBuf , _nMsgLen) ;
		if( !GetRet)
			gxWriteLog("收消息失败MsgID: %d ",_nMsgID );
	}
return 0 ;
}
void CgxMsgBufProtobuf::ReleaseAll()  
{
}
int CgxMsgBufProtobuf::GetHeadLen()  	
{
return 0 ;
}
GX_MSG* CgxMsgBufProtobuf::CreateMessage(const std::string& _TypeName)
{
	GX_MSG* pMessage = NULL;
	const Descriptor* descriptor =	DescriptorPool::generated_pool()->FindMessageTypeByName(_TypeName);
	if (descriptor)
	{
		const Message* pPrototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (pPrototype!=NULL)
			pMessage = pPrototype->New();
	}else 
	{
		descriptor = m_Importer.pool()->FindMessageTypeByName(_TypeName);
		if(descriptor)
			pMessage = m_dynamicMessageFactory->GetPrototype(descriptor)->New();
		else
			gxWriteLog("not found message type %s",_TypeName.c_str()) ; 
	}
	return pMessage;
}
const FieldDescriptor * CgxMsgBufProtobuf::GetProtoBufField(const Descriptor* _pDescriptor,char* _pName)
{
	const FieldDescriptor * pField = NULL ; 
	if(( _pDescriptor == NULL )||(_pName==NULL))
		return pField ; 
	if(strlen( _pName ) >0) 
		pField = _pDescriptor->FindFieldByName(_pName) ;
return pField ; 
}
const FieldDescriptor * CgxMsgBufProtobuf::GetProtoBufField(const Descriptor* _pDescriptor,int _nNum)
{
	const FieldDescriptor * pField = NULL ; 
	if( _pDescriptor == NULL )
		return pField ; 
	pField = _pDescriptor->FindFieldByNumber(_nNum) ; 
return pField ; 
}
int CgxMsgBufProtobuf::WriteMsg(GX_MSG *_pMsg)
{
	if( _pMsg == NULL ) 
		return -1 ;
	const Reflection* pReflection = _pMsg->GetReflection();
	const Descriptor* pDescriptor = _pMsg->GetDescriptor();
	const FieldDescriptor* pField =  NULL ;
	int Count = pDescriptor->field_count() ;
	GX_MSG * pSubMsg = NULL ;
	for( int i = 0;i < Count ;i ++ ) 
	{
		pField = pDescriptor->field(i) ; 
		if( pField == NULL ) 
			continue ;
		if( pField->is_repeated()) 
		{
			gxWriteLog("-----Repeated--- %s ---" ,pField->name().c_str() ) ;
			int Size =  pReflection->FieldSize(*_pMsg,pField) ;
			for( int j = 0;j < Size ;j ++ ) 
			{
				pSubMsg = pReflection->MutableRepeatedMessage(_pMsg,pField,j) ;
				WriteMsg(pSubMsg) ;
			}
			gxWriteLog("-----Repeated--- End ---") ;
			continue ;
		}
		string Value = GetFieldString(_pMsg,pField) ;	
		if( Value.empty())
			continue ;
		gxWriteLog(" %s  %s",pField->name().c_str(),Value.c_str()) ; 
	}
return 0 ; 
}
string CgxMsgBufProtobuf::GetFieldString(GX_MSG *_pMsg,const FieldDescriptor * _pField)
{
	string Ret ; 
	if( (_pMsg == NULL)||(_pField==NULL) )
		return Ret ; 
	if(_pField->label() == FieldDescriptor::LABEL_REPEATED)
		return Ret ;
	const Reflection* pReflection = _pMsg->GetReflection();
	if( pReflection == NULL) 
		return Ret ; 
	char Tmp[128] ;
	memset(Tmp ,0,sizeof(Tmp)) ;
	switch(_pField->type())
	{
    case FieldDescriptor::TYPE_FIXED64 :
    case FieldDescriptor::TYPE_INT64 :
    case FieldDescriptor::TYPE_UINT64 :
		_snprintf(Tmp,sizeof(Tmp),"%I64d",pReflection->GetInt64(*_pMsg,_pField)) ;
		Ret = Tmp ;
		break; 
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
	case FieldDescriptor::TYPE_UINT32 :
		_snprintf(Tmp,sizeof(Tmp),"%d",pReflection->GetInt32(*_pMsg,_pField)) ;
		Ret = Tmp ;
		break; 
    case FieldDescriptor::TYPE_STRING :
		Ret = pReflection->GetString(*_pMsg,_pField) ;
		break; 
    case FieldDescriptor::TYPE_DOUBLE :
		_snprintf(Tmp,sizeof(Tmp),"%f",pReflection->GetDouble(*_pMsg,_pField)) ;
		Ret = Tmp ;
		break; 
    case FieldDescriptor::TYPE_FLOAT :
		_snprintf(Tmp,sizeof(Tmp),"%f",pReflection->GetFloat(*_pMsg,_pField)) ;
		Ret = Tmp ;
	  break; 
    case FieldDescriptor::TYPE_BOOL :
		_snprintf(Tmp,sizeof(Tmp),"%d",pReflection->GetBool(*_pMsg,_pField)) ;
		Ret = Tmp ;
		break; 
	default : 
		gxWriteLog( "not support type  %d ", _pField->type() );
	}
return Ret ; 
}
int CgxMsgBufProtobuf::WriteDatalog(char*_pBuf,int _nBufLen)							
{
	if( m_pMsg == NULL ) 
		return -1 ;
	const Reflection* pReflection = m_pMsg->GetReflection();
	const Descriptor* pDescriptor = m_pMsg->GetDescriptor();
	const FieldDescriptor* pField =  NULL ;
	int Count = pDescriptor->field_count() ;
	GX_MSG * pSubMsg = NULL ;
	for( int i = 0;i < Count ;i ++ ) 
	{
		pField = pDescriptor->field(i) ; 
		if( pField == NULL ) 
			continue ;
		if( pField->is_repeated()) 
		{
			gxWriteLog("-----Repeated--- %s ---",pField->name().c_str() ) ;
			int Size =  pReflection->FieldSize(*m_pMsg,pField) ;
			for( int j = 0;j < Size ;j ++ ) 
			{
				pSubMsg = pReflection->MutableRepeatedMessage(m_pMsg,pField,j) ;
				WriteMsg(pSubMsg) ;
			}
			gxWriteLog("-----Repeated--- End ---") ;
			continue ;
		}
		string Value = GetFieldString(m_pMsg,pField) ;	
		if( Value.empty())
			continue ;
		gxWriteLog(" %s  %s",pField->name().c_str(),Value.c_str()) ; 
	}
return 0 ;
}
int CgxMsgBufProtobuf::SetField(GX_MSG *_pSubMsg,const FieldDescriptor* _pField,const Reflection* _pReflection,void*_pData,int _nDataLen)
{
	if( ( m_pMsg == NULL )||(_pField==NULL)||(_pReflection==NULL)||(_pData==NULL) )
		return -1 ; 
	GX_MSG *pMsg = m_pMsg ;
	if( _pSubMsg !=NULL ) 
		pMsg = _pSubMsg;

	switch(_pField->type())
	{
	case FieldDescriptor::TYPE_FIXED64 :
	case FieldDescriptor::TYPE_INT64 :
		_pReflection->SetInt64(pMsg,_pField,*(__int64*)_pData);
		break;
	case FieldDescriptor::TYPE_UINT64 :
		_pReflection->SetInt64(pMsg,_pField,*(__int64*)_pData);
		break;
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
		_pReflection->SetInt32(pMsg,_pField,*(int*)_pData);
		break;
	case FieldDescriptor::TYPE_UINT32 :
		_pReflection->SetInt32(pMsg,_pField,*(int*)_pData);
		break;
	case FieldDescriptor::TYPE_STRING :
		_pReflection->SetString(pMsg,_pField,(char*)_pData);
		break;
	case FieldDescriptor::TYPE_DOUBLE :
		_pReflection->SetDouble(pMsg,_pField,*(double*)_pData);
		break;
	case FieldDescriptor::TYPE_FLOAT :
		_pReflection->SetFloat(pMsg,_pField,*(float*)_pData);
		break;
	case FieldDescriptor::TYPE_BOOL :
		_pReflection->SetBool(pMsg,_pField,*(bool*)_pData);
		break;
	default : 
		gxWriteLog( "not support type  %d", _pField->type() );
	}
return 0 ; 
}
int CgxMsgBufProtobuf::GetFieldInfo(const FieldDescriptor ** _ppField ,const Reflection** _ppReflection,int _nMember,int _nAdd,int _nSub,GX_MSG **_pSubMsg)
{
	if( m_pMsg == NULL ) 
		return -1 ;
	const Reflection* pReflection = m_pMsg->GetReflection();
	const Descriptor* pDescriptor = m_pMsg->GetDescriptor();

	if(( pReflection == NULL )||(pDescriptor == NULL) )
		return -1 ;
	const FieldDescriptor* pField =  NULL ;
	int nMember = _nMember +1 ;
	GX_MSG * pSubMsg = NULL ;
	pField = pDescriptor->FindFieldByNumber(nMember);
	if( pField == NULL ) 
	{
		gxWriteLog("Google ProtoBuf 协议配置错误！未找到相应成员！MAS(%d,%d,%d)",
			nMember,_nAdd,_nSub) ;
		return -1 ;
	}
	if((! pField->is_repeated()) && (pField->type() != FieldDescriptor::CPPTYPE_MESSAGE ))
	{
		*_ppField = pField;
		*_ppReflection = pReflection ;
		return 0 ;
	}
	if(( pField->is_repeated()) && (pField->type() == FieldDescriptor::TYPE_MESSAGE ))
	{
		const FieldDescriptor* pSubField =  NULL ;
		const Reflection* pSubReflection = NULL;
		const Descriptor* pSubDescriptor = NULL;
		int Size = pReflection->FieldSize(*m_pMsg,pField) ; 
		for( int i = Size ; i <= _nAdd  ; i ++ ) 
			pReflection->AddMessage(m_pMsg ,pField);
		if( _nAdd <0)
		{
			gxWriteLog("配置错误！Add取傎小于零。MAS（%d,%d,%d）",_nMember,_nAdd,_nSub) ; 
			return -1 ;
		}else
			pSubMsg = pReflection->MutableRepeatedMessage(m_pMsg,pField,_nAdd) ;

		if( pSubMsg!=NULL ) 
		{
			pSubReflection = pSubMsg->GetReflection();
			pSubDescriptor = pSubMsg->GetDescriptor();
			if(( pSubReflection == NULL ) || (pSubDescriptor == NULL ))
				return -1 ;
			pSubField = pSubDescriptor->FindFieldByNumber(_nSub);
			*_ppField = pSubField;
			*_ppReflection = pSubReflection ;
			*_pSubMsg = pSubMsg;
			return 0 ;
		}else
		{
			gxWriteLog("Google ProtoBuf 协议配置错误！未找到相应子消息！MAS(%d,%d,%d)",
				nMember,_nAdd,_nSub) ;
			return -1 ;			
		}
	}
	gxWriteLog("Google ProtoBuf 协议配置错误！成员类型和属性不匹配！MAS(%d,%d,%d)",
		nMember,_nAdd,_nSub) ;
return -2 ; 
}
int CgxMsgBufProtobuf::SetData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub) 
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return -1 ;
return SetField(pSubMsg,pField ,pReflection,_pData,_nDataLen) ; 
}
void* CgxMsgBufProtobuf::GetData(int _nMember,int _nAdd,int _nSub) 
{
	int nDataLen = 0 ;
return GetData(nDataLen,_nMember,_nAdd,_nSub) ;
}
void* CgxMsgBufProtobuf::GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) 
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg=NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	GX_MSG * pMsg = m_pMsg;
	if( pSubMsg !=NULL ) 
		pMsg = pSubMsg; 
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_STRING  :
		_nDataLen = pReflection->GetStringReference(*pMsg,pField,NULL ).length() ;
		return (char*)pReflection->GetStringReference(*pMsg,pField , NULL ).c_str() ;
		break;
	default:
		break;
	}
return NULL ;
}
int CgxMsgBufProtobuf::GetDataLen(int _nMember,int _nAdd,int _nSub) 
{
	int nDataLen = 0 ;
	GetData(nDataLen,_nMember,_nAdd,_nSub) ;
return nDataLen ;
}
int CgxMsgBufProtobuf::GetDataType(int _nMember,int _nAdd,int _nSub)
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	int nRet = GX_MSGBUF_DATATYPE_MAX ;
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_FIXED64 :
	case FieldDescriptor::TYPE_INT64 :
		nRet = GX_MSGBUF_DATATYPE_LONGLONG ;
		break;
	case FieldDescriptor::TYPE_UINT64 :
		nRet = GX_MSGBUF_DATATYPE_LONGLONG ;
		break;
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
		nRet = GX_MSGBUF_DATATYPE_INT ;
		break;
	case FieldDescriptor::TYPE_UINT32 :
		nRet = GX_MSGBUF_DATATYPE_UINT ;
		break;
	case FieldDescriptor::TYPE_STRING :
		nRet = GX_MSGBUF_DATATYPE_STR ;
		break;
	case FieldDescriptor::TYPE_DOUBLE :
		nRet = GX_MSGBUF_DATATYPE_DOUBLE ;
		break;
	case FieldDescriptor::TYPE_FLOAT :
		nRet = GX_MSGBUF_DATATYPE_FLOAT ;
		break;
	case FieldDescriptor::TYPE_BOOL :
		nRet = GX_MSGBUF_DATATYPE_INT ;
		break;
	default:
		break;
	}
return nRet ;
}
int CgxMsgBufProtobuf::SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub) 
{
return SetData(&_nData,sizeof(_nData),_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::SetShortData(short _nData ,int _nMember,int _nAdd,int _nSub) 
{
return SetData(&_nData,sizeof(_nData),_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) 
{
return SetData(&_fData,sizeof(_fData),_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) 
{
return SetData(&_Data,sizeof(_Data),_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::SetInt64Data(__int64 _Data,int _nMember,int _nAdd,int _nSub)  
{
return SetData(&_Data,sizeof(_Data),_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::SetSuffixData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub)
{
return SetData(_pData,_nDataLen,_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::GetIntData(int _nMember,int _nAdd,int _nSub)
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	int nRet = 0 ;
	GX_MSG * pMsg = m_pMsg; 
	if( pSubMsg!=NULL ) 
		pMsg = pSubMsg ;
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_FIXED64 :
	case FieldDescriptor::TYPE_INT64 :
	case FieldDescriptor::TYPE_UINT64 :
		return (int)pReflection->GetInt64(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
	case FieldDescriptor::TYPE_UINT32 :
		return pReflection->GetInt32(*pMsg,pField);
		break;
    case FieldDescriptor::TYPE_DOUBLE :
		return (int)pReflection->GetDouble(*pMsg,pField);
		break;
    case FieldDescriptor::TYPE_FLOAT :
		return (int)pReflection->GetFloat(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_BOOL :
		return (int)pReflection->GetBool(*pMsg,pField);
		break;
	default:
		break;
	}
return 0 ;
}
short CgxMsgBufProtobuf::GetShortData(int _nMember,int _nAdd,int _nSub)
{
return (short)GetIntData(_nMember,_nAdd,_nSub) ;
}
float CgxMsgBufProtobuf::GetFloatData(int _nMember,int _nAdd,int _nSub)
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	int nRet = 0 ;
	GX_MSG *pMsg = m_pMsg ;
	if( pSubMsg!=NULL ) 
		pMsg = pSubMsg; 
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_FIXED64 :
	case FieldDescriptor::TYPE_INT64 :
    case FieldDescriptor::TYPE_UINT64 :
		return (float)pReflection->GetInt64(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
	case FieldDescriptor::TYPE_UINT32 :
		return (float)pReflection->GetInt32(*pMsg,pField);
		break;
    case FieldDescriptor::TYPE_DOUBLE :
		return (float)pReflection->GetDouble(*pMsg,pField);
		break;
    case FieldDescriptor::TYPE_FLOAT :
		return pReflection->GetFloat(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_BOOL :
		return pReflection->GetBool(*pMsg,pField);
		break;
	default:
		break;
	}
return 0 ;
}
double CgxMsgBufProtobuf::GetDoubleData(int _nMember,int _nAdd,int _nSub) 
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	GX_MSG *pMsg = m_pMsg ;
	if( pSubMsg!=NULL ) 
		pMsg = pSubMsg; 

	int nRet = 0 ;
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_FIXED64 :
	case FieldDescriptor::TYPE_INT64 :
	case FieldDescriptor::TYPE_UINT64 :
		return (float)pReflection->GetInt64(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
	case FieldDescriptor::TYPE_UINT32 :
		return pReflection->GetInt32(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_DOUBLE :
		return pReflection->GetDouble(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_FLOAT :
		return pReflection->GetFloat(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_BOOL :
		return pReflection->GetBool(*pMsg,pField);
		break;
	default:
		break;
	}
return 0 ;
}
__int64 CgxMsgBufProtobuf::GetInt64Data(int _nMember,int _nAdd,int _nSub)
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	int nRet = 0 ;
	GX_MSG *pMsg = m_pMsg ;
	if( pSubMsg!=NULL ) 
		pMsg = pSubMsg; 
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_FIXED64 :
    case FieldDescriptor::TYPE_INT64 :
    case FieldDescriptor::TYPE_UINT64 :
		return pReflection->GetInt64(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_INT32 :
	case FieldDescriptor::TYPE_UINT32 :
		return pReflection->GetInt32(*pMsg,pField);
		break;
    case FieldDescriptor::TYPE_DOUBLE :
		return (__int64)pReflection->GetDouble(*pMsg,pField);
		break;
    case FieldDescriptor::TYPE_FLOAT :
		return (__int64)pReflection->GetFloat(*pMsg,pField);
		break;
	case FieldDescriptor::TYPE_BOOL :
		return pReflection->GetBool(*pMsg,pField);
		break;
	default:
		break;
	}
return 0 ;
}
void* CgxMsgBufProtobuf::GetSuffixData(int _nMember,int _nAdd,int _nSub)
{
	int nDataLen = 0 ;
return GetData(nDataLen,_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufProtobuf::GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) 
{
return GetDataLen(_nMember,_nAdd,_nSub) ;
}
void* CgxMsgBufProtobuf::GetSuffixData(int &_nSuffixLen,int _nMember,int _nAdd,int _nSub)
{
return GetData(_nSuffixLen,_nMember,_nAdd,_nSub)  ;
}
int CgxMsgBufProtobuf::GetDataStr(char *_pstrValue,int &_nValueLen,int _nMember,int _nAdd,int _nSub) 
{
	const Reflection* pReflection = NULL;
	const FieldDescriptor* pField = NULL;
	GX_MSG *pSubMsg= NULL;
	if( GetFieldInfo(&pField,&pReflection,_nMember,_nAdd,_nSub,&pSubMsg) <0 ) 
		return NULL ;
	if(pField->label() == FieldDescriptor::LABEL_REPEATED)
		return NULL ;
	GX_MSG *pMsg = m_pMsg ;
	if( pSubMsg!=NULL ) 
		pMsg = pSubMsg; 
	switch(pField->type())
	{
	case FieldDescriptor::TYPE_BYTES :
	case FieldDescriptor::TYPE_STRING  :
		{
			std::string str = pReflection->GetString(*pMsg,pField ) ;
			if( _nValueLen >= (int)str.length() )
				strncpy(_pstrValue , str.c_str() , str.length()) ;
			_nValueLen = str.length();
		}
		break;
	case FieldDescriptor::CPPTYPE_BOOL :
		_snprintf(_pstrValue,_nValueLen,"%d",pReflection->GetBool(*pMsg,pField)) ;
		_nValueLen = strlen(_pstrValue) ; 
		break;
	case FieldDescriptor::TYPE_INT32 :
	case FieldDescriptor::TYPE_UINT32 :
		_snprintf(_pstrValue,_nValueLen,"%d",pReflection->GetInt32(*pMsg,pField)) ;
		_nValueLen = strlen(_pstrValue) ; 
		break;
	case FieldDescriptor::TYPE_INT64 :
	case FieldDescriptor::TYPE_UINT64 :
		_snprintf(_pstrValue,_nValueLen,"%I64d" ,pReflection->GetInt64(*pMsg,pField)) ;
		_nValueLen = strlen(_pstrValue) ; 
		break;
	case FieldDescriptor::TYPE_FLOAT :
		_snprintf(_pstrValue,_nValueLen,"%f",pReflection->GetFloat(*pMsg,pField)) ;
		_nValueLen = strlen(_pstrValue) ; 
		break;
	case FieldDescriptor::TYPE_DOUBLE :
		_snprintf(_pstrValue,_nValueLen,"%f",pReflection->GetDouble(*pMsg,pField)) ;
		_nValueLen = strlen(_pstrValue) ; 
		break;
	default:
		break;
	}
return 0 ;
}
int CgxMsgBufProtobuf::BeginAddData()
{
return 0 ;
}
int CgxMsgBufProtobuf::EndAddData(int _nAddCount)
{
	if( m_pMsg == NULL ) 
		return -1 ;
	m_pMsg->SerializeToString(&m_strData) ;
return 0 ;
}
int CgxMsgBufProtobuf::GetDataLen() 
{
return m_strData.length(); ;
}
void * CgxMsgBufProtobuf::GetData() 
{
return (void*)m_strData.c_str() ;
}
int CgxMsgBufProtobuf::GetHeadInfo(CgxMsgHeadInfo &_Info)
{
return 0 ;
}

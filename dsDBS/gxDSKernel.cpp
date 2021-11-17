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

#include "gxDSKernel.h"
#include "gxMsgBufByte1.h"
#pragma warning(disable:4996)

CgxSysInterfaceCfgList CgxDSKernel::m_SysInterfaceCfgItems;

CgxDSKernel::CgxDSKernel(void)
{
	m_bDebugFlag = FALSE;
	m_nDSSVRID = -1;
}
CgxDSKernel::~CgxDSKernel(void)
{
	ReleaseAll() ;
}
void CgxDSKernel::ReleaseAll() 
{
	m_DBSysMsgItems.ReleaseAll() ;
	m_DBSysMsgPlayItems.ReleaseAll() ;
	m_DBSysDBSvrInfoItems.DeleteAll() ;
}
int CgxDSKernel::Init(int _nDBType,char *_pDBName,char *_pDsnName,char *_pIP,int _nPort,int _nCnntCount,char*_pUser,char *_pPass)
{
	gxWriteLog("") ;
	gxWriteLog("          -----------系统初始化-------------               ") ;
	int nRet = InitDB(_nDBType,_pDBName,_pDsnName,_pIP,_nPort,_nCnntCount,_pUser,_pPass);
	if( nRet <0) 
	{
		gxWriteLog("加载系统配置出错！[%d]" , nRet) ;
		return nRet;
	}

	nRet =InitDBSvrConnect();
	if( nRet <0)
	{
		gxWriteLog("初始化应用数据库联接失败！[%d]" , nRet) ;
		return nRet;
	}

	nRet = InitProtocol() ;
	if( nRet <0)
	{
		gxWriteLog("初始化协议失败！[%d]" , nRet) ;
		return nRet;
	}

	nRet = InitDBSysMsgHeadLen();
	if( nRet <0)
	{
		gxWriteLog("初始化各消息头长度失败！[%d]" , nRet) ;
		return nRet;
	}
	nRet = InitAckMsgInfo();
	if( nRet <0)
	{
		gxWriteLog("初始化应答消息信息失败！[%d]" , nRet) ;
		return nRet;
	}
	gxWriteLog("          -----------系统初始化成功！-------------               ") ;
	gxWriteLog("") ;
return 0 ;
}
int CgxDSKernel::InitDB(int _nDBType,char *_pDBName,char *_pDsnName,char *_pIP,int _nPort,int _nCnntCount,char *_pUser,char *_pPass)
{
	if(( _pDBName == NULL) || (_pDsnName == NULL)||(_pUser==NULL)||(_nCnntCount <0)||(_pPass ==NULL))
		return -1 ;
	m_SysDBInfo.m_nDBType = _nDBType ;
	strcpy(m_SysDBInfo.m_szDBName , _pDBName) ;
	strcpy(m_SysDBInfo.m_szDSNName , _pDsnName) ;
	strcpy(m_SysDBInfo.m_szIP , _pIP) ;
	strcpy(m_SysDBInfo.m_szDBUserName, _pUser);
	strcpy(m_SysDBInfo.m_szDBPassword , _pPass) ;
	m_SysDBInfo.m_nPort  = _nPort;
	m_SysDBInfo.m_nConnectCount = _nCnntCount;
	if( m_SysDBInfo.InitDB()<0 )
		gxWriteLog("初始化系统数据库联接失败！") ;
	else
		gxWriteLog("初始化系统数据库联接成功！");

	int nRet =LoadDBSys(m_SysDBInfo.GetDBPool() , m_SysDBInfo.m_nDBType) ;
	if( nRet >=0)
		gxWriteLog("加载鼎石系统配置表成功！") ;
	else
		gxWriteLog("加载鼎石系统配置表失败！[%d]" , nRet) ;

	if( m_bDebugFlag)
		m_DBSysMsgItems.WriteLog() ;
return 0 ; 
}
int CgxDSKernel::LoadDBSys(void *_pDBPool,int _nDBType) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	m_DBSysMsgItems.ReleaseAll() ;
	m_DBSysMsgPlayItems.ReleaseAll();

	if(LoadDBSys( _pDBPool,_nDBType,m_nDSSVRID) <0)
	{
		gxWriteLog("加载消息定义表失败！ ");
		return -10;
	}
	else
		gxWriteLog("加载消息定义表成功！ 加载%d个。",m_DBSysMsgItems.GetMsgCount());

	if( m_DBSysMsgPlayItems.LoadDBSys(_pDBPool,_nDBType,m_nDSSVRID) <0)
	{
		gxWriteLog("加载消息处理表失败！ ");
		return -20;
	}else
		gxWriteLog("加载消息处理表成功！加载%d个。 ",m_DBSysMsgPlayItems.GetMsgPlayCount());

	if( LoadDBSysDBSvrInfo(_pDBPool,_nDBType) <0)
	{
		gxWriteLog("加载数据库服务信息表失败！ ");
		return -30;
	}else
		gxWriteLog("加载数据库服务信息表成功！ ");

	gxWriteLog("开始加载数据及协议接口配置表！ ");
	if(m_SysInterfaceCfgItems.LoadSysInterfaceCfg(m_nDSSVRID,_pDBPool,_nDBType) <0)
	{
		gxWriteLog("加载数据及协议接口配置表失败！ ");
		return -50;
	}else
		gxWriteLog("加载数据及协议接口配置表成功！ ");
return 0 ; 
}
int CgxDSKernel::LoadDBSysDBSvrInfo(void *_pDBPool,int _nDBType) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 

	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"DSDBSID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DBType"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DBName"				,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"DSNName"				,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"IP"					,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"Port"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ConnectCount"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DBUserName"			,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"DBPassword"			,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	_snprintf(szWhere,sizeof(szWhere)," 1 = 1 order by DBType,DSDBSID");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_DBSvrInfo") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysDBSvrInfo *pDBInfo = new CgxDBSysDBSvrInfo() ;
			if( pDBInfo==NULL)
				continue;
			pDBInfo->m_nDSDBSID					= *(int*) pInfo[0].pVar;
			pDBInfo->m_nDBType					= *(int*) pInfo[1].pVar;
			strcpy(pDBInfo->m_szDBName			, (char*) pInfo[2].pVar);
			strcpy(pDBInfo->m_szDSNName			, (char*) pInfo[3].pVar);
			strcpy(pDBInfo->m_szIP				, (char*) pInfo[4].pVar);
			pDBInfo->m_nPort					= *(int*) pInfo[5].pVar;
			pDBInfo->m_nConnectCount			= *(int*) pInfo[6].pVar;
			strcpy(pDBInfo->m_szDBUserName		, (char*) pInfo[7].pVar);
			strcpy(pDBInfo->m_szDBPassword		, (char*) pInfo[8].pVar);

			m_DBSysDBSvrInfoItems.AddTail(pDBInfo) ;
		}
	}
return bExec ; 
}
int CgxDSKernel::LoadDBSys(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	if( LoadDBSysStruct(_pDBPool,_nDBType,_nDSSVRID) <0)
	{
		gxWriteLog("加载系统结构定义表失败！") ;
		return -2;
	}
	if( LoadDBSysMsg(_pDBPool,_nDBType,_nDSSVRID) <0)
	{		
		gxWriteLog("加载系统消息定义表失败！") ;
		return -3 ; 
	}
	if( LoadDBSysMsgMember(_pDBPool,_nDBType,_nDSSVRID) <0)
	{
		gxWriteLog("加载系统消息成员定义表失败！") ;
		return -4;
	}
return 0;
}
int CgxDSKernel::LoadDBSysMsg(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	CgxDBPlay Exec(_pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {		 
		{"DSMsgID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"MsgHeadID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"MsgHeadProto"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"MsgBodyProto"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"HeadLenInMsg"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"SuffixStartPos"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"IsLoginMsg"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"MsgName"					,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"SuffixCount"				,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"SuffixSumLen"				,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"SuffixStructID"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DSSVRID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if(  _nDSSVRID != -1)
		_snprintf(szWhere,sizeof(szWhere)," DSSVRID=%d order by  DSMsgID",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere)," 1=1 order by  DSMsgID");
	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_Msg") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysMsg *pMsg = new CgxDBSysMsg() ;
			if( pMsg==NULL)
				continue;
			pMsg->m_nDSMsgID					= *(int*) pInfo[0].pVar;
			pMsg->m_nMsgHeadID					= *(int*) pInfo[1].pVar;
			pMsg->m_nMsgHeadProto				= *(int*) pInfo[2].pVar;
			pMsg->m_nMsgBodyProto				= *(int*) pInfo[3].pVar;
			pMsg->m_nHeadLenInMsg				= *(int*) pInfo[4].pVar;
			pMsg->m_nSuffixStartPos				= *(int*) pInfo[5].pVar;
			pMsg->m_nIsLoginMsg					= *(int*) pInfo[6].pVar;
			strcpy(pMsg-> m_szMsgName			, (char*) pInfo[7].pVar);
			strcpy(pMsg-> m_szSuffixCount		, (char*) pInfo[8].pVar);
			strcpy(pMsg-> m_szSuffixSumLen		, (char*) pInfo[9].pVar);
			pMsg->m_nSuffixStructID				= *(int*) pInfo[10].pVar;
			pMsg->m_nDSSVRID					= *(int*) pInfo[11].pVar;
			m_DBSysMsgItems.InsertMsg(pMsg);
		}
	}
return bExec ; 
}
int CgxDSKernel::LoadDBSysStruct(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"DSSTID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"StructName"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"DSSVRID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if( _nDSSVRID != -1)
		_snprintf(szWhere,sizeof(szWhere)," DSSVRID = %d order by DSSTID",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere)," 1 = 1 order by DSSTID");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_Struct") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysStruct *pStruct = new CgxDBSysStruct() ;
			if( pStruct==NULL)
				continue;
			pStruct->m_nDSSTID				= *(int*) pInfo[0].pVar;
			strcpy(pStruct->m_szStructName	, (char*) pInfo[1].pVar);
			pStruct->m_nDSSVRID				= *(int*) pInfo[2].pVar;
			m_DBSysMsgItems.InsertStruct(pStruct) ;
		}
	}
return bExec ; 
}
int CgxDSKernel::LoadDBSysMsgMember(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"DSMsgMemberID"			,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"DSMsgID"					,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"MemberAttribute"			,GX_DBDATATYPE_STR 	,NULL,128	,1,0} ,
		{"MemberName"				,GX_DBDATATYPE_STR 	,NULL,128	,1,0} ,
		{"MemberClass"				,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"MemberType"				,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"MemberLen"				,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"MemberOrder"				,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"DefaultStr"				,GX_DBDATATYPE_STR 	,NULL,512	,1,0} ,
		{"HeadMemberType"			,GX_DBDATATYPE_INT 	,NULL,8		,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if( _nDSSVRID!= -1)
		_snprintf(szWhere,sizeof(szWhere),"DSSVRID = %d  order by MemberClass desc, MemberOrder ",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere),"1=1  order by MemberClass desc, MemberOrder ");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_MsgMember") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysMsgMember *pItem = new CgxDBSysMsgMember() ;
			if( pItem==NULL)
				continue;
			pItem->m_nDSMsgMemberID					= *(int*) pInfo[0].pVar;
			pItem->m_nDSMsgID						= *(int*) pInfo[1].pVar;
			strcpy(pItem->m_szMemberAttribute		, (char*) pInfo[2].pVar);
			strcpy(pItem->m_szMemberName			, (char*) pInfo[3].pVar);
			pItem->m_nMemberClass					= *(int*) pInfo[4].pVar;
			pItem->m_nMemberType					= *(int*) pInfo[5].pVar;
			pItem->m_nMemberLen						= *(int*) pInfo[6].pVar;
			pItem->m_nMemberOrder					= *(int*) pInfo[7].pVar;
			strcpy(pItem-> m_szDefaultStr			, (char*) pInfo[8].pVar);
			pItem->m_nHeadMemberType				= *(int*) pInfo[9].pVar;

			int ID =  *(int*) pInfo[1].pVar;
			if( pItem->m_nMemberClass ==1)
			{
				CgxDBSysStruct * pStruct = m_DBSysMsgItems.GetStructOfID(ID) ; 
				if( pStruct!=NULL) 
				{
					pStruct->AddMemberItem(pItem) ; 
				}
				else
				{
					gxWriteLog("加载时找不到相应的结构!%d" , ID) ;
				}
			}else
			{
				m_DBSysMsgItems.SetDBSysMsgMember(pItem) ;
				delete pItem ;
				pItem = NULL ;
			}
		}
	}
return bExec ; 
}
int CgxDSKernel::InitDBSvrConnect()
{
	CgxDBSysDBSvrInfo *pDBInfo = NULL;
	int Count = m_DBSysDBSvrInfoItems.GetCount() ; 
	for( int i=0; i< Count ;i ++ ) 
	{
		pDBInfo = m_DBSysDBSvrInfoItems.GetAt(i) ; 
		if( pDBInfo == NULL ) 
			continue; 
		if( pDBInfo->InitDB()<0)
		{
			gxWriteLog("初始化数据库%s,%s失败!数据库类型：%d",
				pDBInfo->m_szDSNName, 
				pDBInfo->m_szDBName,
				pDBInfo->m_nDBType) ;
			return -100-i;
		}
	}
return 0 ;
}
int CgxDSKernel::CompFuncDBSysDBSvrInfo(const void *_pItem1,const void *_pItem2)
{
	CgxDBSysDBSvrInfo * pItem1 = *(CgxDBSysDBSvrInfo **)_pItem1;
	CgxDBSysDBSvrInfo * pItem2 = *(CgxDBSysDBSvrInfo **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nDSDBSID - (__int64)pItem2->m_nDSDBSID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0 ;
}
CgxDBSysDBSvrInfo * CgxDSKernel::GetDBSvrInfo(int _ID)
{
	CgxDBSysDBSvrInfo Info ; 
	Info.m_nDSDBSID = _ID ;
return m_DBSysDBSvrInfoItems.Find(&Info,CompFuncDBSysDBSvrInfo) ;
}
int CgxDSKernel::OnMsg(DWORD _dwMsgID,string _Req,string &_Ack)
{
	if( m_bDebugFlag)
		gxWriteLog("消息处理开始！ ID =%8d[0x%08X]",_dwMsgID,_dwMsgID) ;

	CgxDBSysMsgPlay *pMsgPlay = m_DBSysMsgPlayItems.GetMsgPlayOfReqID(_dwMsgID) ; 
	if( pMsgPlay == NULL ) 
	{
		gxWriteLog("消息处理结束！未知的消息处理，请检查配置！ ID =%8d[0x%08X]",_dwMsgID,_dwMsgID) ;
		return -1; 
	}

	CgxDBSysMsg *pDBReqMsg = m_DBSysMsgItems.GetMsgOfID(_dwMsgID) ;
	CgxDBSysMsg *pDBAckMsg = m_DBSysMsgItems.GetMsgOfID(pMsgPlay->m_nAckID);
	if(( pDBReqMsg == NULL )  || (pDBAckMsg == NULL) )
	{
		gxWriteLog("消息处理结束！未配置请求消息(%d)或应答消息(%d)，请检查相应配置！",
			_dwMsgID,
			pMsgPlay->m_nAckID) ;
		return -1 ;
	}

	CgxMsgBufPlay ReqBuf ;
	CgxMsgBufPlay AckBuf ;
	ReqBuf.Init(_dwMsgID,pDBReqMsg,(char*)_Req.c_str() , _Req.length() ) ;
	AckBuf.Init(pMsgPlay->m_nAckID ,pDBAckMsg, NULL , 0 ) ;

	CgxDBSysDBSvrInfo *pDBSvrInfo = GetDBSvrInfo(pMsgPlay->m_nDSDBSID);
	if( pDBSvrInfo == NULL ) 
	{
		gxWriteLog("未知的数据库服务ID(%d)，请检查配置！ ID =%8d[0x%08X]",
			pMsgPlay->m_nDSDBSID,_dwMsgID,_dwMsgID) ;
		return -2;
	}
	if(m_bDebugFlag)
		ReqBuf.DebugInfo() ;
	int nRet = pMsgPlay->Play(pDBSvrInfo , ReqBuf , AckBuf) ;
	if( m_bDebugFlag)
		AckBuf.DebugInfo() ;
	if( AckBuf.GetDataLen() >0) 
		_Ack.append((char*)AckBuf.GetData(),AckBuf.GetDataLen() );
	if( m_bDebugFlag)
		gxWriteLog("消息处理结束！ ID =%8d[0x%08X] 结果%d",_dwMsgID,_dwMsgID,nRet) ;
return nRet ;
}
void CgxDSKernel::SetDebug(BOOL _bEnable)
{
	m_bDebugFlag = _bEnable;
	CgxMsgBufPlay::m_nDebugInfoFlag = _bEnable;
	CgxDBExecBase::SetDebug(_bEnable) ;
}
int CgxDSKernel::InitProtocol()
{
	int nRet = CgxMsgBufByte1::Init(&m_DBSysMsgItems) ;
	if( m_bDebugFlag ) 
		CgxMsgBufByte1::WriteByte1MsgListLog() ;
	int Count = m_SysInterfaceCfgItems.GetItemCount() ;
	CgxSysInterfaceCfg * pItem = NULL ;
	for(int i = 0 ;i < Count;i ++ ) 
	{
		pItem= m_SysInterfaceCfgItems.GetItemOfIdx(i) ; 
		if( pItem == NULL ) 
			continue; 
		if( pItem->m_pMsgBufBase == NULL ) 
			continue; 
		pItem->m_pMsgBufBase->InitProtocol(&m_DBSysMsgItems);
	}
return nRet ; 
}
int CgxDSKernel::InitDBSysMsgHeadLen()
{
	int Count = m_DBSysMsgItems.GetMsgCount() ; 
	CgxDBSysMsg * pDBMsg = NULL ;
	for( int i = 0 ;i <Count ;i ++ ) 
	{
		pDBMsg = m_DBSysMsgItems.GetMsg(i) ; 
		if( pDBMsg == NULL ) 
			continue ; 
		CgxMsgHeadInfo Info ;
		memset(&Info , 0 , sizeof(Info)) ;
		int MsgHeadID = pDBMsg->m_nMsgHeadID ;
		if(MsgHeadID <1)
			MsgHeadID = pDBMsg->m_nDSMsgID;
		CgxMsgBufPlay::GetMsgHeadInfo(Info,MsgHeadID ,pDBMsg->m_nMsgHeadProto);
		pDBMsg->m_nHeadLen = Info.m_dwHeadLen;
	}
return 0 ;
}
int CgxDSKernel::InitAckMsgInfo(CgxDBSysMsg &_HeadMsg,CgxDBSysMsgPlay& _MsgPlay)
{
	int StructCount = _HeadMsg.GetStructCount() ; 
	CgxDBSysStruct * pStruct = NULL  ;
	CgxDBSysMsgMember * pMember = NULL ;
	for( int i = 0 ;i < StructCount ;i++ ) 
	{
		pStruct = _HeadMsg.GetStruct(i) ;
		if( pStruct == NULL ) 
			continue ;
		int MemberCount = pStruct->GetMemberCount() ;
		for( int j = 0 ;j < MemberCount ;j ++ ) 
		{
			pMember = pStruct->GetMemberItem(j) ;
			if( pMember == NULL ) 
				continue ;
			switch(pMember->m_nHeadMemberType)
			{
			case 1:
				_MsgPlay.m_AckMsgIDMember[0] = i ;
				_MsgPlay.m_AckMsgIDMember[2] = j;
				_MsgPlay.m_AckMsgIDMember[3] = pMember->m_nMemberType;
				break;
			case 2:
				_MsgPlay.m_AckMsgLenMember[0] = i; 
				_MsgPlay.m_AckMsgLenMember[2] = j;
				_MsgPlay.m_AckMsgLenMember[3] =pMember->m_nMemberType;
				break;
			case 3:
				_MsgPlay.m_AckMsgSerialMember[0] = i;
				_MsgPlay.m_AckMsgSerialMember[2] = j;
				_MsgPlay.m_AckMsgSerialMember[3] = pMember->m_nMemberType;
				break;
			default:
				break;
			}
		}
	}
return 0 ; 
}
int CgxDSKernel::InitAckMsgInfo()
{
	int Count = m_DBSysMsgPlayItems.GetMsgPlayCount() ;
	CgxDBSysMsgPlay * pMsgPlay = NULL ;
	CgxDBSysMsg * pMsg = NULL ;
	CgxDBSysMsg * pHeadMsg = NULL ;
	for( int i =0 ;i < Count ;i ++ ) 
	{
		pMsgPlay = m_DBSysMsgPlayItems.GetMsgPlayOfIdx(i) ; 
		if( pMsgPlay == NULL ) 
			continue ;
		pMsg = m_DBSysMsgItems.GetMsgOfID(pMsgPlay->m_nAckID) ;
		if( pMsg == NULL ) 
			continue ;
		pHeadMsg = m_DBSysMsgItems.GetMsgOfID(pMsg->m_nMsgHeadID) ;
		if( pHeadMsg == NULL ) 
			continue; 
		InitAckMsgInfo(*pHeadMsg , *pMsgPlay) ;
	}
return 0 ; 
}
int CgxDSKernel::GetHeadLen(DWORD _dwProtoType,DWORD _dwMsgID) 
{
	CgxMsgBufPlay ReqBuf ;
	CgxMsgHeadInfo HeadInfo;
	memset(&HeadInfo , 0 , sizeof(HeadInfo) ) ;
	ReqBuf.GetMsgHeadInfo(HeadInfo,_dwMsgID,_dwProtoType) ;
return HeadInfo.m_dwHeadLen ; 
}
int CgxDSKernel::GetMsgLen(void *_pMsgHeadData,int _nDataLen,DWORD _dwProtoType,DWORD _dwMsgID) 
{
	if( _pMsgHeadData == NULL ) 
		return -1 ;
	CgxMsgBufPlay ReqBuf ;
	CgxMsgHeadInfo HeadInfo;
	memset(&HeadInfo,0,sizeof(HeadInfo) ) ;
	ReqBuf.GetMsgHeadInfo(HeadInfo,_dwMsgID,_dwProtoType) ;

	CgxDBSysMsg *pDBReqMsg = m_DBSysMsgItems.GetMsgOfID(_dwMsgID) ;
	ReqBuf.Init(_dwMsgID,pDBReqMsg,(char*)_pMsgHeadData , _nDataLen ) ;

	CgxDBSysMsgMember * pMember=  GetHeadMemberOfType(_dwMsgID,2) ;
	if( pMember == NULL ) 
		return -2 ;
	switch(pMember->m_nMemberType)
	{
	case  GX_MSGBUF_DATATYPE_SWORD:
	case  GX_MSGBUF_DATATYPE_WORD:
		return ReqBuf.GetShortData(0,-1,pMember->m_nMemberOrder) ;
		break;
	case GX_MSGBUF_DATATYPE_INT :
	case GX_MSGBUF_DATATYPE_UINT :
		return ReqBuf.GetIntData(0,-1,pMember->m_nMemberOrder) ;
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG :
		return (int)ReqBuf.GetInt64Data(0,-1,pMember->m_nMemberOrder) ;
		break;
	default:
		gxWriteLog("消息头关键成员不支持此类型! [%d](MsgID=%d)",
			pMember->m_nMemberType,
			_dwMsgID) ;
		break;
	}
return 0 ; 
}
int CgxDSKernel::GetMsgType(void *_pMsgHeadData ,int _nDataLen,DWORD _dwProtoType,DWORD _dwMsgID) 
{
	if( _pMsgHeadData == NULL ) 
		return -1 ;
	CgxMsgBufPlay ReqBuf ;
	CgxMsgHeadInfo HeadInfo;
	memset(&HeadInfo , 0 , sizeof(HeadInfo) ) ;
	ReqBuf.GetMsgHeadInfo(HeadInfo,_dwMsgID,_dwProtoType) ;

	CgxDBSysMsg *pDBReqMsg = m_DBSysMsgItems.GetMsgOfID(_dwMsgID) ;
	ReqBuf.Init(_dwMsgID,pDBReqMsg,(char*)_pMsgHeadData,_nDataLen ) ;

	CgxDBSysMsgMember * pMember=  GetHeadMemberOfType(_dwMsgID,1) ;
	if( pMember == NULL ) 
		return -2 ;
	switch(pMember->m_nMemberType)
	{
	case  GX_MSGBUF_DATATYPE_SWORD:
	case  GX_MSGBUF_DATATYPE_WORD:
		return ReqBuf.GetShortData(0,-1,pMember->m_nMemberOrder) ;
		break;
	case GX_MSGBUF_DATATYPE_INT :
	case GX_MSGBUF_DATATYPE_UINT :
		return ReqBuf.GetIntData(0,-1,pMember->m_nMemberOrder) ;
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG :
		return (int)ReqBuf.GetInt64Data(0,-1,pMember->m_nMemberOrder) ;
		break;
	default:
		gxWriteLog("消息头关键成员不支持此类型! [%d](MsgID=%d)",
			pMember->m_nMemberType,
			_dwMsgID) ;
		break;
	}
return 0 ; 
}
CgxDBSysMsgMember * CgxDSKernel::GetHeadMemberOfType(int _nMsgID,int _nHeadMemberType)
{
	CgxDBSysStruct * pStruct = m_DBSysMsgItems.GetStruct(0) ;
	if( pStruct == NULL ) 
		return NULL ;
	int Count = pStruct->GetMemberCount() ; 
	CgxDBSysMsgMember * pMember = NULL ;
	for( int i = 0 ;i < Count ;i ++ ) 
	{
		pMember = pStruct->GetMemberItem(i) ;
		if( pMember == NULL ) 
			continue; 
		if( pMember->m_nHeadMemberType == _nHeadMemberType ) 
			return pMember;
	}
return NULL ;
}

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
	gxWriteLog("          -----------ϵͳ��ʼ��-------------               ") ;
	int nRet = InitDB(_nDBType,_pDBName,_pDsnName,_pIP,_nPort,_nCnntCount,_pUser,_pPass);
	if( nRet <0) 
	{
		gxWriteLog("����ϵͳ���ó���[%d]" , nRet) ;
		return nRet;
	}

	nRet =InitDBSvrConnect();
	if( nRet <0)
	{
		gxWriteLog("��ʼ��Ӧ�����ݿ�����ʧ�ܣ�[%d]" , nRet) ;
		return nRet;
	}

	nRet = InitProtocol() ;
	if( nRet <0)
	{
		gxWriteLog("��ʼ��Э��ʧ�ܣ�[%d]" , nRet) ;
		return nRet;
	}

	nRet = InitDBSysMsgHeadLen();
	if( nRet <0)
	{
		gxWriteLog("��ʼ������Ϣͷ����ʧ�ܣ�[%d]" , nRet) ;
		return nRet;
	}
	nRet = InitAckMsgInfo();
	if( nRet <0)
	{
		gxWriteLog("��ʼ��Ӧ����Ϣ��Ϣʧ�ܣ�[%d]" , nRet) ;
		return nRet;
	}
	gxWriteLog("          -----------ϵͳ��ʼ���ɹ���-------------               ") ;
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
		gxWriteLog("��ʼ��ϵͳ���ݿ�����ʧ�ܣ�") ;
	else
		gxWriteLog("��ʼ��ϵͳ���ݿ����ӳɹ���");

	int nRet =LoadDBSys(m_SysDBInfo.GetDBPool() , m_SysDBInfo.m_nDBType) ;
	if( nRet >=0)
		gxWriteLog("���ض�ʯϵͳ���ñ�ɹ���") ;
	else
		gxWriteLog("���ض�ʯϵͳ���ñ�ʧ�ܣ�[%d]" , nRet) ;

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
		gxWriteLog("������Ϣ�����ʧ�ܣ� ");
		return -10;
	}
	else
		gxWriteLog("������Ϣ�����ɹ��� ����%d����",m_DBSysMsgItems.GetMsgCount());

	if( m_DBSysMsgPlayItems.LoadDBSys(_pDBPool,_nDBType,m_nDSSVRID) <0)
	{
		gxWriteLog("������Ϣ�����ʧ�ܣ� ");
		return -20;
	}else
		gxWriteLog("������Ϣ�����ɹ�������%d���� ",m_DBSysMsgPlayItems.GetMsgPlayCount());

	if( LoadDBSysDBSvrInfo(_pDBPool,_nDBType) <0)
	{
		gxWriteLog("�������ݿ������Ϣ��ʧ�ܣ� ");
		return -30;
	}else
		gxWriteLog("�������ݿ������Ϣ��ɹ��� ");

	gxWriteLog("��ʼ�������ݼ�Э��ӿ����ñ� ");
	if(m_SysInterfaceCfgItems.LoadSysInterfaceCfg(m_nDSSVRID,_pDBPool,_nDBType) <0)
	{
		gxWriteLog("�������ݼ�Э��ӿ����ñ�ʧ�ܣ� ");
		return -50;
	}else
		gxWriteLog("�������ݼ�Э��ӿ����ñ�ɹ��� ");
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
		gxWriteLog("����ϵͳ�ṹ�����ʧ�ܣ�") ;
		return -2;
	}
	if( LoadDBSysMsg(_pDBPool,_nDBType,_nDSSVRID) <0)
	{		
		gxWriteLog("����ϵͳ��Ϣ�����ʧ�ܣ�") ;
		return -3 ; 
	}
	if( LoadDBSysMsgMember(_pDBPool,_nDBType,_nDSSVRID) <0)
	{
		gxWriteLog("����ϵͳ��Ϣ��Ա�����ʧ�ܣ�") ;
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
					gxWriteLog("����ʱ�Ҳ�����Ӧ�Ľṹ!%d" , ID) ;
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
			gxWriteLog("��ʼ�����ݿ�%s,%sʧ��!���ݿ����ͣ�%d",
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
		gxWriteLog("��Ϣ����ʼ�� ID =%8d[0x%08X]",_dwMsgID,_dwMsgID) ;

	CgxDBSysMsgPlay *pMsgPlay = m_DBSysMsgPlayItems.GetMsgPlayOfReqID(_dwMsgID) ; 
	if( pMsgPlay == NULL ) 
	{
		gxWriteLog("��Ϣ���������δ֪����Ϣ�����������ã� ID =%8d[0x%08X]",_dwMsgID,_dwMsgID) ;
		return -1; 
	}

	CgxDBSysMsg *pDBReqMsg = m_DBSysMsgItems.GetMsgOfID(_dwMsgID) ;
	CgxDBSysMsg *pDBAckMsg = m_DBSysMsgItems.GetMsgOfID(pMsgPlay->m_nAckID);
	if(( pDBReqMsg == NULL )  || (pDBAckMsg == NULL) )
	{
		gxWriteLog("��Ϣ���������δ����������Ϣ(%d)��Ӧ����Ϣ(%d)��������Ӧ���ã�",
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
		gxWriteLog("δ֪�����ݿ����ID(%d)���������ã� ID =%8d[0x%08X]",
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
		gxWriteLog("��Ϣ��������� ID =%8d[0x%08X] ���%d",_dwMsgID,_dwMsgID,nRet) ;
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
		gxWriteLog("��Ϣͷ�ؼ���Ա��֧�ִ�����! [%d](MsgID=%d)",
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
		gxWriteLog("��Ϣͷ�ؼ���Ա��֧�ִ�����! [%d](MsgID=%d)",
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

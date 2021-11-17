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

#include "gxSysInterfaceCfg.h"
#include "gxDBPlay.h"
#include "gxMsgBufBase.h"
#include "gxFunc.h"
#pragma warning(disable:4996)

CgxSysInterfaceCfg::CgxSysInterfaceCfg(void)
{
	m_nInterfaceID=-1;
	memset(m_szIFName,0,sizeof(m_szIFName));
	memset(m_szIFPath,0,sizeof(m_szIFPath));
	m_nIsEnable=0;
	m_hDll = NULL ;
	m_FuncCreateDBLoad=NULL;
	m_FuncCreateDBExec=NULL;

	m_pDBLoadBase = NULL ;
	m_pDBExecBase = NULL ;
	m_pMsgBufBase = NULL ;
}

CgxSysInterfaceCfg::~CgxSysInterfaceCfg(void)
{
	if( m_hDll!=NULL)
		::FreeLibrary(m_hDll) ;
	m_hDll = NULL ;
	if( m_pDBLoadBase != NULL ) 
		delete m_pDBLoadBase ;
	if( m_pDBExecBase !=NULL ) 
		delete m_pDBExecBase;
	if( m_pMsgBufBase !=NULL ) 
		delete m_pMsgBufBase ;
	m_pDBLoadBase = NULL ;
	m_pDBExecBase = NULL ;
	m_pMsgBufBase = NULL ;
}

int CgxSysInterfaceCfg::LoadDll()
{
	if( strlen(m_szIFPath) <1) 
		return -1 ;
	char szPath[MAX_PATH];
	memset(szPath , 0 , sizeof(szPath)) ;
	if( strlen(m_szIFPath) < 2 )
		GetPathName(szPath) ;
	else
		strcpy(szPath , m_szIFPath ) ;

	strcat(szPath , m_szIFName) ;
	m_hDll = ::LoadLibrary(szPath) ;
	if( m_hDll == NULL ) 
	{
		gxWriteLog("���ؽӿ�Dllʧ�ܣ��Ҳ����ļ���ID[%d],[%s]",m_nInterfaceID,szPath) ;
		return -200 ;
	}
	if(( m_nInterfaceID > DS_DB_TYPE_NONE )&& (m_nInterfaceID <DS_DB_TYPE_MAX))
	{
		m_FuncCreateDBLoad =(DSCreateDBLoad) GetProcAddress(m_hDll,"DSCreateDBLoad") ;
		m_FuncCreateDBExec =(DSCreateDBExec) GetProcAddress(m_hDll,"DSCreateDBExec") ;
		if(( m_FuncCreateDBLoad == NULL)  || (m_FuncCreateDBExec == NULL))
		{			
			gxWriteLog("	�������ݽӿ�Dllʧ�ܣ�δ������Ӧ�ӿ�ID[%d],[%s]",m_nInterfaceID,szPath) ;
			return -2 ;
		}else
			gxWriteLog("	�������ݽӿ�Dll�ɹ���[%d],[%s]",m_nInterfaceID,szPath) ;
	}

	if((m_nInterfaceID >DS_MSG_TYPE_NONE) && (m_nInterfaceID <DS_MSG_TYPE_MAX))
	{
		m_FuncCreateMsgBuf =(DSCreateMsgBuf) GetProcAddress(m_hDll,"DSCreateMsgBuf") ;
		if( m_FuncCreateMsgBuf == NULL ) 
		{			
			gxWriteLog("	����Э��ӿ�Dllʧ�ܣ�δ������Ӧ�ӿ�ID[%d],[%s]",m_nInterfaceID,szPath) ;
			return -2 ;
		}else
			gxWriteLog("	����Э��ӿ�Dll�ɹ���[%d],[%s]",m_nInterfaceID,szPath) ;

		m_pMsgBufBase =(CgxMsgBufBase *) m_FuncCreateMsgBuf(m_nInterfaceID);
	}
return 0 ;
}

CgxSysInterfaceCfgList::CgxSysInterfaceCfgList(void)
{

}
CgxSysInterfaceCfgList::~CgxSysInterfaceCfgList(void)
{

}

int CgxSysInterfaceCfgList::LoadSysInterfaceCfg(int _nDSSVRID,void *_pDBPool,int _nDBType) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 

	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"InterfaceID"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"IFName"				,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"IFPath"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"IsEnable"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if( _nDSSVRID!= -1)
		_snprintf(szWhere,sizeof(szWhere)," DSSVRID = %d order by InterfaceID",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere)," IsEnable = 1 order by InterfaceID");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_SysInterfaceCfg") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxSysInterfaceCfg *pCfg = new CgxSysInterfaceCfg() ;
			if( pCfg==NULL)
				continue;

			pCfg->m_nInterfaceID				= *(int*) pInfo[0].pVar;
			strcpy(pCfg->m_szIFName				, (char*) pInfo[1].pVar);
			strcpy(pCfg->m_szIFPath				, (char*) pInfo[2].pVar);
			pCfg->m_nIsEnable					= *(int*) pInfo[3].pVar;
			m_SysIFItems.Insert(pCfg ,CompFuncSysIF) ;
			if( pCfg->LoadDll()< 0) 
			{
				return -100 ;
			}
		}
	}
return bExec ; 
}
CgxSysInterfaceCfg * CgxSysInterfaceCfgList::GetItemOfIdx(int _nIdx)
{
return m_SysIFItems.GetAt(_nIdx) ;	
}
int CgxSysInterfaceCfgList::GetItemCount()
{
return m_SysIFItems.GetCount() ;
}
int CgxSysInterfaceCfgList::CompFuncSysIF(const void *_pItem1, const void *_pItem2)
{
	CgxSysInterfaceCfg * pItem1 = *(CgxSysInterfaceCfg **)_pItem1;
	CgxSysInterfaceCfg * pItem2 = *(CgxSysInterfaceCfg **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nInterfaceID - (__int64)pItem2->m_nInterfaceID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0 ;
}
CgxSysInterfaceCfg * CgxSysInterfaceCfgList::FindItem(int _nID)
{
	CgxSysInterfaceCfg  * pItem = NULL ; 
	CgxSysInterfaceCfg FindItem;
	FindItem.m_nInterfaceID  = _nID ;
	pItem = m_SysIFItems.Find(&FindItem , CompFuncSysIF) ;
return pItem ;
}
CgxDBLoadBase * CgxSysInterfaceCfgList::CreateDBLoad(int _nID)
{
	CgxSysInterfaceCfg  * pItem = NULL ;
	pItem = FindItem(_nID) ;
	if( pItem == NULL ) 
		return NULL ; 
	CgxDBLoadBase * pDBLoad = NULL ;
	pDBLoad =(CgxDBLoadBase * ) (pItem->m_FuncCreateDBLoad)(_nID) ;
return pDBLoad;
}
CgxDBExecBase * CgxSysInterfaceCfgList::CreateDBExec(int _nID,void *_pPool) 
{
	CgxSysInterfaceCfg  * pItem = NULL ;
	pItem = FindItem(_nID) ;
	if( pItem == NULL ) 
		return NULL ; 
	CgxDBExecBase * pDBExec = NULL ;
	pDBExec =(CgxDBExecBase * ) (pItem->m_FuncCreateDBExec)(_nID,_pPool) ;
return pDBExec;
}
CgxMsgBufBase * CgxSysInterfaceCfgList::CreateMsgBuf(int _nID) 
{
	CgxSysInterfaceCfg  * pItem = NULL ;
	pItem = FindItem(_nID) ;
	if( pItem == NULL ) 
		return NULL ; 
	CgxMsgBufBase * pMsgBuf = NULL ;
	pMsgBuf =(CgxMsgBufBase * ) (pItem->m_FuncCreateMsgBuf)(_nID) ;
return pMsgBuf;
}

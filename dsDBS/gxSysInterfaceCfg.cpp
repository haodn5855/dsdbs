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
		gxWriteLog("加载接口Dll失败！找不到文件！ID[%d],[%s]",m_nInterfaceID,szPath) ;
		return -200 ;
	}
	if(( m_nInterfaceID > DS_DB_TYPE_NONE )&& (m_nInterfaceID <DS_DB_TYPE_MAX))
	{
		m_FuncCreateDBLoad =(DSCreateDBLoad) GetProcAddress(m_hDll,"DSCreateDBLoad") ;
		m_FuncCreateDBExec =(DSCreateDBExec) GetProcAddress(m_hDll,"DSCreateDBExec") ;
		if(( m_FuncCreateDBLoad == NULL)  || (m_FuncCreateDBExec == NULL))
		{			
			gxWriteLog("	加载数据接口Dll失败！未发现相应接口ID[%d],[%s]",m_nInterfaceID,szPath) ;
			return -2 ;
		}else
			gxWriteLog("	加载数据接口Dll成功！[%d],[%s]",m_nInterfaceID,szPath) ;
	}

	if((m_nInterfaceID >DS_MSG_TYPE_NONE) && (m_nInterfaceID <DS_MSG_TYPE_MAX))
	{
		m_FuncCreateMsgBuf =(DSCreateMsgBuf) GetProcAddress(m_hDll,"DSCreateMsgBuf") ;
		if( m_FuncCreateMsgBuf == NULL ) 
		{			
			gxWriteLog("	加载协议接口Dll失败！未发现相应接口ID[%d],[%s]",m_nInterfaceID,szPath) ;
			return -2 ;
		}else
			gxWriteLog("	加载协议接口Dll成功！[%d],[%s]",m_nInterfaceID,szPath) ;

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

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

#include "gxDBExecOdbc.h"

#pragma warning(disable:4996)
CgxDBLoadOdbc::CgxDBLoadOdbc(void)
{
	m_pPool = NULL ; 
	m_nPoolCount = 0 ; 
}

CgxDBLoadOdbc::~CgxDBLoadOdbc(void)
{
	if( m_pPool!=NULL) 
		delete []m_pPool ; 
	m_nPoolCount = 0 ; 
}
int CgxDBLoadOdbc::InitDB(char *_pDSN,char *_pDB,char*_pUserName,char*_pPass,int _nConnectCount,int _nPort)
{
	SetDBCount(1) ;
	return AddDB(0 , _pDSN,_pDB,_pUserName,_pPass,_nConnectCount,_nPort) ;
}
int CgxDBLoadOdbc::SetDBCount(int _nCount)
{
	if( m_pPool != NULL ) 
		return 2 ; 
	m_nPoolCount = _nCount;
	if( m_nPoolCount <1 )
		return -1 ; 
	m_pPool = new CgxDBPool[m_nPoolCount] ; 
return 0 ; 
}
int CgxDBLoadOdbc::AddDB(int _nIdx,char *_pDSN,char *_pDB,char*_pUserName,char*_pPass,int _nConnectCount,int _nPort)
{
	CgxDBPool * pPool =(CgxDBPool *) GetDBPool(_nIdx) ; 
	if( pPool == NULL ) 
		return -2 ; 

	char szDSN [128] ; 
	char szDB[128] ; 
	char szUserName[128];
	char szPassword[128];
	memset(szDSN,0,sizeof(szDSN)) ; 
	memset(szDB ,0,sizeof(szDB)) ;
	memset(szUserName,0,sizeof(szUserName));
	memset(szPassword,0,sizeof(szPassword)) ; 

	strcpy(szDSN,_pDSN)  ;
	strcpy(szDB,_pDB)  ;
	strcpy(szUserName,_pUserName)  ;
	strcpy(szPassword,_pPass)  ;

	if(! pPool->OnInitialUpdate(szDB,FALSE,szDSN,szUserName,szPassword,_nConnectCount) )
		return -6; 
	return 0 ; 
}

void * CgxDBLoadOdbc::GetDBPool(int _nIdx) 
{
	if(( _nIdx <0) || (_nIdx >=m_nPoolCount) )
		return NULL ;
	CgxDBPool * pPool = &m_pPool[_nIdx] ; 
return pPool ; 
}

CgxDBExecOdbc::CgxDBExecOdbc(CgxDBPool *_pPool,int _nIdx)
{
	m_pColInfo = NULL ; 
	m_nColCount	=0 ;
	m_pConnInfo = NULL ;
	m_nRecCount = 0 ;
	m_pColLen = NULL ;

	m_pPool = _pPool ; 
	m_nIdxFlag = _nIdx ;
}
CgxDBExecOdbc::CgxDBExecOdbc(CgxDBLoadBase *_pDBLoad,int _nIdx)
{
	m_pColInfo = NULL ; 
	m_nColCount	=0 ;
	m_pConnInfo = NULL ;
	m_nRecCount = 0 ;
	m_pColLen = NULL ;

	m_pPool = NULL ; 
	m_nIdxFlag = -1 ; 
	if( _pDBLoad!=NULL ) 
	{
		m_pPool = (CgxDBPool *)_pDBLoad->GetDBPool(_nIdx) ; 
		m_nIdxFlag = _nIdx ; 
	}
}
CgxDBExecOdbc::~CgxDBExecOdbc()
{
	FreeColInfo() ; 
	FreeConnect();
	m_pPool = NULL ; 
}

int CgxDBExecOdbc::ExeProc(char * _ProcName,DB_PARAM_INFO * _ParamInfo)
{
	if(( _ProcName == NULL )|| (_ParamInfo == NULL) || (m_pPool==NULL) 	)
		return -1 ;
	
	int Ret = m_pPool->ExecProc(_ProcName,_ParamInfo) ;
	if(Ret != SQL_SUCCESS ) 
	{
		gxWriteLog("过程: [%s] [%d] 出错" , _ProcName,m_nIdxFlag) ;
		return -2; 
	}
return 0 ; 
}
int	CgxDBExecOdbc::ExeProc(const char *_ProcName,PDB_PARAM_INFO _ParamInfo,PDBCOLINFO _pColInfo)
{
return 0 ;
}

int CgxDBExecOdbc::Exec(const char *pszStmt,__int64 *_pInserID)
{
	int Ret = 0 ;
	m_pConnInfo = m_pPool->GetConnect(__FILE__, __LINE__) ; 
	if( m_pConnInfo!=NULL)
	{
		Ret =  m_pPool->Exec(pszStmt ,m_pConnInfo ) ; 
		if( m_bDebugFlag )
			gxWriteLog("语句：%s 执行结果[%d]",pszStmt , Ret) ;
		if( Ret != SQL_SUCCESS)
			gxWriteLog("过程: [%s:%d] ,[%s]出错",__FILE__,__LINE__,pszStmt);
	}
	FreeConnect() ; 
return Ret ; 
}
int CgxDBExecOdbc::ExeSqlEx(char *_pTableName,DBTABLEINFO *_pTableInfo,char *_pWhere) 
{
	if(( _pTableName == NULL ) || (_pTableInfo == NULL))
		return -1 ; 
	if( m_pPool== NULL ) 
		return -2 ; 
	int Size = 0 ; 
	char SqlBuf [2048] ; 
	memset(SqlBuf , 0 , sizeof(SqlBuf)) ;
	strcpy(SqlBuf , " select "); 
	for(;strlen(_pTableInfo[Size].ColName)>0;Size++ ) 
	{
		strcat(SqlBuf , _pTableInfo[Size].ColName) ; 
		if( strlen(_pTableInfo[Size+1].ColName)>0)
			strcat(SqlBuf , ",")  ;
	}
	FreeColInfo() ;
	FreeConnect() ; 
	m_pColInfo = new DBCOLINFO[Size+1] ; 
	m_pColLen = new int[Size+1];
	memset(m_pColLen ,0,sizeof(int) * (Size+1));
	for( int i = 0 ;i < Size ;i ++ ) 
	{
		m_pColInfo[i].nDataType		= _pTableInfo[i].Type ; 
		if(_pTableInfo[i].Size>0)
			m_pColInfo[i].pVar		= new char[_pTableInfo[i].Size] ; 
		else
			m_pColInfo[i].pVar		= new char[1] ; 
		m_pColInfo[i].nVarLen		= _pTableInfo[i].Size ; 
		_pTableInfo[i].Data			= (char*)m_pColInfo[i].pVar  ;
		m_pColLen[i]				= _pTableInfo[i].Size ; 
	}
	m_pColInfo[Size].nDataType		= 0 ; 
	m_pColInfo[Size].pVar			= NULL ; 
	m_pColInfo[Size].nVarLen		= 0 ; 
	m_nColCount = Size ; 

	strcat(SqlBuf , " from " ) ;
	strcat(SqlBuf , _pTableName) ; 

	if( _pWhere != NULL)
	{
		strcat(SqlBuf , " where " ) ; 
		strcat(SqlBuf , _pWhere) ; 
	}
	m_nRecCount=0;
	m_pConnInfo = m_pPool->GetConnect(__FILE__, __LINE__) ; 
	if( m_pConnInfo!=NULL)
	{
		int Ret = m_pPool->ExecSelect(SqlBuf ,m_pColInfo,m_pConnInfo ) ; 
		if( m_bDebugFlag )
			gxWriteLog("语句：%s 执行结果[%d]",SqlBuf , Ret) ;
		if( Ret == SQL_NO_DATA ) 
		{
			FreeColInfo() ; 
			FreeConnect() ; 
			return -1 ;
		}
		if( Ret != SQL_SUCCESS && Ret!= SQL_SUCCESS_WITH_INFO) 
		{
			FreeColInfo() ; 
			FreeConnect(1,SqlBuf) ; 
			return -3 ;
		}
		if( m_pColInfo !=NULL ) 
		{
			return 1 ; 
		}
	}else
		return -3 ;
return 0 ; 
}
int CgxDBExecOdbc::FreeColInfo()
{
	if( m_pColInfo!= NULL ) 
	{
		for(int i = 0;i < m_nColCount ;i ++ ) 
		{
			if( m_pColInfo[i].pVar!= NULL ) 
			{
				delete [](char*)(m_pColInfo[i].pVar ); 
				m_pColInfo[i].pVar = NULL ; 
			}
		}
	}
	if( m_pColInfo!= NULL ) 
	{
		delete []m_pColInfo ;
		m_pColInfo =NULL; 
	}
	if( m_pColLen!=NULL)
	{
		delete []m_pColLen ; 
		m_pColLen = NULL ; 
	}
	m_nColCount = 0 ; 
return 0 ; 
}
int CgxDBExecOdbc::FreeConnect(int _WriteErrorFlag,char *_pszSmt)
{
	if( m_pPool == NULL ) 
		return 0 ; 
	if( m_pConnInfo!=NULL ) 
	{
		if((_WriteErrorFlag) && (_pszSmt!=NULL))
			m_pPool->WriteError( m_pConnInfo,__FILE__, __LINE__ , _pszSmt);
		m_pPool->FreeConnect(m_pConnInfo) ; 
		m_pConnInfo = NULL ;
	}
return 0 ; 
}
void CgxDBExecOdbc::ClearSqlEx()
{
	FreeColInfo() ; 
	FreeConnect() ; 
}
DBCOLINFO* CgxDBExecOdbc::GetNextRecord()
{
	if( m_pPool == NULL ) 
		return NULL ; 
	int Ret = 0 ;
	if( m_pColInfo !=NULL ) 
	{
		for( int i = 0 ;i < m_nColCount ;i ++ ) 
		{
			memset(m_pColInfo[i].pVar , 0 , sizeof(char) *m_pColLen[i]) ; 
		}
		Ret = m_pPool->FetchData(m_pConnInfo) ;
		if( Ret == SQL_NO_DATA)
		{
			FreeColInfo() ; 
			FreeConnect() ; 
			return NULL ; 
		}
		if( Ret != SQL_SUCCESS && Ret!= SQL_SUCCESS_WITH_INFO) 
		{
			FreeColInfo() ; 
			FreeConnect(1,"GetNextRecord") ; 
			return NULL ; 
		}
		m_nRecCount++ ;
		return m_pColInfo ; 
	}
return NULL ; 
}
int CgxDBExecOdbc::GetNext()
{
	if( m_pPool == NULL ) 
		return NULL ; 
	int Ret = 0 ;
	if( m_pConnInfo== NULL)
	{
		return -2 ;
	}
	Ret =m_pPool->FetchData( m_pConnInfo) ; 
	if( Ret == SQL_NO_DATA)
	{
		return 0 ; 
	}
	if( Ret != SQL_SUCCESS && Ret!= SQL_SUCCESS_WITH_INFO) 
	{
		m_pPool->WriteError(m_pConnInfo,__FILE__,__LINE__,"GetNext");
		return -1 ; 
	}
	return 1 ; 
}

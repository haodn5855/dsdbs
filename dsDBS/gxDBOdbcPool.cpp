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

#include "gxFunc.h"
#include "gxDBOdbcPool.h"

using namespace zsGame;

#define BufferSize 2048

void GetError(PSQLCONNECTINFO pSQLConnect,int _Type =SQL_HANDLE_STMT);
void DisplayError(SQLCHAR* SqlState,SQLINTEGER NativeError,SQLCHAR* Msg,SQLSMALLINT MsgLen);

CgxDBPool::CgxDBPool()
{
	memset(m_szDSN,0,sizeof(m_szDSN));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_szPassword,0,sizeof(m_szPassword));
	memset(m_szDBName,0,sizeof(m_szDBName));
	m_cnGetForReconnect = 0;
	m_cnConnect = 0;
	m_pConnectList = NULL;
	m_hSemaphore = NULL;
	m_cnWaitingForConnect = 0;
	m_cnUsingConnect = 0;
	m_nLastConnect = 0;
}
CgxDBPool::~CgxDBPool()
{
	for(int i=0;i<m_nLastConnect;i++)
	{
		FreeSQLHandle(&m_pConnectList[i]);
	}
	if(m_pConnectList!=NULL)
	{
		delete[] m_pConnectList;
		m_pConnectList = NULL ; 
		m_cnConnect = 0 ;
	}
	if( m_hSemaphore!=NULL) 
	{
		CloseHandle(m_hSemaphore) ; 
		m_hSemaphore = NULL ;
	}
}
BOOL CgxDBPool::OnInitialUpdate(BOOL bOCI,BOOL bConnect,int cnGetForReconnect)
{
return	OnInitialUpdate(m_szDBName,bOCI,m_szDSN,m_szUserName,m_szPassword,m_cnConnect,bConnect,cnGetForReconnect) ;
}

BOOL CgxDBPool::OnInitialUpdate(const char *pszDBName,BOOL bOCI,const char *pszDSN,
								const char *pszUserName,const char *pszPassword,int cnConnect,BOOL bConnect,int cnGetForReconnect)
{
	if(pszDBName == NULL || pszDSN == NULL || pszUserName == NULL || pszPassword == NULL)
	{
		return FALSE;
	}
	strcpy_s(m_szDBName,pszDBName);
	strcpy_s(m_szDSN,pszDSN);
	strcpy_s(m_szUserName,pszUserName);
	strcpy_s(m_szPassword,pszPassword);
	m_cnConnect = cnConnect; 

	m_pConnectList = new SQLCONNECTINFO[m_cnConnect];
	memset(m_pConnectList , 0,sizeof(SQLCONNECTINFO) * m_cnConnect) ;

	if( m_hSemaphore==NULL)
		m_hSemaphore = CreateSemaphore(NULL,m_cnConnect,m_cnConnect,NULL);

	m_cnGetForReconnect = cnGetForReconnect;

	if(bConnect)
	{
		m_pConnectList[m_nLastConnect].nLineNo = m_nLastConnect;
		if(Connect(&m_pConnectList[m_nLastConnect]))
			return TRUE;
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CgxDBPool::Connect(PSQLCONNECTINFO pSQLConnect,int cnRetry)
{
	SQLHENV hEnv;
	SQLRETURN sr;

	int Retryed = 0;
	while (!SQL_SUCCEEDED(sr=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv)) && Retryed++ < cnRetry)
	{
	} 
	if(!SQL_SUCCEEDED(sr))
	{
		return FALSE;
	}
	Retryed = 0;
	while (!SQL_SUCCEEDED(sr=SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0)) && Retryed++ < cnRetry){} 
	if(!SQL_SUCCEEDED(sr))
	{ 
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}
	Retryed = 0;
	while (!SQL_SUCCEEDED(sr=SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&pSQLConnect->hDbc)) && Retryed++ < cnRetry){}
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect ,SQL_HANDLE_DBC) ;
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}

	Retryed = 0;
	SQLPOINTER rgbValue;
	int i = 0;
	rgbValue = &i;
	sr = SQLSetConnectAttr(pSQLConnect->hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(rgbValue), 0);
	if(!SQL_SUCCEEDED(sr))
	{ 
		GetError(pSQLConnect ,SQL_HANDLE_DBC) ;
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}
	if(!pSQLConnect->bAutoCommit)
	{
				while (!SQL_SUCCEEDED(sr=SQLSetConnectAttr(pSQLConnect->hDbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_OFF,SQL_IS_POINTER)) && Retryed++ < cnRetry)
		{
		}
	}
	if(!SQL_SUCCEEDED(sr))
	{ 
		GetError(pSQLConnect,SQL_HANDLE_DBC) ;
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}
		long Timeout = 30;
	sr=SQLSetConnectAttr( pSQLConnect->hDbc,SQL_ATTR_LOGIN_TIMEOUT,(SQLPOINTER)Timeout,0/*sizeof(Timeout)*/) ;
	if(!SQL_SUCCEEDED(sr))
	{ 
		GetError(pSQLConnect,SQL_HANDLE_DBC) ;
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}

	Retryed = 0;
	while (!SQL_SUCCEEDED(sr=SQLConnect(pSQLConnect->hDbc,(SQLCHAR*)m_szDSN,SQL_NTS,(SQLCHAR*) m_szUserName,SQL_NTS,(SQLCHAR*) m_szPassword,SQL_NTS)) && Retryed++ < cnRetry)
	{
	}
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect,SQL_HANDLE_DBC) ;
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}

	Retryed = 0;
	while (!SQL_SUCCEEDED(sr=SQLAllocHandle(SQL_HANDLE_STMT,pSQLConnect->hDbc,&pSQLConnect->hStmt)) && Retryed++ < cnRetry){}
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect) ;
		SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
		return FALSE;
	}
	sr = SQLSetStmtAttr(pSQLConnect->hStmt,SQL_ATTR_CURSOR_TYPE,SQL_CURSOR_FORWARD_ONLY,SQL_NTS);
	pSQLConnect->bSelect =FALSE;
	pSQLConnect->bUsing = FALSE;
	pSQLConnect->cnGet = 0;
	pSQLConnect->bAutoCommit = TRUE;

	SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
	return TRUE;
}

PSQLCONNECTINFO CgxDBPool::GetConnect(const char *pszFile,int nLineNo)
{
	DWORD  dwSemaphore = WaitForSingleObject(m_hSemaphore,INFINITE);
	if( dwSemaphore == WAIT_FAILED ) 
		return NULL ;
	if( dwSemaphore!= WAIT_OBJECT_0)
		return NULL ;
	m_cnWaitingForConnect++;

		m_cs.Lock();
	for(int i=0;i<=m_nLastConnect;i++)
	{
		if(!m_pConnectList[i].bUsing) 
		{
			m_cnWaitingForConnect--;
			m_cnUsingConnect++;
			if(m_cnGetForReconnect!=0 && m_pConnectList[i].cnGet>m_cnGetForReconnect) 				this->Connect(&m_pConnectList[i]);
			m_pConnectList[i].bUsing = TRUE;
			m_pConnectList[i].cnGet++;
			m_cs.Unlock();
			return &m_pConnectList[i];
		}
	}
		if(m_nLastConnect<m_cnConnect-1)
	{
		m_nLastConnect++;
		m_pConnectList[m_nLastConnect].nLineNo = m_nLastConnect;
		if(Connect(&m_pConnectList[m_nLastConnect]))
		{
			m_cnWaitingForConnect--;
			m_cnUsingConnect++;
			m_pConnectList[m_nLastConnect].bUsing = TRUE;
			m_pConnectList[m_nLastConnect].cnGet++;
			m_cs.Unlock();
			return &m_pConnectList[m_nLastConnect];
		}
	}
	m_cs.Unlock();
	return NULL;
}
void CgxDBPool::FreeConnect(PSQLCONNECTINFO pSQLConnect)
{
	pSQLConnect->bUsing = FALSE;
	m_cnUsingConnect--;
	if(pSQLConnect->bSelect)
	{
		SQLCloseCursor(pSQLConnect->hStmt);
		ClearMoreResults(pSQLConnect);
	}
	ReleaseSemaphore(m_hSemaphore,1,NULL);
}

int CgxDBPool::Exec(const char *pszStmt,PSQLCONNECTINFO pSQLConnect,BOOL bNeedPrepare)
{
	if(NULL == pszStmt || NULL == pSQLConnect) 
	{
		return !SQL_SUCCESS;
	}

	if(pSQLConnect->bSelect) 
	{
		SQLCloseCursor(pSQLConnect->hStmt);
		pSQLConnect->bSelect = FALSE;
	}
	SQLRETURN sr;
	if(bNeedPrepare)	{
		sr = SQLPrepare(pSQLConnect->hStmt,(SQLCHAR*)pszStmt,SQL_NTS);
		if(!SQL_SUCCEEDED(sr))
		{
			GetError(pSQLConnect);
			return sr;
		}
		sr = SQLExecute(pSQLConnect->hStmt);
		if(!SQL_SUCCEEDED(sr))
		{
			GetError(pSQLConnect);
			return sr;
		}
	}
	else	{
		sr = SQLExecDirect(pSQLConnect->hStmt,(SQLCHAR*)pszStmt,SQL_NTS);
		if(!SQL_SUCCEEDED(sr))
		{
			GetError(pSQLConnect);
			return sr;
		}
		Commit(pSQLConnect) ; 
	}
	return SQL_SUCCESS;
}

void CgxDBPool::FreeSQLHandle(PSQLCONNECTINFO pSQLConnect)
{
	if(pSQLConnect->hStmt!=NULL)
		SQLFreeHandle(SQL_HANDLE_STMT,pSQLConnect->hStmt);
	if(pSQLConnect->hDbc!=NULL)
		SQLFreeHandle(SQL_HANDLE_DBC,pSQLConnect->hDbc);
}

BOOL CgxDBPool::Commit(PSQLCONNECTINFO pSQLConnect)
{
	SQLRETURN sr;
	sr = SQLEndTran(SQL_HANDLE_DBC,pSQLConnect->hDbc,SQL_COMMIT);
	if(!SQL_SUCCEEDED(sr))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CgxDBPool::Rollback(PSQLCONNECTINFO pSQLConnect)
{
	SQLRETURN sr;
	sr = SQLEndTran(SQL_HANDLE_DBC,pSQLConnect->hDbc,SQL_ROLLBACK);
	if(!SQL_SUCCEEDED(sr))
	{
		return FALSE;
	}
	return TRUE;
}
int CgxDBPool::ExecSelect(const char *pszStmt,PDBCOLINFO pci,PSQLCONNECTINFO pSQLConnect)
{
	if(NULL == pszStmt || NULL == pSQLConnect || NULL == pci) 
	{
		return !SQL_SUCCESS;
	}
						SQLRETURN sr;
	sr = SQLExecDirect(pSQLConnect->hStmt,(SQLCHAR*)pszStmt,SQL_NTS);
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect) ;
		return sr;
	}
	pSQLConnect->bSelect = TRUE;
	SQLSMALLINT CoumnCount = 0;
	sr = SQLNumResultCols(pSQLConnect->hStmt,&CoumnCount);
	if(!SQL_SUCCEEDED(sr))
	{
		return sr;
	}
	if(CoumnCount<1)
	{
		return SQL_NO_DATA;
	}
	for(int i = 0;i < CoumnCount;i++)
	{
		sr = BindParam(pSQLConnect, i+1, &pci[i]);
		if(!SQL_SUCCEEDED(sr))
		{
			return sr;
		}
	}
	return SQL_SUCCESS;
}
void CgxDBPool::ClearMoreResults(PSQLCONNECTINFO _pSQLConnect)
{
	while (SQLMoreResults(_pSQLConnect->hStmt) != SQL_NO_DATA);
}
int CgxDBPool::FetchData(PSQLCONNECTINFO pSQLConnect)
{
	SQLRETURN sr;

	sr = SQLFetch(pSQLConnect->hStmt) ;
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect);
		return sr;
	}
	return SQL_SUCCESS;
}
int CgxDBPool::ExecFunc(const char *pszFunc,PDB_PARAM_INFO ppi,PSQLCONNECTINFO pSQLConnect)
{
	return 0 ; 
}
int CgxDBPool::ExecProc(const char *pszProc,PDB_PARAM_INFO ppi,PSQLCONNECTINFO pSQLConnect)
{
	if(NULL == pszProc || NULL == pSQLConnect || NULL == ppi) 
	{
		return !SQL_SUCCESS;
	}

	SQLRETURN sr;
	char SqlBuf [BufferSize]; 

	memset(SqlBuf,0,sizeof(SqlBuf)) ;
	strcpy_s(SqlBuf," {call "); 
	strcat_s(SqlBuf,pszProc);
	strcat_s(SqlBuf,"(");
	int ParamCount = 0;
	while(ppi[ParamCount].nIOType != NULL)
	{	
		if(ParamCount > 0)
			strcat_s(SqlBuf,",?");
		else if(0 == ParamCount)
			strcat_s(SqlBuf,"?");

		sr = BindParam(pSQLConnect, ParamCount+1 , &ppi[ParamCount]);
		if(!SQL_SUCCEEDED(sr))
		{
			return sr;
		}
		ParamCount++;
	}
	strcat_s(SqlBuf,")}");

	if(pSQLConnect->bSelect)
	{
		SQLCloseCursor(pSQLConnect->hStmt);
		pSQLConnect->bSelect = FALSE;
	}
	sr = SQLExecDirect(pSQLConnect->hStmt,(SQLCHAR*)SqlBuf,SQL_NTS);
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect);
		return sr;
	}
	sr = SQLMoreResults(pSQLConnect->hStmt) ;
	if(( sr!=SQL_SUCCESS )&& (sr!=SQL_NO_DATA) )
	{
		GetError(pSQLConnect);
		return sr;
	}
	return SQL_SUCCESS;
}
int CgxDBPool::BindParam(PSQLCONNECTINFO pSQLConnect,int nPos,PDB_PARAM_INFO ppi)
{
	int C_Type,SQL_Type,IO_Type;
	SQLRETURN sr;
	switch(ppi->nIOType)
	{
	case GX_DBIOTYPE_CONSTANT:
		IO_Type = SQL_PARAM_TYPE_UNKNOWN;
		break;
	case GX_DBIOTYPE_INPUT:
		IO_Type = SQL_PARAM_INPUT;
		break;
	case GX_DBIOTYPE_OUTPUT:
		IO_Type = SQL_PARAM_OUTPUT;
		break;
	case GX_DBIOTYPE_INPUTOUTPUT:
		IO_Type = SQL_PARAM_INPUT_OUTPUT;
		break;
	}

	switch(ppi->nDataType)
	{
	case GX_DBDATATYPE_CHAR:
		C_Type   = SQL_C_CHAR;
		SQL_Type = SQL_CHAR;
		break;
	case GX_DBDATATYPE_BYTE:
		C_Type   = SQL_C_BIT;
		SQL_Type = SQL_CHAR;
		break;
	case GX_DBDATATYPE_SWORD:
		C_Type   = SQL_C_SSHORT;
		SQL_Type = SQL_INTEGER;
		break;
	case GX_DBDATATYPE_WORD:
		C_Type   = SQL_C_USHORT;
		SQL_Type = SQL_INTEGER;
		break;
	case GX_DBDATATYPE_INT:
		C_Type   = SQL_C_LONG;
		SQL_Type = SQL_INTEGER;
		break;
	case GX_DBDATATYPE_UINT:
		C_Type   = SQL_C_ULONG;
		SQL_Type = SQL_INTEGER;
		break;
	case GX_DBDATATYPE_STR:
		C_Type   = SQL_C_CHAR;
		SQL_Type = SQL_VARCHAR;
		break;
	case GX_DBDATATYPE_RAWSTR:
		C_Type   = SQL_C_CHAR;
		SQL_Type = SQL_VARCHAR;
		break;
	case GX_DBDATATYPE_BINARY:
		C_Type   = SQL_C_BINARY;
		SQL_Type = SQL_BINARY;
		break;
	case GX_DBDATATYPE_LONGLONG:
		C_Type   = SQL_C_SBIGINT;
		SQL_Type = SQL_BIGINT;
		break;
	case GX_DBDATATYPE_FLOAT:
		C_Type   = SQL_C_FLOAT;
		SQL_Type = SQL_FLOAT;
		break;
	case GX_DBDATATYPE_DOUBLE:
		C_Type   = SQL_C_DOUBLE;
		SQL_Type = SQL_DOUBLE;
		break;
	case GX_DBDATATYPE_DATE:
		C_Type   = SQL_C_TYPE_DATE;
		SQL_Type = SQL_TIMESTAMP;
		break;
	case GX_DBDATATYPE_DECIMAL:
		C_Type   = SQL_C_CHAR;;
		SQL_Type = SQL_DECIMAL;
		break;
	}
	SQLINTEGER cbValue = SQL_NTS;

	sr=SQLBindParameter(pSQLConnect->hStmt,
		nPos,
		IO_Type,
		C_Type,
		SQL_Type,
		ppi->nVarLen,
		ppi->nDecimal_Digits,
		ppi->pVar,
		ppi->nVarLen,
		&ppi->nVarLen);
	if(!SQL_SUCCEEDED(sr))
	{
		GetError(pSQLConnect);
		return sr;
	}

	return SQL_SUCCESS;
}
int CgxDBPool::BindParam(PSQLCONNECTINFO pSQLConnect,int nPos,PDBCOLINFO pci)
{
	int C_Type;
	SQLRETURN sr;

	switch(pci->nDataType)
	{
	case GX_DBDATATYPE_CHAR:
		C_Type   = SQL_C_CHAR;
		break;
	case GX_DBDATATYPE_BYTE:
		C_Type   = SQL_C_BIT;
		break;
	case GX_DBDATATYPE_SWORD:
		C_Type   = SQL_C_SSHORT;
		break;
	case GX_DBDATATYPE_WORD:
		C_Type   = SQL_C_USHORT;
		break;
	case GX_DBDATATYPE_INT:
		C_Type   = SQL_C_LONG;
		break;
	case GX_DBDATATYPE_UINT:
		C_Type   = SQL_C_ULONG;
		break;
	case GX_DBDATATYPE_STR:
		C_Type   = SQL_C_CHAR;
		break;
	case GX_DBDATATYPE_RAWSTR:
		C_Type   = SQL_C_CHAR;
		break;
	case GX_DBDATATYPE_BINARY:
		C_Type   = SQL_C_BINARY;
		break;
	case GX_DBDATATYPE_LONGLONG:
		C_Type   = SQL_C_UBIGINT;
		break;
	case GX_DBDATATYPE_FLOAT:
		C_Type   = SQL_C_FLOAT;
		break;
	case GX_DBDATATYPE_DOUBLE:
		C_Type   = SQL_C_DOUBLE;
		break;
	case GX_DBDATATYPE_DATE:
		C_Type   = SQL_C_TYPE_DATE;
		break;
	}
	sr = SQLBindCol(pSQLConnect->hStmt,nPos,C_Type,pci->pVar,pci->nVarLen,&pci->nRetLen);
	if(!SQL_SUCCEEDED(sr))
	{
		return sr;
	}
	return SQL_SUCCESS;
}
BOOL CgxDBPool::SetAutoCommit(PSQLCONNECTINFO pSQLConnect,BOOL value)
{
	SQLPOINTER IsAuto;
	if(value)
	{
		IsAuto = (SQLPOINTER)SQL_AUTOCOMMIT_ON;
	}
	else
	{
		IsAuto = (SQLPOINTER)SQL_AUTOCOMMIT_OFF;
	}
	SQLRETURN sr;
	sr = SQLSetConnectAttr(pSQLConnect->hDbc,SQL_ATTR_AUTOCOMMIT,IsAuto,SQL_IS_POINTER);
	pSQLConnect->bAutoCommit = value;
	if(!SQL_SUCCEEDED(sr))
	{ 
		return FALSE;
	}
	return TRUE;
}
void CgxDBPool::WriteError(PSQLCONNECTINFO pSQLConnect,const char *pszFile,int nLine,const char *pszStmt)
{
		GetError(pSQLConnect);
}

void GetError(PSQLCONNECTINFO pSQLConnect ,int _Type)
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError=0;
	SQLSMALLINT   i=0, MsgLen=0;
	SQLRETURN     rc;
	memset(SqlState, 0,sizeof(SqlState)) ; 
	memset(Msg ,0,sizeof(Msg)) ;
		i = 1;
	if( _Type == SQL_HANDLE_DBC)
	{
		while ((rc = SQLGetDiagRec(SQL_HANDLE_DBC, pSQLConnect->hDbc, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA) {
				DisplayError(SqlState,NativeError,Msg,MsgLen);
				i++;
		}
	}else
	{
		while ((rc = SQLGetDiagRec(SQL_HANDLE_STMT, pSQLConnect->hStmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA) {
				DisplayError(SqlState,NativeError,Msg,MsgLen);
				i++;
		}
	}
	return;
}
void DisplayError(SQLCHAR* SqlState,SQLINTEGER NativeError,SQLCHAR* Msg,SQLSMALLINT MsgLen)
{
	gxWriteLog("State %s Error:%d  Sql:%s  SqlLen:%d" , 
		SqlState  ,NativeError,Msg , MsgLen) ;
}


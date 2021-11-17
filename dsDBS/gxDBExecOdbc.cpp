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
		gxWriteLog("����: [%s] [%d] ����" , _ProcName,m_nIdxFlag) ;
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
			gxWriteLog("��䣺%s ִ�н��[%d]",pszStmt , Ret) ;
		if( Ret != SQL_SUCCESS)
			gxWriteLog("����: [%s:%d] ,[%s]����",__FILE__,__LINE__,pszStmt);
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
			gxWriteLog("��䣺%s ִ�н��[%d]",SqlBuf , Ret) ;
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

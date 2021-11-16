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

#include <gxFunc.h>
#include <gxBaseDll.h>
#include <gxBase.h> 
#include <gxDBBaseDll.h>
#include <sql.h>
#include <sqlext.h>
#pragma once
#ifndef GX_DB_EXEC_BASE_2021_01_01
#define GX_DB_EXEC_BASE_2021_01_01

#define		DS_DB_TYPE_NONE					10000
#define		DS_DB_TYPE_ODBC					10001
#define		DS_DB_TYPE_MYSQL				10002
#define		DS_DB_TYPE_SQLSERVER			10003
#define		DS_DB_TYPE_ORACLE				10004

#define		DS_DB_TYPE_REDIS				10101
#define		DS_DB_TYPE_CASS					10201

#define		DS_DB_TYPE_MAX					19999


#define		GX_DBIOTYPE_CONSTANT			0														
#define		GX_DBIOTYPE_INPUT				1														
#define		GX_DBIOTYPE_OUTPUT				2														
#define		GX_DBIOTYPE_INPUTOUTPUT			3														

#define		GX_DBDATATYPE_CHAR				0														
#define		GX_DBDATATYPE_BYTE				1														
#define		GX_DBDATATYPE_SWORD				2														
#define		GX_DBDATATYPE_WORD				3														
#define		GX_DBDATATYPE_INT				4														
#define		GX_DBDATATYPE_UINT				5														
#define		GX_DBDATATYPE_STR				6														
#define		GX_DBDATATYPE_RAWSTR			7														
#define		GX_DBDATATYPE_BINARY			8														
#define		GX_DBDATATYPE_LONGLONG			9														
#define		GX_DBDATATYPE_FLOAT				10														
#define		GX_DBDATATYPE_DATE				11														
#define		GX_DBDATATYPE_DECIMAL			12														
#define		GX_DBDATATYPE_DOUBLE			15														

typedef struct tagSolConnectInfo
{
	BOOL bUsing;
	BOOL bSelect;																		
	int cnGet;																			
	int cnError;
	const char *pszFile;
	int nLineNo;
	SQLHDBC hDbc;
	SQLHSTMT hStmt;
	BOOL bAutoCommit;																	
}SQLCONNECTINFO,*PSQLCONNECTINFO;

typedef struct tagDBParamInfo
{
	int nIOType;																		
	int nDataType;																		
	void *pVar;																			
	long nVarLen;																		
	int nDecimal_Digits;																
	int nIsNull;																		
}DB_PARAM_INFO,*PDB_PARAM_INFO;


typedef struct tagDBColInfo
{
	int nDataType;																		
	void *pVar;																			
	int nVarLen;																		
	long nRetLen;																		
	int nIsNull;																		
}DBCOLINFO,*PDBCOLINFO;

typedef struct tagDBTableInfo
{
	char ColName[512] ;																	
	int Type  ;																			
	char *Data;																			
	int Size ;																			
	int IsNull;																			
	int IsDateEx ;																		
}DBTABLEINFO;

class DS_BASE_DLL CgxDBLoadBase
{
public:
	CgxDBLoadBase() ; 
	virtual ~CgxDBLoadBase() ;
	virtual void *GetDBPool(int _nIdx)=0;
	virtual int InitDB(char *_pDSN,char *_pDB,char*_pUserName,char*_pPass,int _nConnectCount,int _nPort=0)=0;		
	virtual int SetDBCount(int _nCount)=0;																			
	virtual int AddDB(int _nIdx,char *_pDSN,char *_pDB,char*_pUserName,char*_pPass,int _nConnectCount,int _nPort=0)=0;		
};

class DS_BASE_DLL CgxDBExecBase
{
public:
	CgxDBExecBase(void);
	virtual ~CgxDBExecBase(void);

	virtual int ExeProc(char * _ProcName , DB_PARAM_INFO * _ParamInfo) =0;							
	virtual int	ExeProc(const char *_ProcName,PDB_PARAM_INFO _ParamInfo,PDBCOLINFO _pColInfo)=0;	
	virtual int ExeSqlEx(char *_pTableName , DBTABLEINFO *_pTableInfo ,char * _pWhere )=0 ;			
	virtual int Exec(const char *pszStmt,__int64 *_pInserID=NULL)=0;								

	virtual DBCOLINFO* GetNextRecord()=0 ;															
	virtual int GetNext()=0;																		

	virtual void ClearSqlEx() =0;																	
	static void SetDebug(BOOL _bEnable);
	static BOOL m_bDebugFlag;
};

#endif
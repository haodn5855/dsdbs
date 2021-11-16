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
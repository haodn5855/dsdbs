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

#pragma once

#include <gxFunc.h>
#include <gxDBOdbcPool.h>
#include <gxDBExecBase.h>
#ifndef GX_DB_EXEC_ODBC_2021_01_01
#define GX_DB_EXEC_ODBC_2021_01_01

class CgxDBLoadOdbc :public CgxDBLoadBase
{
public:
	CgxDBLoadOdbc(void);
	virtual ~CgxDBLoadOdbc(void);
	void *GetDBPool(int _nIdx) ;												int InitDB(char *_pDSN,char *_pDB,char*_pUserName , char*_pPass,int _nConnectCount,int _nPort=0);			int SetDBCount(int _nCount);															int AddDB(int _nIdx , char *_pDSN,char *_pDB,char*_pUserName , char*_pPass,int _nConnectCount,int _nPort=0);		private:
	CgxDBPool * m_pPool;		int m_nPoolCount ;																	};

class CgxDBExecOdbc :public CgxDBExecBase
{
public:
	CgxDBExecOdbc(CgxDBPool *_pPool,int _nIdx=0);
	CgxDBExecOdbc(CgxDBLoadBase *_pDBLoad , int _nIdx=0) ; 
	virtual ~CgxDBExecOdbc() ; 
	int ExeProc(char * _ProcName , DB_PARAM_INFO * _ParamInfo) ;							int	ExeProc(const char *_ProcName,PDB_PARAM_INFO _ParamInfo,PDBCOLINFO _pColInfo);		int ExeSqlEx(char *_pTableName,DBTABLEINFO *_pTableInfo ,char * _pWhere ) ;				int Exec(const char *pszStmt,__int64 *_pInserID=NULL);									DBCOLINFO* GetNextRecord() ;															int GetNext();																			void ClearSqlEx() ;																	private:
	CgxDBPool *m_pPool ;															int m_nIdxFlag ;																		int FreeColInfo() ;																		int FreeConnect(int _WriteErrorFlag =0,char *_pszSmt=NULL) ;						
	SQLCONNECTINFO * m_pConnInfo ;															int m_nRecCount ;																		DBCOLINFO * m_pColInfo ;																int *m_pColLen ;																		int m_nColCount ;																	};
#endif
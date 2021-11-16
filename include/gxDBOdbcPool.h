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

#ifndef GX_DB_ODBC_POOL_2012_12_12_H
#define GX_DB_ODBC_POOL_2012_12_12_H

#include <gxDBExecBase.h>

namespace zsGame
{
class CgxDBPool :public CgxObj 
{
public:
	CgxDBPool();
	virtual ~CgxDBPool();

	virtual BOOL OnInitialUpdate(BOOL bOCI,BOOL bConnect=TRUE,int cnGetForReconnect=0);
	virtual BOOL OnInitialUpdate(const char *pszDBName,BOOL bOCI,const char *pszDSN,
		const char *pszUserName,const char *pszPassword,int cnConnect=10,BOOL bConnect=TRUE,int cnGetForReconnect=0);
public:
	BOOL Connect(PSQLCONNECTINFO pSQLConnect,int cnRetry=2);							
	PSQLCONNECTINFO GetConnect(const char *pszFile=NULL,int nLineNo=0);					
	BOOL Commit(PSQLCONNECTINFO pSQLConnect);											
	BOOL Rollback(PSQLCONNECTINFO pSQLConnect);											
	void FreeConnect(PSQLCONNECTINFO pSQLConnect);										
	void WriteError(PSQLCONNECTINFO pSQLConnect,SQLRETURN nRet,const char *pszFile=NULL,int nLine=0,char *pszState=NULL);	
	void WriteError(PSQLCONNECTINFO pSQLConnect,const char *pszFile=NULL,int nLine=0,const char *pszStmt=NULL);
	void ClearMoreResults(PSQLCONNECTINFO pSQLConnect);									
	
	int Exec(const char *pszStmt,PSQLCONNECTINFO pSQLConnect=NULL,BOOL bNeedPrepare=FALSE);		
	int Exec(const char *pszStmt, PDBCOLINFO pci, PSQLCONNECTINFO pSQLConnect = NULL);			
	int ExecProc(const char *pszProc,PDB_PARAM_INFO ppi,PSQLCONNECTINFO pSQLConnect=NULL);	
	int ExecProc2(const char *pszProc,PDB_PARAM_INFO ppi);									
	int ExecFunc(const char *pszFunc,PDB_PARAM_INFO ppi,PSQLCONNECTINFO pSQLConnect=NULL);	
	int ExecFunc2(const char *pszFunc,PDB_PARAM_INFO ppi);									
	int ExecSelect(const char *pszStmt,PDBCOLINFO pci,PSQLCONNECTINFO pSQLConnect);		
	int FetchData(PSQLCONNECTINFO pSQLConnect);											
	int ExecSelect(const char *pszStmt,PDBCOLINFO pci);									
	BOOL WriteLog(LPCSTR pszFmt, ...);
public:

	SQLHENV m_hEnv;
	char m_szDSN[256];																	
	char m_szUserName[64];																
	char m_szPassword[64];																
	char m_szDBName[64];
	int m_cnGetForReconnect;															
	int m_cnConnect;																	
	PSQLCONNECTINFO m_pConnectList;														
	HANDLE m_hSemaphore;																
	int m_cnWaitingForConnect;															
	int m_cnUsingConnect;																
	CgxCriticalSection m_cs;															
	int m_nLastConnect;
	int SetAutoCommit(PSQLCONNECTINFO pSQLConnect,BOOL value);							
private:
	void FreeSQLHandle(PSQLCONNECTINFO pSQLConnect);									
	BOOL GenePFStmt(char *pszStmt,const char *pszPF,PDB_PARAM_INFO ppi,PSQLCONNECTINFO pSQLConnect,BOOL bFunc);	
	int BindParam(PSQLCONNECTINFO pSQLConnect,int nPos,PDB_PARAM_INFO ppi);			
	int BindParam(PSQLCONNECTINFO pSQLConnect,int nPos,PDBCOLINFO pci);					
	int BindCol(PSQLCONNECTINFO pSQLConnect,int nPos,PDBCOLINFO pci);					
	INT OCIReplaceEmpno(const char* pszStmtEmpno,LPSTR pszBufStmt,INT nBufLen);			
};
}
#endif 

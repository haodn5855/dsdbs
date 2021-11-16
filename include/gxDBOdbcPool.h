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

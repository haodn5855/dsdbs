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

#include "gxdbexecOdbc.h"
#include "gxDBPlay.h"
#include "gxDSKernel.h"

CgxDBPlay::CgxDBPlay(void *_pPool,int _nDBType)
{
	if( _pPool == NULL ) 
		m_pBase= NULL ;
	switch(_nDBType)
	{
	case DS_DB_TYPE_ODBC :
		m_pBase = new CgxDBExecOdbc((CgxDBPool *)_pPool) ;
		break;		  
					default :
		m_pBase = CgxDSKernel::m_SysInterfaceCfgItems.CreateDBExec(_nDBType, _pPool) ;
		break; 
	}
}
CgxDBPlay::~CgxDBPlay(void)
{
	if( m_pBase !=NULL ) 
	{
		delete m_pBase ;
		m_pBase = NULL ;
	}
}
int CgxDBPlay::ExeProc(char *_pProcName,DB_PARAM_INFO * _pParamInfo)
{
	if( m_pBase==NULL ) 
		return -1 ;
	return m_pBase->ExeProc(_pProcName,_pParamInfo) ; 
}
int CgxDBPlay::ExeProc(const char *_pProcName,PDB_PARAM_INFO _pParamInfo,PDBCOLINFO _pColInfo)
{
	if( m_pBase==NULL ) 
		return -1 ;
	return m_pBase->ExeProc(_pProcName,_pParamInfo,_pColInfo) ; 
}
int CgxDBPlay::ExeSqlEx(char *_pTableName,DBTABLEINFO *_pTableInfo,char * _pWhere ) 
{
	if( m_pBase==NULL ) 
		return -1 ;
	return m_pBase->ExeSqlEx(_pTableName,_pTableInfo ,_pWhere) ; 
}
int CgxDBPlay::Exec(const char *pszStmt,__int64 *_pInserID)
{
	if( m_pBase==NULL ) 
		return -1 ;
	return m_pBase->Exec(pszStmt,_pInserID) ; 
}

DBCOLINFO* CgxDBPlay::GetNextRecord() 
{
	if( m_pBase==NULL ) 
		return NULL ;
	return m_pBase->GetNextRecord() ; 
}
int CgxDBPlay::GetNext()
{
	if( m_pBase==NULL ) 
		return -1 ;
	return m_pBase->GetNext() ; 
}
void CgxDBPlay::ClearSqlEx()
{
	if( m_pBase==NULL ) 
		return  ;
	m_pBase->ClearSqlEx() ; 
}

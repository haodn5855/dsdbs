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

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

#include "gxLog.h"
#pragma warning(disable:4996)

CgxList<GX_LOG_INFO> CgxLog::m_LogList;
int CgxLog::m_nLogThreadRunFlag =0 ;
CgxCriticalSection CgxLog::m_LogLock;
HANDLE CgxLog::m_hLogThread=NULL;
DWORD CgxLog::m_dwLogThreadID=0;

CgxLog::CgxLog() 
{
	InitLogThread() ;
}
CgxLog::~CgxLog()
{
	CloseLogThread() ;
}
int CgxLog::GetLogFileName(char *_pFileName)
{
	if( _pFileName == NULL ) 
		return -1 ; 
	char Buf[1024] ; 
	memset(Buf , 0,sizeof(Buf)) ; 
	GetModuleFileName(g_hInstance,Buf,1024) ; 
	strncpy(_pFileName , Buf , strlen(Buf) -3) ;
	strcat(_pFileName,"Log") ;
return 0 ; 
}
DWORD WINAPI CgxLog::FuncThreadLog(void * _pVar) 
{
	for( ;m_nLogThreadRunFlag;Sleep(10) )
	{
		if( m_LogList.GetCount() >0) 
		{
			char szFileName[MAX_PATH];
			memset(szFileName, 0 , sizeof(szFileName)) ;
			GetLogFileName(szFileName) ;
			CgxList<GX_LOG_INFO> TmpList;
			m_LogLock.Lock() ;
			int Count = m_LogList.GetCount() ;
			for(int i = 0; i<Count; i ++ ) 
			{
				GX_LOG_INFO * pLog = m_LogList.GetAt(i) ;
				if( pLog == NULL ) 
					continue ;
				TmpList.AddTail(pLog);
			}
			m_LogList.RemoveAll() ;
			m_LogLock.Unlock() ;
			FILE * fp = fopen(szFileName , "ab+") ;
			if( fp == NULL )
				continue; 
			for( int i = 0 ;i < Count ;i ++ ) 
			{
				GX_LOG_INFO * pLog = TmpList.GetAt(i) ;
				if( pLog == NULL ) 
					continue;
				fwrite(pLog->szData , pLog->nDataLen,1,fp) ; 
				if( i%10 ==8)
					fflush(fp) ;
			}
			fclose(fp) ; 
			TmpList.DeleteAll() ;
		}
	}
return 0 ;
}
int CgxLog::InitLogThread()
{
	m_nLogThreadRunFlag = 1;
	m_hLogThread=CreateThread(NULL,0,FuncThreadLog,NULL,0,&m_dwLogThreadID);
return 0 ;
}
void CgxLog::CloseLogThread() 
{
	m_nLogThreadRunFlag =0 ;
	WaitForSingleObject(m_hLogThread,INFINITE);
	CloseHandle(m_hLogThread) ;
}
int CgxLog::AddLog(char *_pData,int _nLen)
{
	if(( _pData == NULL ) || (_nLen <1))
		return -1 ;
	GX_LOG_INFO * pInfo = new GX_LOG_INFO ;
	memset(pInfo , 0,sizeof(GX_LOG_INFO) ) ;
	pInfo->nDataLen= _nLen ;
	if( pInfo->nDataLen>GX_MAX_LOG_BUF)
		pInfo->nDataLen = GX_MAX_LOG_BUF;
	memcpy(pInfo->szData , _pData,pInfo->nDataLen) ;
	m_LogLock.Lock() ;
	m_LogList.AddTail(pInfo) ;
	m_LogLock.Unlock() ;
return 0 ; 
}

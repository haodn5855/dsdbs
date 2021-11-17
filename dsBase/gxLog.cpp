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

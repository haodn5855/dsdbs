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

#include "gxFunc.h"
#include <stdio.h>
#include <time.h>
#include <gxMemDebug.h>
#pragma warning(disable:4996)

CgxFunc g_Func ; 
HINSTANCE g_hInstance  = NULL ; 

CgxFunc::CgxFunc()
{
 	m_pDebugBuf = NULL ; 
	m_nDebugBufLen = 4096 ; 
}
CgxFunc::~CgxFunc()
{
	ReleaseAll() ; 
}
int CgxFunc::Init()
{
	int nRet = 0 ; 
	Lock() ; 
	if( m_pDebugBuf!=NULL)
	{
		delete []m_pDebugBuf ;
		m_pDebugBuf = NULL ; 
	}
	m_pDebugBuf = new char[m_nDebugBufLen] ;
	Unlock() ; 
return nRet ; 
}
int CgxFunc::ReleaseAll()
{
	m_Lock.Lock() ; 
	if( m_pDebugBuf!=NULL) 
		delete []m_pDebugBuf ; 
	m_pDebugBuf = NULL ; 
	m_Lock.Unlock() ; 
return 0 ; 
}
time_t AToTime(const char * _Time)
{
	time_t t ; 
	struct tm tm1;
	sscanf(_Time, "%4d-%2d-%2d %2d:%2d:%2d",     
		&tm1.tm_year, 
		&tm1.tm_mon, 
		&tm1.tm_mday, 
		&tm1.tm_hour, 
		&tm1.tm_min,
		&tm1.tm_sec);
	tm1.tm_year -= 1900;
	tm1.tm_mon --;
	tm1.tm_isdst=-1;
	t = mktime(&tm1);
return t ; 
}
char * TimeToA(time_t _Time,char * _TimeStr) 
{
	if( _TimeStr== NULL ) 
		return NULL ; 
	int year=0,mon=0,day=0,hour=0,min=0,sec=0;
	struct tm *m_tm; 
	m_tm = localtime(&(time_t)_Time);
	year = m_tm->tm_year+1900;
	mon = m_tm->tm_mon+1;
	day = m_tm->tm_mday;
	hour = m_tm->tm_hour;
	min = m_tm->tm_min;
	sec = m_tm->tm_sec; 
	sprintf(_TimeStr, "%04d-%02d-%02d %02d:%02d:%02d", 
		year, mon, day, hour, min, sec);
return _TimeStr ; 
}
int WriteLogHead(char *_pBuf)
{
	if( _pBuf == NULL ) 
		return -1; 

	SYSTEMTIME st ;
	GetLocalTime(&st) ; 
	sprintf(_pBuf , "%04d-%02d-%02d %02d:%02d:%02d:%03d %08X " , 
		st.wYear ,
		st.wMonth ,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds,
		GetCurrentThreadId( ) );
return 0 ; 
}
int gxWriteLog(const char *_pFmt ,...) 
{
	char szBuf[GX_MAX_LOG_BUF];
	memset(szBuf , 0 , sizeof(szBuf)) ; 
	WriteLogHead(szBuf) ;

	char * p = szBuf ;
	p+= strlen(szBuf);

	va_list	va;
	va_start(va,_pFmt);
	vsprintf(p,_pFmt,va);
	va_end(va);
	strcat(p,"\r\n");
	g_Func.m_Log.AddLog(szBuf,strlen(szBuf)) ;
return 0 ; 
}
int gxDebugStr(const char *_pFmt ,...)
{
	int nRet = 0 ; 
	g_Func.Lock() ; 
	if( g_Func.m_pDebugBuf!=NULL)
	{
		memset(g_Func.m_pDebugBuf , 0 ,sizeof(char) *g_Func.m_nDebugBufLen) ; 
		va_list	va;
		va_start(va,_pFmt);
		vsprintf(g_Func.m_pDebugBuf,_pFmt,va);
		va_end(va);
		OutputDebugString(g_Func.m_pDebugBuf) ; 
		OutputDebugString("\r\n") ;
	}else
		nRet =-1;
	g_Func.Unlock() ; 
return nRet ; 
}
BOOL StartThread(LPTHREAD_START_ROUTINE _Func , LPVOID _pParam) 
{
return QueueUserWorkItem(_Func ,_pParam , WT_EXECUTEINIOTHREAD) ;
}
int gxSysInit(HINSTANCE _hInstance)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	g_hInstance = _hInstance ; 
	g_Func.Init() ; 
	gxWriteLog("------------------ϵͳ��ʼ����-----------------") ; 
return 0 ; 
}
int gxSysRelease() 
{
	g_Func.ReleaseAll() ; 
return 0 ; 
}
int GetCfgFileName(char *_pFileName)
{
	if( _pFileName == NULL ) 
		return -1 ; 
	char Buf[1024] ; 
	memset(Buf , 0,sizeof(Buf)) ; 
	GetModuleFileName(g_hInstance,Buf,1024) ; 
	strncpy(_pFileName , Buf , strlen(Buf) -3) ;
	strcat(_pFileName,"Xml") ;
return 0 ; 
}
int GetPathName(char *_pFileName)
{
	if( _pFileName == NULL) 
		return -1 ; 
	char Buf[512];
	memset(Buf , 0 , sizeof(Buf)) ; 
	GetModuleFileName(g_hInstance,Buf , sizeof(Buf)) ;
	int Len = strlen(Buf) ; 
	for( int i = Len-1 ;i>=0 ;i--)
	{
		if( Buf[i]=='\\')
		{
			Buf[i+1]= 0;
			break; 
		}
	}
	strcpy(_pFileName , Buf) ;
return 0 ; 
}
#pragma warning(disable:4244)
uint64_t ntoh64(const uint64_t *_pInput)
{
    uint64_t rval=0;
    uint8_t *pData = (uint8_t *)&rval;

    pData[0] = *_pInput >> 56;
    pData[1] = *_pInput >> 48;
    pData[2] = *_pInput >> 40;
    pData[3] = *_pInput >> 32;
    pData[4] = *_pInput >> 24;
    pData[5] = *_pInput >> 16;
    pData[6] = *_pInput >> 8;
    pData[7] = *_pInput >> 0;
    return rval;
}
#pragma warning(default:4244)

uint64_t hton64(const uint64_t *_pInput)
{
    return (ntoh64(_pInput));
}

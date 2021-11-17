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
	gxWriteLog("------------------系统初始化！-----------------") ; 
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

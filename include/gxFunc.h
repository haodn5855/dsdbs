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

#ifndef GX_FUNC_2011_09_08
#define GX_FUNC_2011_09_08
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <gxCriticalSection.h>
#include "gxList.h"
#include "gxLog.h"

using namespace zsGame ; 

#define _TXT_LINE(x)  #x
#define __TXT_LINE(x)  _TXT_LINE(x)
#define _TXT__LINE  __TXT_LINE(__LINE__)
#define CodeMsg(x)  "������ʾ:  " #x "   "   __FILE__ "  ( "   _TXT__LINE  " ) " 
#define Code_Msg(x) message(CodeMsg(x))



class CgxFunc
{
public:
	CgxFunc() ; 
	~CgxFunc() ; 
	int Init() ; 
	int ReleaseAll() ; 
public:
	char *m_pDebugBuf ;
	int m_nDebugBufLen ;
	inline void Lock(){m_Lock.Lock();}; 
	inline void Unlock(){ m_Lock.Unlock();} ;
private:
	CgxCriticalSection m_Lock ;
public:
	CgxLog m_Log;
};

extern HINSTANCE g_hInstance ;
extern CgxFunc g_Func;


extern int gxSysInit(HINSTANCE _hInstance) ;			
extern int gxSysRelease() ;								
extern int GetCfgFileName(char *_pFileName);			
extern int GetPathName(char *_pFileName);				


extern time_t AToTime(const char * _Time);				
extern char * TimeToA(time_t _Time,char * _TimeStr) ;	


extern int gxWriteLog(const char *_pFmt ,...) ; 

extern int gxDebugStr(const char *_pFmt ,...) ; 

extern BOOL StartThread(LPTHREAD_START_ROUTINE _Func , LPVOID _pParam) ;

extern uint64_t ntoh64(const uint64_t *input);
extern uint64_t hton64(const uint64_t *input);


#endif
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

#include <gxFunc.h>
#include <gxList.h>
#pragma once
#ifndef GX_SYS_INTERFACE_CFG_2021_01_01
#define GX_SYS_INTERFACE_CFG_2021_01_01

typedef void *(WINAPI *DSCreateDBLoad)(int _nID) ;
typedef void *(WINAPI *DSCreateDBExec)(int _nID,void*_pPool); 
typedef void *(WINAPI *DSCreateMsgBuf)(int _nID); 
class CgxDBLoadBase;
class CgxDBExecBase;
class CgxMsgBufBase;
class CgxSysInterfaceCfg
{
public:
	CgxSysInterfaceCfg(void);
	virtual ~CgxSysInterfaceCfg(void);
	int m_nInterfaceID;
	char m_szIFName[128];
	char m_szIFPath[256];
	int m_nIsEnable;
	int LoadDll() ;

	DSCreateDBLoad m_FuncCreateDBLoad;
	DSCreateDBExec m_FuncCreateDBExec;
	DSCreateMsgBuf m_FuncCreateMsgBuf;
	CgxDBLoadBase * m_pDBLoadBase;
	CgxDBExecBase * m_pDBExecBase;
	CgxMsgBufBase * m_pMsgBufBase;
private:
	HMODULE m_hDll ;
};
class CgxSysInterfaceCfgList
{
public:
	CgxSysInterfaceCfgList(void);
	virtual ~CgxSysInterfaceCfgList(void);
	int LoadSysInterfaceCfg(int _nDSSVRID,void *_pDBPool,int _nDBType) ;
	CgxSysInterfaceCfg * GetItemOfIdx(int _nIdx) ;
	CgxSysInterfaceCfg * FindItem(int _nID) ;
	int GetItemCount() ;

	CgxDBLoadBase * CreateDBLoad(int _nID) ;
	CgxDBExecBase * CreateDBExec(int _nID ,void *_pPool) ;
	CgxMsgBufBase * CreateMsgBuf(int _nID) ;
private:
	CgxList <CgxSysInterfaceCfg> m_SysIFItems;
	static int CompFuncSysIF(const void *_pItem1,const void *_pItem2);							};
#endif
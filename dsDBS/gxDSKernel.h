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

#pragma once
#include <gxDBSDll.h>
#include <string>
#include "gxDBPlay.h"
#include "gxMsgBufPlay.h"
#include "gxDBSysMsgList.h"
#include "gxDBSysMsgPlay.h"
#include "gxSysInterfaceCfg.h"

#ifndef GX_DS_KERNEL_2021_01_01
#define GX_DS_KERNEL_2021_01_01

using namespace std;

class  CgxDSKernel
{
public:
	CgxDSKernel();
	virtual ~CgxDSKernel(void);
	void ReleaseAll() ;
	int Init(int _nDBType,char *_pDBName,char *_pDsnName,char *_pIP,int _nPort,int _nCnntCount,char*_pUser,char *_pPass);
private:
	int InitDB(int _nDBType,char *_pDBName,char *_pDsnName,char *_pIP,int _nPort,int _nCnntCount,char*_pUser,char *_pPass);
	int InitDBSvrConnect();
	int InitProtocol();
	int InitDBSysMsgHeadLen() ;
	int InitAckMsgInfo() ;
	int InitAckMsgInfo(CgxDBSysMsg &_HeadMsg,CgxDBSysMsgPlay& _MsgPlay);
private:
	CgxDBSysDBSvrInfo m_SysDBInfo;						int LoadDBSys(void *_pDBPool,int _nDBType) ;
public:
	CgxDBSysMsgList m_DBSysMsgItems;
	CgxDBSysMsgPlayList m_DBSysMsgPlayItems;
	static CgxSysInterfaceCfgList m_SysInterfaceCfgItems;
private:
	int LoadDBSys(void *_pDBPool,int _nDBType,int _nDSSVRID) ;
	int LoadDBSysMsg(void *_pDBPool,int _nDBType,int _nDSSVRID); 
	int LoadDBSysStruct(void *_pDBPool,int _nDBType,int _nDSSVRID); 
	int LoadDBSysMsgMember(void *_pDBPool,int _nDBType,int _nDSSVRID); 
private:
	CgxList<CgxDBSysDBSvrInfo> m_DBSysDBSvrInfoItems;
	int LoadDBSysDBSvrInfo(void *_pDBPool,int _nDBType) ;
	CgxDBSysDBSvrInfo * GetDBSvrInfo(int _ID);
	static int CompFuncDBSysDBSvrInfo(const void *_pItem1, const void *_pItem2);
public:
	int OnMsg(DWORD _dwMsgID ,string _Req , string &_Ack) ;					
	void SetDebug(BOOL _bEnable=TRUE);
	inline void SetDSSVRID(int _nDSSVRID){m_nDSSVRID = _nDSSVRID;};
	int GetHeadLen(DWORD _dwProtoType,DWORD _dwMsgID) ;
	int GetMsgLen(void *_pMsgHeadData,int _nDataLen,DWORD _dwProtoType,DWORD _dwMsgID) ;
	int GetMsgType(void *_pMsgHeadData ,int _nDataLen,DWORD _dwProtoType,DWORD _dwMsgID) ;
private:
	CgxDBSysMsgMember * GetHeadMemberOfType(int _nMsgID,int _nHeadMemberType);
private:
	BOOL m_bDebugFlag;
	int m_nDSSVRID;
};

#endif

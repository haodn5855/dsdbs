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
#ifndef GX_DBSYS_MSG_LIST_2021_11_11
#define GX_DBSYS_MSG_LIST_2021_11_11
#include <gxFunc.h>
#include "gxList.h"
#include <gxBaseDll.h>

using namespace zsGame;
#pragma warning(disable:4251)

class DS_BASE_DLL CgxDBSysMsgMember
{
public:
	CgxDBSysMsgMember();
	virtual ~CgxDBSysMsgMember();
public:
	int m_nDSMsgMemberID;
	int m_nDSMsgID;
	char m_szMemberAttribute[128];
	char m_szMemberName[128];
	int m_nMemberClass;
	int m_nMemberType;
	int m_nMemberLen;
	int m_nMemberOrder;
	char m_szDefaultStr[512];
	int m_nHeadMemberType;
	char m_szComment[512];
};

class DS_BASE_DLL CgxDBSysStruct
{
public:
	int m_nDSSTID;
	char m_szStructName[128];
	int m_nDSSVRID;
	char m_szComment[512];
public:
	int m_nOriginalDSSTID;
public:
	CgxDBSysStruct();
	virtual ~CgxDBSysStruct();

	void ReleaseAll();	
	CgxDBSysMsgMember * GetMemberItem(int _nIdx);															
	int GetMemberCount() ;																			
	int AddMemberItem(CgxDBSysMsgMember *_pItem);
	void WriteLog();
private:
	 CgxList<CgxDBSysMsgMember> m_DBMemberItems;
};

class DS_BASE_DLL CgxDBSysMsg
{
public:
	CgxDBSysMsg() ;
	virtual ~CgxDBSysMsg();
public:
	int m_nDSMsgID;
	int m_nMsgHeadID;
	int m_nMsgHeadProto;
	int m_nMsgBodyProto;
	int m_nHeadLenInMsg;
	int m_nSuffixStartPos;
	int m_nIsLoginMsg;
	char m_szMsgName[256];
	char m_szSuffixCount[128];
	char m_szSuffixSumLen[128];
	int m_nSuffixStructID;
	int m_nDSSVRID;
	char m_szComment[512];

	int m_nHeadLen;
public:
	void ReleaseAll() ;
	void Clear();																					
	CgxDBSysStruct *GetStruct(int _nIdx);																
	int AddStruct(CgxDBSysStruct *_pItem) ;																
	CgxDBSysStruct * AddStruct(int _nID ) ;																
	int GetStructCount() ;																			
	void WriteLog();
private:
	CgxList<CgxDBSysStruct> m_DBStructItems;
};

class DS_BASE_DLL CgxDBSysMsgList
{
public:
	CgxDBSysMsgList(void);
	virtual ~CgxDBSysMsgList(void);
	void ReleaseAll();																				
	void WriteLog() ;																				

	CgxDBSysMsg * GetMsg(int _nIdx);																
	CgxDBSysMsg * GetMsg(char *_pMsgName) ;														
	CgxDBSysMsg *GetMsgOfID(int _nID) ;															
	CgxDBSysStruct * GetStruct(int _nIdx);																
	CgxDBSysStruct * GetStructOfID(int _nID) ;															
	int InsertMsg(CgxDBSysMsg * _pMsg) ;															
	int InsertStruct(CgxDBSysStruct *_pStruct) ;															
	int GetMsgCount();
	int GetStructCount() ; 
private:
	CgxList<CgxDBSysMsg> m_DBMsgItems;
	CgxList<CgxDBSysStruct> m_DBStructItems;
	static int CompFuncDBSysMsg(const void *_pItem1, const void *_pItem2);							
public:
	static int CompFuncDBSysStruct(const void *_pItem1, const void *_pItem2);							
public:
	int SetDBSysMsgMember(CgxDBSysMsgMember *_pMember);
};
#pragma warning(default:4251)

#endif
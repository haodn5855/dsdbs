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
#ifndef GX_BYTE1_MSG_2021_01_01
#define GX_BYTE1_MSG_2021_01_01

#include <gxFunc.h>
#include "gxMsgBufBase.h"
#include "gxDBSysMsgList.h"

class CgxByte1Member
{
public:
	CgxByte1Member();
	CgxByte1Member(CgxDBSysMsgMember *_pDBSysMember);
	virtual ~CgxByte1Member() ;
	int m_nIdx;																	
	char m_szMemberName[256];													
	int m_nType;																
	int m_nLen;																	
	int m_nSize;																
	int m_nOffSet;																
	int m_nFlag;			

	int m_nDSMsgMemberID;
	int m_nDSMsgID;
	int m_nMemberOrder;
	char m_szDefaultStr[512];
	int m_nHeadMemberType;
};

class CgxByte1Struct
{
public:
	CgxByte1Struct() ;
	CgxByte1Struct(CgxDBSysStruct *_pDBSysStruct) ;
	virtual ~CgxByte1Struct() ;
public:
	int m_nID ;																							char m_szStructName[256];																			int m_nOffSet ;																						int m_nBlackSize;																					int m_nItemTypeMaxSize;																				int GetDataLen() ;																					int SetDataLen(int _nLen);																		
	int SetStruct(CgxDBSysStruct *_pDBSysStruct) ;
	void ReleaseAll() ;
	int GetSuffixTypeSize(char *_pBuf);																	int GetStaticDataLen() ;																			int InitStructSize();																				static int GetTypeOffset(int _Type);																static int GetMemberType(char *_pType);																CgxByte1Member * GetMemberItem(int _nIdx);																int ResetMemberLen(CgxByte1Member *_pItem) ;																int GetMemberCount() ;																				void WriteLog() ;																				private:
	int m_nDataLen ;																					CgxList<CgxByte1Member> m_MemberItems;
};

class CgxByte1Msg
{
public:
	CgxByte1Msg() ;
	CgxByte1Msg(CgxDBSysMsg *_pDBSysMsg);
	virtual ~CgxByte1Msg() ;
public:
	int m_nID ;																							char m_szMsgName[256];																				int m_SuffixStructID ;																				int m_SuffixCountIdx;																					int m_SuffixCountSubIdx;
	int m_SuffixSumSizeIdx;																				int m_SuffixSumSizeSubIdx;																			int m_SuffixStructSizeIdx;																			int m_nStructSize ;																					int m_nStaticStructSize;																			int GetDataLen(char *_pBuf) ;																		int GetSuffixTypeSize(char *_pBuf);																
		int m_nMsgHeadID;
	int m_nMsgHeadProto;
	int m_nMsgBodyProto;
	int m_nHeadLenInMsg;
	int m_nSuffixStartPos;

	int GetStaticDataLen() ;																			int InitMsgSize() ;																					void ReleaseAll() ;
	void Clear();																					
	int GetMemberOffset(int _nMember,int _nAdd,int _nSub) ;
	CgxByte1Member * GetMember(int _nMember,int _Add,int _nSub);

	CgxByte1Struct *GetStruct(int _nIdx);																	int AddStruct(CgxByte1Struct *_pItem) ;																	CgxByte1Struct * AddStruct(int _nID ) ;																	int GetStructCount() ;																				void WriteLog() ;																					CgxByte1Struct m_SuffixStruct;				private:
	CgxList<CgxByte1Struct> m_StructItems;
};

class CgxByte1MsgList
{
public:
	CgxByte1MsgList();
	virtual ~CgxByte1MsgList();
	int Init(CgxDBSysMsgList *_pDBSysMsgList) ;
	void ReleaseAll();																					void WriteLog() ;																					CgxByte1Msg * GetMsg(int _nIdx);																	CgxByte1Msg * GetMsg(char *_pMsgName) ;															CgxByte1Msg *GetMsgOfID(int _nID) ;																CgxByte1Struct * GetStruct(int _nIdx);																	CgxByte1Struct * GetStructOfID(int _nID) ;																int InsertMsg(CgxByte1Msg * _pMsg) ;																int InsertStruct(CgxByte1Struct *_pStruct) ;																static int CompFuncByte1Msg(const void *_pItem1, const void *_pItem2);								static int CompFuncByte1Struct(const void *_pItem1, const void *_pItem2);								int InitMsgList() ;							
private:
	CgxList<CgxByte1Struct> m_StructItems;
	CgxList<CgxByte1Msg> m_MsgItems ;
	int SetSuffixStruct(CgxDBSysMsgList* _pDBSysMsgList ,CgxByte1Msg* _pByte1Msg) ;
};
#endif
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
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
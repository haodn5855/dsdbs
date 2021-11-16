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
#ifndef GX_MSGBUFBASE_2021_01_01
#define GX_MSGBUFBASE_2021_01_01
#include <gxFunc.h>
#include <gxBaseDll.h>

#define		DS_MSG_TYPE_NONE					20000
#define		DS_MSG_TYPE_BYTE1					20001
#define		DS_MSG_TYPE_BYTE2					20002
#define		DS_MSG_TYPE_BYTE4					20004
#define		DS_MSG_TYPE_GOOGLE_PROTOBUF			20008
#define		DS_MSG_TYPE_XML						20016
#define		DS_MSG_TYPE_JOSN					20032
#define		DS_MSG_TYPE_JCE						20064

#define		DS_MSG_TYPE_MAX						29999


#define		GX_MSGBUF_DATATYPE_CHAR					0														
#define		GX_MSGBUF_DATATYPE_BYTE					1														
#define		GX_MSGBUF_DATATYPE_SWORD				2														
#define		GX_MSGBUF_DATATYPE_WORD					3														
#define		GX_MSGBUF_DATATYPE_INT					4														
#define		GX_MSGBUF_DATATYPE_UINT					5														
#define		GX_MSGBUF_DATATYPE_STR					6														
#define		GX_MSGBUF_DATATYPE_RAWSTR				7														
#define		GX_MSGBUF_DATATYPE_BINARY				8														
#define		GX_MSGBUF_DATATYPE_LONGLONG				9														
#define		GX_MSGBUF_DATATYPE_FLOAT				10														
#define		GX_MSGBUF_DATATYPE_DATE					11														
#define		GX_MSGBUF_DATATYPE_DECIMAL				12														
#define		GX_MSGBUF_DATATYPE_DOUBLE				15	
#define		GX_MSGBUF_DATATYPE_SUFFIX				31		

#define		GX_MSGBUF_DATATYPE_MAX					1000	


typedef struct tagDSMsgSuffix
{
	DWORD dwOffSet;
	DWORD dwSize;
}MSG_SUFFIX;
class DS_BASE_DLL CgxMsgHeadInfo
{
public:
	CgxMsgHeadInfo();
	virtual ~CgxMsgHeadInfo();
public:
	DWORD m_dwMsgHeadID ;
	DWORD m_dwMsgHeadProto;
	DWORD m_dwMsgBodyProto;
	MSG_SUFFIX m_MemberMsgID;
	MSG_SUFFIX m_MemberMsgLen;
	MSG_SUFFIX m_MemberSerial;
	DWORD m_dwHeadLen;
};
class DS_BASE_DLL CgxMsgBufLoadBase
{
public:
	CgxMsgBufLoadBase(){};
	virtual ~CgxMsgBufLoadBase(){};
};

extern DS_BASE_DLL int GetMemberAndSubIdx(char *_pBuf , int &_nMemberIdx,int &_nSubIdx);
extern DS_BASE_DLL int GetMemberAddSub(char *_pBuf , int &_nMember,int &_nAdd,int &_nSub);
class CgxDBSysMsgList;
class DS_BASE_DLL CgxMsgBufBase
{
public:
	DWORD m_dwMsgID;
	DWORD m_dwMsgHeadID ;
	DWORD m_dwMsgHeadProto;
	DWORD m_dwMsgBodyProto;
public:
	CgxMsgBufBase(CgxMsgBufLoadBase *_pLoad , DWORD _dwMsgType){};
	CgxMsgBufBase();
	virtual ~CgxMsgBufBase(void);
	virtual int InitProtocol(CgxDBSysMsgList *_pSysMsgList) = 0 ;		
	virtual int InitMsg(int m_nMsgID,char *_pMsgBuf,int _nMsgLen) =0;
	virtual void ReleaseAll() = 0 ;
	virtual int GetHeadLen() = 0 ;	
	virtual int WriteDatalog(char*_pBuf,int _nBufLen)=0;				
	virtual int SetData(void *_pData, int _nDataLen,int _nMember,int _nAdd,int _nSub) =0;
	virtual void* GetData(int _nMember,int _nAdd,int _nSub)=0 ;
	virtual void* GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int GetDataLen(int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int GetDataType(int _nMember,int _nAdd,int _nSub)=0;
	
	
	virtual int SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int SetShortData(short _nData ,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) =0;
	virtual int SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int SetInt64Data(__int64 _Data ,int _nMember,int _nAdd,int _nSub) =0 ;
	virtual int SetSuffixData(void *_pData , int _nDataLen ,int _nMember,int _nAdd,int _nSub)=0;

	virtual int GetIntData(int _nMember,int _nAdd,int _nSub)=0;
	virtual short GetShortData(int _nMember,int _nAdd,int _nSub)=0;
	virtual float GetFloatData(int _nMember,int _nAdd,int _nSub)=0;
	virtual double GetDoubleData(int _nMember,int _nAdd,int _nSub) =0;
	virtual __int64 GetInt64Data(int _nMember,int _nAdd,int _nSub)=0;
	virtual void* GetSuffixData(int _nMember,int _nAdd,int _nSub)=0;
	virtual int GetSuffixDataLen(int _nMember,int _nAdd,int _nSub)=0 ;
	virtual void* GetSuffixData(int &_nSuffixLen ,int _nMember,int _nAdd,int _nSub)=0;

	virtual int GetDataStr(char *_pstrValue ,int &_nValueLen,int _nMember,int _nAdd,int _nSub)=0; 
	virtual int GetHeadInfo(CgxMsgHeadInfo &_Info) =0;
	
	virtual int BeginAddData()=0;
	virtual int EndAddData(int _nAddCount)=0;
	virtual int GetDataLen() = 0 ;
	virtual void * GetData() = 0 ;
};

#endif
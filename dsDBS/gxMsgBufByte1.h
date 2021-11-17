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
#ifndef GX_MSGBUF_BYTE1_2021_01_01
#define GX_MSGBUF_BYTE1_2021_01_01

#include "gxMsgBufBase.h"
#include "gxDBSysMsgList.h"
#include "gxByte1Msg.h"

#define MSGBUF_BYTE1_SIZE	2048


class CgxMemBuf
{
public:
	CgxMemBuf() ; 
	virtual ~CgxMemBuf() ;
	char *GetData() ;
	int AddData(void *_pData,int _nLen);
	BOOL ReallocMem(int _nAddSize=1024);
	int GetBufLen() ;
	int GetDataLen() ;
	int InitDataLen(int _nLen) ;
private:
	char *m_pData;
	int m_nDataLen;						
	int m_nBufLen;					
};

class CgxMsgBufByte1 :	public CgxMsgBufBase
{
public:
	CgxMsgBufByte1();
	virtual ~CgxMsgBufByte1(void);
public:
	virtual int InitProtocol(CgxDBSysMsgList *_pSysMsgList) ; 	
	virtual int InitMsg(int _nMsgID,char *_pMsgBuf,int _nMsgLen) ;
	virtual void ReleaseAll() ;
	virtual int GetHeadLen()  ;		virtual int WriteDatalog(char*_pBuf,int _nBufLen);					virtual int SetData(void *_pData, int _nDataLen,int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetData(int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) ;
	virtual int GetDataLen(int _nMember,int _nAdd,int _nSub) ;
	virtual int GetDataType(int _nMember,int _nAdd,int _nSub);
	
	virtual int SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetShortData(short _nData,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetInt64Data(__int64 _Data ,int _nMember,int _nAdd,int _nSub)  ;
	virtual int SetSuffixData(void *_pData ,int _nDataLen,int _nMember,int _nAdd,int _nSub);

	virtual int GetIntData(int _nMember,int _nAdd,int _nSub);
	virtual short GetShortData(int _nMember,int _nAdd,int _nSub);
	virtual float GetFloatData(int _nMember,int _nAdd,int _nSub);
	virtual double GetDoubleData(int _nMember,int _nAdd,int _nSub) ;
	virtual __int64 GetInt64Data(int _nMember,int _nAdd,int _nSub);
	virtual void* GetSuffixData(int _nMember,int _nAdd,int _nSub);
	virtual int GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetSuffixData(int &_nSuffixLen ,int _nMember,int _nAdd,int _nSub);
	virtual int GetDataStr(char *_pstrValue ,int &_nValueLen,int _nMember,int _nAdd,int _nSub); 

	virtual int GetHeadInfo(CgxMsgHeadInfo &_Info) ;

	virtual int BeginAddData();
	virtual int EndAddData(int _nAddCount);
	virtual int GetDataLen();
	virtual void * GetData();
	MSG_SUFFIX *GetSuffix(int _nMember,int _nAdd,int _nSub);
public:
	static int Init(CgxDBSysMsgList *_pSysMsgList);
public:
	static int WriteByte1MsgListLog();
	int WriteDataLog();
	int GetLogStr(char *_pBuf,void *_pItem,int _nMember,int _nAdd,int _nSub);
	int WriteDataLog(CgxByte1Struct *_pStruct);
private:
	static CgxByte1MsgList m_Byte1MsgItems;
	CgxByte1Msg *m_pByte1Msg;
	CgxMemBuf m_Buf ;
	CgxMemBuf m_AddBuf;					
	CgxMemBuf m_SuffixBuf ;				
	int CheckAddBufLen(int _nAddIdx);
};

#endif
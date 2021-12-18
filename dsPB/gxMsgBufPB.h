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
#ifndef GX_MSGBUF_PB_H_2021_06_13
#define GX_MSGBUF_PB_H_2021_06_13
#include "gxMsgbufbase.h"
#include "gxPBInterface.h"

using namespace google;
using namespace protobuf;

class CgxDBSysMsgList;
class CgxDBSysStruct;
class CgxDBSysMsgMember;
class CgxDBSysMsg;

class CgxMultiFileErrorCollector :public compiler::MultiFileErrorCollector
{
public:
	CgxMultiFileErrorCollector(void);
	virtual ~CgxMultiFileErrorCollector(void);
	virtual void AddError(const string& _FileName,int _nLine,int _nColumn,const string& _Message);
};

class CgxMsgBufProtobuf :public CgxMsgBufBase
{
public:
	CgxMsgBufProtobuf(void);
	virtual ~CgxMsgBufProtobuf(void);
	int InitProtocol(CgxDBSysMsgList *_pSysMsgList);

	virtual int InitMsg(int _nMsgID,char *_pMsgBuf,int _nMsgLen) ;
	virtual void ReleaseAll()  ;
	virtual int GetHeadLen() ;	
	virtual int WriteDatalog(char*_pBuf,int _nBufLen);										
	virtual int SetData(void *_pData, int _nDataLen,int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetData(int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) ;
	virtual int GetDataLen(int _nMember,int _nAdd,int _nSub) ;
	virtual int GetDataType(int _nMember,int _nAdd,int _nSub);
	
	
	virtual int SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetShortData(short _nData ,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetInt64Data(__int64 _Data,int _nMember,int _nAdd,int _nSub)  ;
	virtual int SetSuffixData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub);

	virtual int GetIntData(int _nMember,int _nAdd,int _nSub);
	virtual short GetShortData(int _nMember,int _nAdd,int _nSub);
	virtual float GetFloatData(int _nMember,int _nAdd,int _nSub);
	virtual double GetDoubleData(int _nMember,int _nAdd,int _nSub) ;
	virtual __int64 GetInt64Data(int _nMember,int _nAdd,int _nSub);
	virtual void* GetSuffixData(int _nMember,int _nAdd,int _nSub);
	virtual int GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetSuffixData(int &_nSuffixLen,int _nMember,int _nAdd,int _nSub);

	virtual int GetDataStr(char *_pstrValue,int &_nValueLen,int _nMember,int _nAdd,int _nSub); 
	int GetHeadInfo(CgxMsgHeadInfo &_Info);

	virtual int BeginAddData();
	virtual int EndAddData(int _nAddCount);
	virtual int GetDataLen() ;
	virtual void * GetData() ;
private:
	GX_MSG *m_pMsg;
	std::string m_strData;
	CgxDBSysMsg *m_pDBSysMsg;
	static CgxDBSysMsgList *m_pSysMsgList;
	static CgxMultiFileErrorCollector m_ErrorCollector;
	static compiler::DiskSourceTree m_SourceTree;

	static compiler::Importer m_Importer ;
	static DynamicMessageFactory * m_dynamicMessageFactory;

	int WriteMsg(GX_MSG *_pMsg);
	string CgxMsgBufProtobuf::GetFieldString(GX_MSG *_pMsg,const FieldDescriptor * _pField);
	const FieldDescriptor * GetProtoBufField(const Descriptor* _pDescriptor,char* _pName);
	const FieldDescriptor * GetProtoBufField(const Descriptor* _pDescriptor,int _nNum);
	GX_MSG* CgxMsgBufProtobuf::CreateMessage(const std::string& _TypeName);
private:
	int MsgToFile(CgxDBSysMsgList *_pSysMsgList,char *_pFileName) ;
	int StructToFile(CgxDBSysMsgList *_pSysMsgList,char *_pFileName) ;
	int StructToFile(CgxList<CgxDBSysStruct> &_StructList,char *_pFileName,CgxDBSysMsgList *_pSysMsgList);
	int StructToFile(CgxDBSysStruct * _pStruct,FILE *_fp,CgxDBSysMsgList *_pSysMsgList) ;
	int FormatMemberStr(char *_pBuf,int _nSize,CgxDBSysMsgMember *_pMember,CgxDBSysMsgList *_pSysMsgList);
	int FormatMemberStr(char *_pBuf,int _nSize,CgxDBSysStruct *_pStruct,CgxDBSysMsgList *_pSysMsgList);
	int GetTypeStr(char *_pStr,int _nLen,int _nType,CgxDBSysMsgList *_pSysMsgList);
	int LoadProtoFile(char *_pFileName);
	int SetField(GX_MSG *_pSubMsg,const FieldDescriptor* _pField,const Reflection* _pReflection,void*_pData,int _nDataLen);
	int GetFieldInfo(const FieldDescriptor ** _ppField,const Reflection** _ppReflection,int _nMember,int _nAdd,int _nSub,GX_MSG **_ppSubMsg);
};

#endif
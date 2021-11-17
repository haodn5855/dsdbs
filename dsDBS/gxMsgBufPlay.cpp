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

#include "gxMsgBufPlay.h"
#include "gxMsgBufByte1.h"
#include "gxDSKernel.h"

CgxMsgBufPlay::CgxMsgBufPlay(void)
{
	m_pMsgBufBase = NULL ;
	m_pHeadBufBase = NULL;
}
CgxMsgBufPlay::CgxMsgBufPlay(DWORD _dwMsgID,void *_pMsg,int _nMsgLen,int _nHeadLen)
{
	m_pMsgBufBase = NULL ;
	Init(_dwMsgID,NULL,_pMsg,_nMsgLen ,_nHeadLen) ;
}
CgxMsgBufPlay::~CgxMsgBufPlay(void)
{
	ReleaseAll() ;
}
void CgxMsgBufPlay::ReleaseAll()
{
	if(m_pMsgBufBase!=NULL) 
		delete m_pMsgBufBase;
	m_pMsgBufBase =NULL ;
	if(m_pHeadBufBase !=NULL ) 
		delete m_pHeadBufBase; 
	m_pHeadBufBase = NULL ;
}
CgxMsgBufBase* CgxMsgBufPlay::Init(DWORD _dwMsgID,CgxDBSysMsg *_pDBMsg,void *_pMsg,int _nMsgLen,int _HeadLen)
{
	if( _pDBMsg ==NULL) 
		return NULL;
	ReleaseAll() ;
	int MsgType = _pDBMsg->m_nMsgBodyProto;  
	
	m_dwMsgID = _dwMsgID ;
	if( _pDBMsg->m_nHeadLen<1)
	{
		gxWriteLog("δ֪����Ϣͷ���ȣ�MsgID=%d(0X%08X) ,MsgType=%d, HeadLen =%d" , 
			_dwMsgID ,_dwMsgID,MsgType,_pDBMsg->m_nHeadLen ) ;
		return NULL ;
	}
	char *pMsgBode = (char*)_pMsg ;
	pMsgBode+=_pDBMsg->m_nHeadLen;
	m_pMsgBufBase = GetBaseOfType (MsgType) ;
	int MsgLen = _nMsgLen - _pDBMsg->m_nHeadLen;
	if( m_pMsgBufBase!=NULL)
		m_pMsgBufBase->InitMsg(_dwMsgID,pMsgBode,MsgLen) ;

	int MsgHeadProto = _pDBMsg->m_nMsgHeadProto;
	int MsgHeadID = _pDBMsg->m_nMsgHeadID;
	if( MsgHeadID ==0)
	{
		MsgHeadID = _pDBMsg->m_nDSMsgID;
		MsgHeadProto = _pDBMsg->m_nMsgBodyProto;
	}
	m_pHeadBufBase = GetBaseOfType(MsgHeadProto) ;
	if( m_pHeadBufBase!= NULL)
		m_pHeadBufBase->InitMsg(MsgHeadID,(char*)_pMsg,_pDBMsg->m_nHeadLen) ;
return m_pMsgBufBase ; 
}
int CgxMsgBufPlay::GetDataStr(char *_pstrValue,int _nValueLen,char *_pszMembers) 
{
	if((_pstrValue == NULL )  || (_pszMembers ==NULL)||(_nValueLen <1))
		return -1 ;
	int nMember=-1;
	int nAdd=-1;
	int nSub=-1;
	int nRet = GetMemberAddSub(_pszMembers,nMember,nAdd,nSub) ;
	if( nRet <0 )
		return -2 ;
return GetDataStr(_pstrValue,_nValueLen ,nMember,nAdd,nSub);
}
int CgxMsgBufPlay::GetDataStr(char *_pstrValue ,int &_nValueLen,int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetDataStr(_pstrValue,_nValueLen ,_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetDataStr(_pstrValue,_nValueLen,0	,_nAdd,_nSub) ;
return 0 ; 
}
CgxMsgBufBase* CgxMsgBufPlay::GetBaseOfType(int _nType)
{
	CgxMsgBufBase * pBufBase = NULL ;
	switch(_nType)
	{
	case DS_MSG_TYPE_NONE	:
		break; 
	case DS_MSG_TYPE_BYTE1	:	
		pBufBase = new CgxMsgBufByte1() ;
		break; 
	case DS_MSG_TYPE_BYTE2	:
		break; 
	case DS_MSG_TYPE_BYTE4	:	
		break; 
	default:
		pBufBase = CgxDSKernel::m_SysInterfaceCfgItems.CreateMsgBuf(_nType);
		break; 
	}
return pBufBase ; 
}
int CgxMsgBufPlay::GetMsgHeadInfo(CgxMsgHeadInfo &_Info,DWORD _dwMsgHeadID,int _nProtoType)
{
	_Info.m_dwMsgHeadID = _dwMsgHeadID ;
	_Info.m_dwMsgHeadProto = _nProtoType;
	int nRet = 0 ;
	switch(_nProtoType)
	{
	case DS_MSG_TYPE_NONE	:
		break; 
	case DS_MSG_TYPE_BYTE1	:	
		{
			CgxMsgBufByte1 *pMsgBufBase = new CgxMsgBufByte1() ;
			if( pMsgBufBase!=NULL ) 
			{
				pMsgBufBase->InitMsg(_dwMsgHeadID,NULL,0) ;
				nRet = pMsgBufBase->GetHeadInfo(_Info) ;
			}
			if( pMsgBufBase!=NULL) 
				delete pMsgBufBase;
			break;
		}
	case DS_MSG_TYPE_BYTE2	:
		break; 
	case DS_MSG_TYPE_BYTE4	:	
		break; 
	default:
 
		{
			CgxMsgBufBase *pMsgBufBase =  CgxDSKernel::m_SysInterfaceCfgItems.CreateMsgBuf(_nProtoType);
			if( pMsgBufBase!=NULL ) 
			{
				pMsgBufBase->InitMsg(_dwMsgHeadID,NULL,0) ;
				nRet = pMsgBufBase->GetHeadInfo(_Info) ;
			}
			if( pMsgBufBase!=NULL) 
				delete pMsgBufBase;
			break;
		}
		break; 
	}
return nRet ; 
}
CgxMsgBufBase* CgxMsgBufPlay::GetBasePtr()
{
return m_pMsgBufBase;
}
int CgxMsgBufPlay::GetMsgTypeOfID(DWORD _dwMsgID)
{
return DS_MSG_TYPE_NONE;
}
int CgxMsgBufPlay::InitProtocol(CgxDBSysMsgList *_pSysMsgList) 
{
return 0 ;
}
int CgxMsgBufPlay::InitMsg(int m_nMsgID,char *_pMsgBuf,int _nMsgLen) 
{
return 0 ;
}
int CgxMsgBufPlay::GetHeadLen()  
{
return 0 ;
}
int CgxMsgBufPlay::m_nDebugInfoFlag=0;
int CgxMsgBufPlay::DebugInfo()
{
	if( m_nDebugInfoFlag)
		WriteDatalog(NULL ,0) ;
return 0 ; 
}
int CgxMsgBufPlay::WriteDatalog(char*_pBuf,int _nBufLen)
{
	if( m_pHeadBufBase!=NULL ) 
		m_pHeadBufBase->WriteDatalog(_pBuf,_nBufLen) ;
	if( m_pMsgBufBase!=NULL ) 
		m_pMsgBufBase->WriteDatalog(_pBuf,_nBufLen) ; 
return 0 ;
}
int CgxMsgBufPlay::SetData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetData(_pData,_nDataLen ,_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetData(_pData,_nDataLen,0,_nAdd,_nSub) ;
return 0 ;
}
void* CgxMsgBufPlay::GetData(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return NULL;
	if(_nMember!=0)
		return m_pMsgBufBase->GetData(_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetData(0,_nAdd,_nSub) ;
return NULL ;
}
void* CgxMsgBufPlay::GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return NULL;
	if(_nMember!=0)
		return m_pMsgBufBase->GetData(_nDataLen ,_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetData(_nDataLen,0,_nAdd,_nSub) ;
return NULL ;
}
int CgxMsgBufPlay::GetDataLen(int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetDataLen(_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetDataLen(0	,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::GetDataType(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetDataType(_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetDataType(0,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetIntData(_nData, _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetIntData(_nData,0	,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::SetShortData(short _nData ,int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetShortData(_nData, _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetShortData(_nData,0,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetFloatData(_fData, _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetFloatData(_fData,0,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetDoubleData(_Data, _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetDoubleData(_Data,0,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::SetInt64Data(__int64 _Data ,int _nMember,int _nAdd,int _nSub)  
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetInt64Data(_Data, _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetInt64Data(_Data,0	,_nAdd,_nSub) ;
return 0 ;
}
int CgxMsgBufPlay::SetSuffixData(void *_pData ,int _nDataLen,int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->SetSuffixData(_pData,_nDataLen,_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->SetSuffixData(_pData ,_nDataLen,0,_nAdd,_nSub) ;
return 0 ;
}

int CgxMsgBufPlay::GetIntData(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetIntData( _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetIntData(0	,_nAdd,_nSub) ;
return 0 ;
}
short CgxMsgBufPlay::GetShortData(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetShortData( _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetShortData(0,_nAdd,_nSub) ;
return 0 ;
}
float CgxMsgBufPlay::GetFloatData(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetFloatData( _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetFloatData(0,_nAdd,_nSub) ;
return 0 ;
}
double CgxMsgBufPlay::GetDoubleData(int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetDoubleData( _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetDoubleData(0,_nAdd,_nSub) ;
return 0 ;
}
__int64 CgxMsgBufPlay::GetInt64Data(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetInt64Data( _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetInt64Data(0,_nAdd,_nSub) ;
return 0 ;
}
void* CgxMsgBufPlay::GetSuffixData(int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return NULL;
	if(_nMember!=0)
		return m_pMsgBufBase->GetSuffixData( _nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetSuffixData(0,_nAdd,_nSub) ;
return NULL ;
}
int CgxMsgBufPlay::GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) 
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	if(_nMember!=0)
		return m_pMsgBufBase->GetSuffixDataLen(_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetSuffixDataLen(0,_nAdd,_nSub) ;
return 0 ;
}
void* CgxMsgBufPlay::GetSuffixData(int &_nSuffixLen,int _nMember,int _nAdd,int _nSub)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return NULL;
	if(_nMember!=0)
		return m_pMsgBufBase->GetSuffixData(_nSuffixLen,_nMember-1,_nAdd,_nSub);
	else
		return m_pHeadBufBase->GetSuffixData(_nSuffixLen ,0	,_nAdd,_nSub) ;
return NULL ;
}
int CgxMsgBufPlay::BeginAddData()
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	m_pHeadBufBase->BeginAddData();
return m_pMsgBufBase->BeginAddData();
}
int CgxMsgBufPlay::EndAddData(int _nAddCount)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
return m_pMsgBufBase->EndAddData(_nAddCount);
}
int CgxMsgBufPlay::GetDataLen()  
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	int HeadLen = m_pHeadBufBase->GetDataLen() ;
	int BodyLen = m_pMsgBufBase->GetDataLen() ;
return HeadLen + BodyLen ;
}
void * CgxMsgBufPlay::GetData()
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return NULL;
	if( m_Buf.GetDataLen() <1) 
		return NULL ;
return m_Buf.GetData() ;
}
int CgxMsgBufPlay::MakeBuf()
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	int Len =0 ;
	int HeadLen = m_pHeadBufBase->GetDataLen() ;
	int BodyLen = m_pMsgBufBase->GetDataLen() ;
	if( HeadLen >0) 
	{
		m_Buf.AddData(m_pHeadBufBase->GetData() , HeadLen) ;
		Len += HeadLen ;
	}
	if( BodyLen >0) 
	{
		m_Buf.AddData(m_pMsgBufBase->GetData() , BodyLen);
		Len+=BodyLen ;
	}
return Len ;
}
int CgxMsgBufPlay::SetHeadLen(int _nLen,int _nMember,int _nAdd,int _nSub,int _nType)
{
	if( (m_pHeadBufBase == NULL ) ||(m_pMsgBufBase==NULL))
		return -1;
	switch(_nType)
	{
	case GX_MSGBUF_DATATYPE_BYTE:
	case GX_MSGBUF_DATATYPE_CHAR:
		{
			BYTE Tmp = (BYTE)_nLen;
			SetData(&Tmp,sizeof(Tmp),_nMember,_nAdd,_nSub);
		}
		break;
	case GX_MSGBUF_DATATYPE_SWORD:
	case GX_MSGBUF_DATATYPE_WORD:
		SetShortData((short)_nLen,_nMember,_nAdd,_nSub);
		break;
	case GX_MSGBUF_DATATYPE_INT:
	case GX_MSGBUF_DATATYPE_UINT:
		SetIntData(_nLen, _nMember,_nAdd,_nSub);
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG:
		SetInt64Data(_nLen, _nMember,_nAdd,_nSub);
		break;
	default:
		return -2;
	break;
	}
return 0 ;	
}
int CgxMsgBufPlay::GetHeadInfo(CgxMsgHeadInfo &_Info)
{

return 0 ; 
}

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

#include <WinSock2.h>
#include "gxMsgBufByte1.h"
#pragma warning(disable:4996)
CgxMemBuf::CgxMemBuf()
{
	m_pData =NULL ;
	m_nDataLen =0;	
	m_nBufLen =MSGBUF_BYTE1_SIZE;			
	m_pData =(char*) malloc(MSGBUF_BYTE1_SIZE);
	memset(m_pData,0,sizeof(char) *MSGBUF_BYTE1_SIZE) ; 
}
CgxMemBuf::~CgxMemBuf()
{
	if( m_pData!=NULL)
		free(m_pData) ;
	m_pData = NULL ;
	m_nDataLen = 0 ; 
	m_nBufLen = 0 ; 
}
char *CgxMemBuf::GetData()
{
return m_pData;
}
int CgxMemBuf::GetBufLen()
{
return m_nBufLen;
}
int CgxMemBuf::GetDataLen()
{
return m_nDataLen;
}
int CgxMemBuf::AddData(void *_pData,int _nLen)
{
	if(( _pData == NULL ) || (_nLen <0))
		return -1 ; 
	if( ! ReallocMem(_nLen +8) )
		return -2 ;
	char * p = m_pData ; 
	p+=m_nDataLen ;
	memcpy(p , _pData , _nLen) ;
	m_nDataLen +=_nLen ;
return 0 ;
}
int CgxMemBuf::InitDataLen(int _nLen)
{
	if( m_nDataLen < _nLen )
		if( ! ReallocMem(_nLen +8) )
			return -2 ;
	m_nDataLen = _nLen;
return m_nDataLen;
}
BOOL CgxMemBuf::ReallocMem(int _nAddSize)
{
	if (m_nBufLen > m_nDataLen +_nAddSize) 
		return TRUE;

	void *pData=(void*)realloc(m_pData,m_nBufLen+_nAddSize);
	if (pData==NULL) 
		return FALSE;

	m_pData=(char*)pData;
	char * p = m_pData ;
	p+=m_nBufLen ;
	memset(p , 0 , sizeof(char) * _nAddSize) ;
	m_nBufLen+=_nAddSize;
return TRUE;
}

CgxByte1MsgList CgxMsgBufByte1::m_Byte1MsgItems;

CgxMsgBufByte1::CgxMsgBufByte1()
{
	m_pByte1Msg = NULL ;
}
CgxMsgBufByte1::~CgxMsgBufByte1(void)
{

}
int CgxMsgBufByte1::Init(CgxDBSysMsgList *_pSysMsgList)
{
	if( _pSysMsgList == NULL ) 
		return -1 ;
	m_Byte1MsgItems.Init(_pSysMsgList) ;
return 0 ; 
}
int CgxMsgBufByte1::InitProtocol(CgxDBSysMsgList *_pSysMsgList) 
{
	if( _pSysMsgList == NULL ) 
		return -1 ;
return Init(_pSysMsgList) ;
}
int CgxMsgBufByte1::InitMsg(int _nMsgID,char *_pMsgBuf,int _nMsgLen) 
{
	m_pByte1Msg = m_Byte1MsgItems.GetMsgOfID(_nMsgID);
	if( m_pByte1Msg == NULL ) 
		return -1; 
	if(( _pMsgBuf!=NULL) && (_nMsgLen>0))
	{
		m_Buf.AddData(_pMsgBuf,_nMsgLen) ;
	}else
	{
		int Len =m_pByte1Msg->GetStaticDataLen() ;
		m_Buf.InitDataLen(Len) ;
	}
return 0 ;
}
void CgxMsgBufByte1::ReleaseAll()
{
}
int CgxMsgBufByte1::GetHeadLen() 
{
	if( m_pByte1Msg!=NULL ) 
		return m_pByte1Msg->GetStaticDataLen() ;
return -1 ; 
}
int CgxMsgBufByte1::GetHeadInfo(CgxMsgHeadInfo &_Info)
{
	if( m_pByte1Msg==NULL ) 
		return -1 ;
	_Info.m_dwHeadLen = m_pByte1Msg->GetStaticDataLen() ; 
	CgxByte1Struct *pStruct = m_pByte1Msg->GetStruct(0) ;
	if( pStruct == NULL ) 
		return -1 ; 
	int Count = pStruct->GetMemberCount() ;
	CgxByte1Member * pMember = NULL ; 
	int InfoCount = 0 ;
	for( int i = 0 ;i < Count ;i ++ ) 
	{
		pMember= pStruct->GetMemberItem(i) ; 
		if( pMember == NULL ) 
			continue ;
		switch(pMember->m_nHeadMemberType) 
		{
		case 1 :
			_Info.m_MemberMsgID.dwOffSet = pMember->m_nOffSet ; 
			_Info.m_MemberMsgID.dwSize   = pMember->m_nSize ;
			InfoCount ++ ;
			break; 
		case 2 :
			_Info.m_MemberMsgLen.dwOffSet = pMember->m_nOffSet ; 
			_Info.m_MemberMsgLen.dwSize   = pMember->m_nSize ;
			InfoCount ++ ;
			break ;
		case 3 :
			_Info.m_MemberSerial.dwOffSet = pMember->m_nOffSet ; 
			_Info.m_MemberSerial.dwSize   = pMember->m_nSize ;
			InfoCount ++ ;
			break; 
		}
	}
return InfoCount ; 
}
int CgxMsgBufByte1::WriteDatalog(char*_pBuf,int _nBufLen)
{
	WriteDataLog() ;
return 0 ;
}
int CgxMsgBufByte1::WriteByte1MsgListLog()
{
	m_Byte1MsgItems.WriteLog() ;
return 0 ; 
}
int CgxMsgBufByte1::WriteDataLog()
{
	if( m_pByte1Msg == NULL ) 
	{
		gxWriteLog(" 未初始化 Byte1Msg!") ;
		return -1 ; 
	}
	gxWriteLog("------------[%d],[%s]--------------------",m_pByte1Msg->m_nID , m_pByte1Msg->m_szMsgName ) ;
	int Count = m_pByte1Msg->GetStructCount() ; 
	CgxByte1Struct * pStruct = NULL ; 
	for( int i = 0 ;i < Count ;i ++ ) 
	{
		pStruct = m_pByte1Msg->GetStruct(i) ; 
		if( pStruct == NULL ) 
			continue ; 
		WriteDataLog(pStruct) ; 
	}
	if( m_pByte1Msg->m_SuffixStructID>0)
	{
		int Count = GetIntData(m_pByte1Msg->m_SuffixCountIdx-1,-1,0) ;
		gxWriteLog("		----- SuffixStructID =%6d,Count=%6d -----",m_pByte1Msg->m_SuffixStructID,Count ) ;

		int MemberCount = m_pByte1Msg->m_SuffixStruct.GetMemberCount() ; 
		char szBuf[1024];
		for( int i = 0 ;i< Count ;i ++ )
		{
			gxWriteLog("			------SuffixStructIdx =%8d------",i) ;
			for( int j = 0 ;j < MemberCount ;j ++ ) 
			{
				CgxByte1Member* pItem = m_pByte1Msg->m_SuffixStruct.GetMemberItem(j) ;
				memset(szBuf , 0 , sizeof(szBuf)) ;
				int DataLen = GetLogStr(szBuf , pItem,-1,i,j) ;
				gxWriteLog("		Record =%3d Sub=%3d ,MemberName=%20s,DataLen = %4d,[%s]",
					i,j,pItem->m_szMemberName , DataLen ,szBuf) ; 
			}
		}
	}
	gxWriteLog("      ") ;
return 0 ; 
}
int CgxMsgBufByte1::GetLogStr(char * _pBuf,void *_pItem,int _nMember,int _nAdd,int _nSub)
{
	CgxByte1Member * pItem = (CgxByte1Member*) _pItem;
	if(( pItem == NULL) || (_pBuf==NULL))
		return -1;
	int DataLen = 0 ;
	int BufLen = 1024;
	switch( pItem->m_nType) 
	{
	case GX_MSGBUF_DATATYPE_CHAR :
	case GX_MSGBUF_DATATYPE_BYTE :
		{
			void *pData =GetData(DataLen,_nMember,_nAdd,_nSub) ;
			BYTE  Value = 0;
			memcpy(&Value , pData , sizeof(Value)) ;
			_snprintf(_pBuf , BufLen, "0x%02x",Value) ;
			DataLen = sizeof(BYTE) ;
		}
	break;
	case GX_MSGBUF_DATATYPE_INT :
	case GX_MSGBUF_DATATYPE_UINT :
		{
			int Value = GetIntData( _nMember, _nAdd, _nSub) ;
			_snprintf(_pBuf , BufLen, "%d",Value) ;
			DataLen = sizeof(int) ;
		}
		break; 
	case  GX_MSGBUF_DATATYPE_SWORD:
	case  GX_MSGBUF_DATATYPE_WORD:
		{
			WORD Value = GetShortData( _nMember, _nAdd, _nSub) ;
			_snprintf(_pBuf ,BufLen , "%d",Value) ;
			DataLen = sizeof(WORD) ;
		}
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG :
		{
			__int64 Value = GetInt64Data( _nMember, _nAdd, _nSub) ;
			_snprintf(_pBuf , BufLen , "%I64d",Value) ;
			DataLen = sizeof(__int64) ;
		}
		break; 
	case  GX_MSGBUF_DATATYPE_FLOAT:
		{
			float Value = GetFloatData( _nMember, _nAdd, _nSub) ;
			_snprintf(_pBuf ,BufLen , "%10.8f",Value) ;
			DataLen = sizeof(float) ;
		}
		break; 
	case  GX_MSGBUF_DATATYPE_DOUBLE:
		{
			double Value = GetDoubleData( _nMember, _nAdd, _nSub) ;
			_snprintf(_pBuf , BufLen , "%10.8f",Value) ;
			DataLen = sizeof(double) ;
		}
		break; 
	case GX_MSGBUF_DATATYPE_SUFFIX :
		{
			void *pData =GetSuffixData(DataLen , _nMember, _nAdd, _nSub) ;
			if( DataLen)
				memcpy(_pBuf,pData , min(1023,DataLen));
		}
		break;
	default :
		{
			void *pData =GetData(DataLen , _nMember, _nAdd, _nSub) ;
			memcpy(_pBuf,pData , min(1023,DataLen));
		}
		break; 
	}
return DataLen ; 
}
int CgxMsgBufByte1::WriteDataLog(CgxByte1Struct *_pStruct)
{
	if(_pStruct == NULL ) 
		return -1 ;
	int Count = _pStruct->GetMemberCount() ; 
	CgxByte1Member * pItem = NULL ;
	char *pData = NULL ; 
	char szBuf[1024];
	for( int i = 0 ;i < Count ;i ++ ) 
	{
		pItem = _pStruct->GetMemberItem(i); 
		if( pItem == NULL ) 
			continue ; 
		int DataLen = 0 ; 
		memset(szBuf, 0 , sizeof(szBuf)) ;
		DataLen = GetLogStr(szBuf , pItem,_pStruct->m_nID-1, -1,i) ;
		gxWriteLog("	Idx=%3d,Sub=%3d ,MemberName=%20s,DataLen = %4d,[%s]",
			_pStruct->m_nID-1,i,pItem->m_szMemberName , DataLen ,szBuf) ; 
	}
return 0 ; 
}
int CgxMsgBufByte1::SetData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub) 
{
	if( _pData == NULL ) 
		return -1 ;

	if(! m_Buf.ReallocMem(_nDataLen))
		return -1; 
		
	if(( _pData == NULL) || (_nDataLen <1))
		return -1 ;
	char * pData = NULL ;
	if( _nMember>=0)	{
		CgxByte1Member * pMember = NULL ;
		if(m_pByte1Msg == NULL ) 
		{
			gxWriteLog("错误！指针为空！%s(%d)",__FILE__,__LINE__) ;
			return -1 ;
		}
		pMember = m_pByte1Msg->GetMember(_nMember,_nAdd,_nSub);
		if( pMember == NULL ) 
			return -1 ;
		if( pMember->m_nType ==GX_MSGBUF_DATATYPE_SUFFIX )
		{

			int OffSet = m_pByte1Msg->GetMemberOffset(_nMember,_nAdd,_nSub) ;
			if(( OffSet <0) || (pMember==NULL))
				return -2 ;
			char * p = m_Buf.GetData() ;
			p+= OffSet ;
			memcpy(p , _pData , _nDataLen);
		}else
		{
			pData = (char*)GetData(_nMember,-1,_nSub);
			if( pData == NULL)
				return -2 ;
			memcpy(pData, _pData , _nDataLen);
		}
	}else
	{
		if( CheckAddBufLen(_nAdd ) <0) 
			return -2 ;
		if( m_pByte1Msg == NULL ) 
			return -1 ;

		int ItemLen = m_pByte1Msg->m_SuffixStruct.GetStaticDataLen() ;
		int OffSet = (_nAdd ) * ItemLen ;
		int FieldOffset =  m_pByte1Msg->GetMemberOffset(-1,0,_nSub);
 
		OffSet += FieldOffset ;
		if( OffSet < 0)
		{
			gxWriteLog("偏移量错误！！！");
			return -2 ;
		}
		pData= m_AddBuf.GetData() ;
		pData+=OffSet ;
		if(m_AddBuf.GetBufLen() < OffSet + _nDataLen)
		{
			gxWriteLog("溢出错误！！！");
			return -3 ;
		}
		memcpy(pData,_pData,_nDataLen) ;
	}
return 0 ; 
}
int CgxMsgBufByte1::CheckAddBufLen(int _nAddIdx)
{
	if(_nAddIdx <0) 
		return -1 ;
	if( m_pByte1Msg == NULL ) 
		return -1 ;
	if(m_pByte1Msg == NULL ) 
	{
		gxWriteLog("错误！指针为空！%s(%d)",__FILE__,__LINE__) ;
		return -1 ;
	}

	int OldBufLen = m_AddBuf.GetBufLen()  ; 
	int ItemLen = m_pByte1Msg->m_SuffixStruct.GetStaticDataLen() ;
	if( ItemLen <1)  
		return -1 ;
	int Len = ItemLen * (_nAddIdx +1);
	if( OldBufLen < Len )
	{
		int AddLen = ItemLen *32 ;
		if( AddLen < 1024 )
			AddLen = 1024;
		m_AddBuf.ReallocMem(AddLen);
	}
	m_AddBuf.InitDataLen(Len);
return 0 ; 
}
void* CgxMsgBufByte1::GetData(int _nMember,int _nAdd,int _nSub) 
{
	int Len =0 ;
return	GetData(Len , _nMember , _nAdd , _nSub) ;
}
void* CgxMsgBufByte1::GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) 
{
	CgxByte1Member * pMember = NULL ;
	if(m_pByte1Msg == NULL ) 
	{
		gxWriteLog("错误！指针为空！%s(%d)",__FILE__,__LINE__) ;
		return NULL ;
	}
	pMember = m_pByte1Msg->GetMember(_nMember,_nAdd,_nSub);

	int OffSet = m_pByte1Msg->GetMemberOffset(_nMember,_nAdd,_nSub) ;
	if(( OffSet <0) || (pMember==NULL))
		return NULL ;
	char * p = m_Buf.GetData() ;
	if(( _nMember<0 )  && (_nAdd >=0))
		OffSet+=m_pByte1Msg->GetStaticDataLen();

	p+=OffSet ;
	_nDataLen = pMember->m_nSize;

	if( pMember->m_nType ==GX_MSGBUF_DATATYPE_SUFFIX )
	{
				MSG_SUFFIX Suf ;
		memset(&Suf , 0,sizeof(Suf)) ;
		memcpy(&Suf , p,sizeof(Suf)) ;

		Suf.dwOffSet = ntohl(Suf.dwOffSet) ;
		Suf.dwSize   = ntohl(Suf.dwSize) ;

		p = m_Buf.GetData() ;
		p+=Suf.dwOffSet;
		_nDataLen = Suf.dwSize;
		
		int BufLen =m_Buf.GetDataLen();
		if(( BufLen < _nDataLen ) || ((UINT)BufLen < Suf.dwOffSet + Suf.dwSize))
		{
			gxWriteLog("数据赋值错误！DataLen =%d,OffSet=%d ,Size=%d.MAS[%d,%d,%d]" ,
				BufLen , Suf.dwOffSet,Suf.dwSize,_nMember,_nAdd,_nSub) ;
			_nDataLen = 0;
		}
	}
return p ;
}
int CgxMsgBufByte1::GetDataLen(int _nMember,int _nAdd,int _nSub) 
{
	int Len =0 ;
	GetData(Len , _nMember , _nAdd , _nSub) ;
	return Len ;
}
int CgxMsgBufByte1::GetDataType(int _nMember,int _nAdd,int _nSub)
{
	CgxByte1Member *pMember = NULL ; 
	if(m_pByte1Msg == NULL ) 
	{
		gxWriteLog("错误！指针为空！%s(%d)",__FILE__,__LINE__) ;
		return -1 ;
	}
	pMember = m_pByte1Msg->GetMember(_nMember,_nAdd,_nSub) ;
	if( pMember!=NULL ) 
		return pMember->m_nType;
return -1;
}
int CgxMsgBufByte1::SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub) 
{
	int nData = htonl(_nData);
return	SetData(&nData,sizeof(nData),_nMember,_nAdd,_nSub);
}
int CgxMsgBufByte1::SetShortData(short _nData,int _nMember,int _nAdd,int _nSub)
{
	short nData = htons(_nData);
return	SetData(&nData,sizeof(nData),_nMember,_nAdd,_nSub);
}
int CgxMsgBufByte1::SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) 
{
return SetData(&_fData ,sizeof(_fData) ,_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufByte1::SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) 
{
return SetData(&_Data , sizeof(_Data) , _nMember,_nAdd,_nSub) ;
}
int CgxMsgBufByte1::SetInt64Data(__int64 _Data ,int _nMember,int _nAdd,int _nSub)  
{
	__int64 Data64 = hton64((unsigned __int64 *)(&_Data));
return SetData(&Data64 , sizeof(Data64),_nMember,_nAdd,_nSub) ;
}
int CgxMsgBufByte1::SetSuffixData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub)
{
	if(( _pData == NULL) || (_nDataLen <= 0))
		return 0 ; 
	CgxByte1Member *pItem = NULL ;
	if(m_pByte1Msg == NULL ) 
	{
		gxWriteLog("错误！指针为空！%s(%d)",__FILE__,__LINE__) ;
		return -1 ;
	}
	pItem = m_pByte1Msg->GetMember(_nMember,_nAdd,_nSub) ; 

	if( pItem == NULL ) 
		return -1 ; 
	if( pItem->m_nType!=GX_MSGBUF_DATATYPE_SUFFIX ) 
	{
		gxWriteLog("类型配置错误！不是后缀结构！ %d:%d" , _nMember , _nSub ) ; 
		return -2 ;
	}
	int Len = m_SuffixBuf.GetDataLen() ;

	char * pData = (char*)m_SuffixBuf.GetData() ;
	if( pData == NULL) 
		return -1 ; 
	m_SuffixBuf.AddData(_pData,_nDataLen) ;

	char szTmp[4];
	memset(szTmp , 0 , sizeof(szTmp)) ;
	m_SuffixBuf.AddData(szTmp , 2) ;

	MSG_SUFFIX  MsgTmp;
	memset(&MsgTmp , 0 , sizeof(MsgTmp));
 
	MsgTmp.dwOffSet = Len;						MsgTmp.dwSize = _nDataLen ;				
	SetData(&MsgTmp , sizeof(MsgTmp),_nMember,_nAdd,_nSub);
return 0 ; 
}
int CgxMsgBufByte1::GetIntData(int _nMember,int _nAdd,int _nSub)
{
	int nValue = 0 ;
	void * pData = GetData(_nMember,_nAdd,_nSub);
	if( pData!= NULL )
	{
		memcpy(&nValue,pData,sizeof(nValue));
	}
	nValue = ntohl(nValue) ;
return nValue ;
}
short CgxMsgBufByte1::GetShortData(int _nMember,int _nAdd,int _nSub)
{
	short nValue = 0 ;
	void * pData = GetData(_nMember,_nAdd,_nSub);
	if( pData!= NULL )
	{
		memcpy(&nValue,pData,sizeof(nValue));
	}
	nValue = ntohs(nValue) ;
return nValue ;
}
float CgxMsgBufByte1::GetFloatData(int _nMember,int _nAdd,int _nSub)
{
	float fValue = 0.0000 ;
	void * pData = GetData(_nMember,_nAdd,_nSub);
	if( pData!= NULL )
	{
		memcpy(&fValue,pData,sizeof(fValue));
	}
return fValue ; 
}
double CgxMsgBufByte1::GetDoubleData(int _nMember,int _nAdd,int _nSub) 
{
	double dValue = 0.0000000000 ;
	void * pData = GetData(_nMember,_nAdd,_nSub);
	if( pData!= NULL )
	{
		memcpy(&dValue,pData,sizeof(dValue));
	}
return dValue ;
}
__int64 CgxMsgBufByte1::GetInt64Data(int _nMember,int _nAdd,int _nSub)
{
	unsigned __int64 Value = 0 ; 
	void * pData = GetData(_nMember,_nAdd,_nSub);
	if( pData!= NULL )
	{
		memcpy(&Value,pData,sizeof(Value));
	}
	Value = ntoh64(&Value) ;
return Value ;	
}
void* CgxMsgBufByte1::GetSuffixData(int _nMember,int _nAdd,int _nSub)
{
	int Len = 0 ;
	void *p = GetSuffixData(Len,_nMember,_nAdd,_nSub) ;
return p;
}
int CgxMsgBufByte1::GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) 
{
	int Len = 0 ;
	GetSuffixData(Len , _nMember,_nAdd,_nSub) ;
return Len;
}
void* CgxMsgBufByte1::GetSuffixData(int &_nSuffixLen,int _nMember,int _nAdd,int _nSub)
{
return GetData(_nSuffixLen,_nMember,_nAdd,_nSub) ;
}
MSG_SUFFIX *CgxMsgBufByte1::GetSuffix(int _nMember,int _nAdd,int _nSub)
{
	MSG_SUFFIX *pSuf = NULL ;
	CgxByte1Member * pMember = NULL ;
	if(m_pByte1Msg == NULL ) 
	{
		gxWriteLog("错误！指针为空！%s(%d)",__FILE__,__LINE__) ;
		return NULL ;
	}

	pMember = m_pByte1Msg->GetMember(_nMember,_nAdd,_nSub);
	int OffSet = m_pByte1Msg->GetMemberOffset(_nMember,_nAdd,_nSub) ;
	if(( _nMember<0) && (_nAdd>=0))
		OffSet += m_pByte1Msg->m_nStaticStructSize;
	if(( OffSet <0) && (pMember==NULL))
		return NULL ;
	char * p = m_Buf.GetData() ;

	p+=OffSet ;

	if( pMember->m_nType ==GX_MSGBUF_DATATYPE_SUFFIX )
	{
		pSuf = (MSG_SUFFIX*)p;
 	
	}
return pSuf;
}
int CgxMsgBufByte1::GetDataStr(char *_pstrValue ,int &_nValueLen,int _nMember,int _nAdd,int _nSub) 
{
	if(( _pstrValue ==NULL)|| (_nValueLen <1) )
		return -1 ;
	int nType = GetDataType(_nMember,_nAdd,_nSub);
	switch(nType)
	{
	case GX_MSGBUF_DATATYPE_CHAR		 :
	case GX_MSGBUF_DATATYPE_BYTE		 :
		{
			void*pData = GetData(_nMember,_nAdd,_nSub);
			BYTE Value =0 ;
			if( pData!=NULL)
			{
				memcpy(&Value,pData,sizeof(Value)) ;
				_snprintf(_pstrValue,_nValueLen,"%d",Value) ;
				_nValueLen =strlen(_pstrValue) ;
			}
			break;
		}
	case GX_MSGBUF_DATATYPE_SWORD	 :
	case GX_MSGBUF_DATATYPE_WORD	 :
		{
			short sData = GetShortData(_nMember,_nAdd,_nSub);
			_snprintf(_pstrValue,_nValueLen,"%d",sData) ;
			_nValueLen =strlen(_pstrValue) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_INT		 :
	case GX_MSGBUF_DATATYPE_UINT		 :
		{
			int nData = GetIntData(_nMember,_nAdd,_nSub);
			_snprintf(_pstrValue,_nValueLen,"%d",nData) ;
			_nValueLen =strlen(_pstrValue) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_STR		 :
		{
			int nLen =0 ;
			void* pData = GetData(nLen,_nMember,_nAdd,_nSub);
			if( pData!=NULL)
			{
				if( nLen >_nValueLen )
					nLen = _nValueLen;
				memcpy(_pstrValue,pData,nLen) ;
				_nValueLen = nLen;
			}
			break;
		}
	case GX_MSGBUF_DATATYPE_RAWSTR	 :
	case GX_MSGBUF_DATATYPE_BINARY	 :
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG	 :
		{
			__int64 Data = GetInt64Data(_nMember,_nAdd,_nSub);
			_snprintf(_pstrValue,_nValueLen,"%I64d",Data) ;
			_nValueLen =strlen(_pstrValue) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_FLOAT	 :
		{
			float fData = GetFloatData(_nMember,_nAdd,_nSub);
			_snprintf(_pstrValue,_nValueLen,"%f",fData) ;
			_nValueLen =strlen(_pstrValue) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_DATE		 :
		break;
	case GX_MSGBUF_DATATYPE_DECIMAL	 :
		break;
	case GX_MSGBUF_DATATYPE_DOUBLE	 :
		{
			double dData = GetDoubleData(_nMember,_nAdd,_nSub);
			_snprintf(_pstrValue,_nValueLen,"%f",dData) ;
			_nValueLen =strlen(_pstrValue) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_SUFFIX	 :
		{
			int nLen =0 ;
			void* pData = GetData(nLen ,_nMember,_nAdd,_nSub);
			if( pData!=NULL)
			{
				if( nLen >_nValueLen )
					nLen = _nValueLen;
				memcpy(_pstrValue , pData , nLen) ;
				_nValueLen = nLen;
			}
			break;
		}
	default:
		break;
	}
return _nValueLen ;
}
int CgxMsgBufByte1::BeginAddData()
{
return 0 ;
}
int CgxMsgBufByte1::EndAddData(int _nAddCount)
{
	if( m_pByte1Msg == NULL ) 
		return -1; 

	if( _nAddCount >0) 
	{
		SetIntData(_nAddCount,m_pByte1Msg->m_SuffixCountIdx -1,-1,0) ;
				m_Buf.AddData(m_AddBuf.GetData() , m_AddBuf.GetDataLen()) ;
	}
		int Len =m_Buf.GetDataLen() ;   
	if( m_SuffixBuf.GetDataLen() >0)
	{
				m_Buf.AddData(m_SuffixBuf.GetData() , m_SuffixBuf.GetDataLen()) ;
	}
	int StructCount = m_pByte1Msg->GetStructCount() ;	
	CgxByte1Struct * pStruct = NULL ; 
	for ( int i = 0; i < StructCount;i ++ )
	{
		pStruct = m_pByte1Msg->GetStruct(i) ; 
		if( pStruct == NULL ) 
			continue ; 
		int MemberCount = pStruct->GetMemberCount() ; 
		for( int j = 0 ;j < MemberCount ;j ++ ) 
		{
			CgxByte1Member *pMember= pStruct->GetMemberItem(j) ;
			if( pMember == NULL ) 
				continue ;
			if( pMember->m_nType != GX_MSGBUF_DATATYPE_SUFFIX )
				continue ;
			MSG_SUFFIX  *pSuf =GetSuffix( i ,-1, j);
			if( pSuf == NULL ) 
				continue ;
			pSuf->dwOffSet +=Len;
			pSuf->dwOffSet = htonl(pSuf->dwOffSet) ;
			pSuf->dwSize   = htonl(pSuf->dwSize) ;	
		}
	}
		if(( m_pByte1Msg->m_SuffixStruct.GetMemberCount()>0 ) && (m_AddBuf.GetDataLen() >0  ))
	{
		if( m_AddBuf.GetDataLen() % m_pByte1Msg->m_SuffixStruct.GetStaticDataLen() != 0) 
		{
			gxWriteLog("后缀的结构长度计算错误! MsgID = %d ,StructID = %d" , m_pByte1Msg->m_nID , m_pByte1Msg->m_SuffixStruct.m_nID) ;
		}
		int AddCount = m_AddBuf.GetDataLen()  /m_pByte1Msg->m_SuffixStruct.GetStaticDataLen() ;
		int Offset = m_pByte1Msg->GetStaticDataLen() ; 
		int ItemLen = m_pByte1Msg->m_SuffixStruct.GetStaticDataLen() ;
		if( m_pByte1Msg->m_SuffixStruct.GetMemberCount() == 0) 
		{
			gxWriteLog("配置错误!后缀的结构项为空") ;
			return -3;
		}
		int ItemCount = m_pByte1Msg->m_SuffixStruct.GetMemberCount() ;
		for( int i = 0 ;i < AddCount ;i ++ ) 
		{
			for( int j = 0 ;j < ItemCount ;j ++ ) 
			{
				CgxByte1Member *pMember = m_pByte1Msg->m_SuffixStruct.GetMemberItem(j) ;
				if( pMember == NULL ) 
					continue ;
				if( pMember->m_nType != GX_MSGBUF_DATATYPE_SUFFIX) 
					continue ; 

				MSG_SUFFIX  *pSuf =GetSuffix( -1, i,j );
				if( pSuf == NULL ) 
					continue ;
				pSuf->dwOffSet +=Len;
				pSuf->dwOffSet = htonl(pSuf->dwOffSet) ;
				pSuf->dwSize   = htonl(pSuf->dwSize) ;	
			}
			Offset += ItemLen ;
		}
	}
return 0 ;
}
int CgxMsgBufByte1::GetDataLen() 
{
return	m_Buf.GetDataLen() ;
}
void * CgxMsgBufByte1::GetData() 
{
return	m_Buf.GetData();
}

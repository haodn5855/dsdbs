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
#include "gxDSApp.h"
#include "gxDSKernel.h"
CgxDSKernel g_DSKernel;


CgxDSApp::CgxDSApp()
{
}

CgxDSApp::~CgxDSApp()
{
	g_DSKernel.ReleaseAll() ;
}

void CgxDSApp::ReleaseAll() 
{
	g_DSKernel.ReleaseAll();
}
int CgxDSApp::Init(int _nDBType,char *_pDBName,char *_pDsnName,char *_pIP,int _nPort,int _nCnntCount,char*_pUser,char *_pPass)
{
return g_DSKernel.Init(_nDBType,_pDBName,_pDsnName,_pIP,_nPort,_nCnntCount,_pUser,_pPass);
}
int CgxDSApp::OnMsg(DWORD _dwMsgID ,string _Req , string &_Ack) 
{
return g_DSKernel.OnMsg( _dwMsgID , _Req , _Ack);
}
void CgxDSApp::SetDebug(BOOL _bEnable)
{
	g_DSKernel.SetDebug( _bEnable);
}
void CgxDSApp::SetDSSVRID(int _nDSSVRID)
{
	g_DSKernel.SetDSSVRID( _nDSSVRID);
}
int CgxDSApp::GetHeadLen(DWORD _dwProtoType,DWORD _dwMsgID) 
{
return	g_DSKernel.GetHeadLen(_dwProtoType,_dwMsgID) ;
 
}
int CgxDSApp::GetMsgLen(void *_pMsgHeadData,int _nDataLen,DWORD _dwProtoType,DWORD _dwMsgID) 
{
return g_DSKernel.GetMsgLen(_pMsgHeadData,_nDataLen,_dwProtoType,_dwMsgID);
}
DWORD CgxDSApp::GetMsgType(void *_pMsgHeadData,int _nDataLen,DWORD _dwProtoType,DWORD _dwMsgID) 
{
return g_DSKernel.GetMsgType(_pMsgHeadData ,_nDataLen,_dwProtoType,_dwMsgID) ;
}

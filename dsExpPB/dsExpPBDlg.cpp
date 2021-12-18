
// dsExpPBDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dsExpPB.h"
#include "dsExpPBDlg.h"
#include "afxdialogex.h"
#include "gxPBInterface.h"
#include "gxThreadClient.h"
#include "DSMsg.pb.h"
#include "gxExpMsgPbDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdsExpPBDlg �Ի���




CdsExpPBDlg::CdsExpPBDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdsExpPBDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdsExpPBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdsExpPBDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CdsExpPBDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdsExpPBDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CdsExpPBDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CdsExpPBDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CdsExpPBDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CdsExpPBDlg ��Ϣ�������

BOOL CdsExpPBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdsExpPBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CdsExpPBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
CgxThreadClient g_Client ;

void CdsExpPBDlg::ExpSelect()
{
	InterfaceSelectReq Req ;
	Req.set_dssvrid(1) ;

	EXP_HEAD Head;
	memset(&Head, 0 , sizeof(Head)) ;
	Head.MsgFlag = 0x01020304;
	Head.MsgID = EXP_MSG_PB_REQ_SELECT;
	Head.MsgLen = sizeof(Head) ; 
	Head.MsgSerial = GetTickCount() ;;
	char szSendBuf[2048];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ;
	char * p = szSendBuf ;
	p+=sizeof(Head) ;
	int DataLen = Req.ByteSize() ;
	bool f =Req.SerializeToArray(p ,sizeof(szSendBuf)- sizeof(Head)) ;

	int SendLen = sizeof(Head) ;
	SendLen += DataLen ;
	Head.MsgLen = SendLen ;
	Head.hton() ;
	memcpy(szSendBuf ,&Head, sizeof(Head)) ;
	g_Client.SendMsg(szSendBuf ,SendLen) ;
}
void CdsExpPBDlg::ExpInsert()
{	
	InterfaceInsertReq Req ;
	for( int i =0 ;i < 2 ;i ++ ) 
	{
		SysInterfaceStruct * pStruct =Req.add_cfgitems() ;
		pStruct->set_interfaceid(GetTickCount() %1000 +2) ;
		pStruct->set_dssvrid(1) ;
		pStruct->set_ifname("Name");
		pStruct->set_ifpath("Path") ;
		pStruct->set_isenable(0) ;
	}

	EXP_HEAD Head;
	memset(&Head, 0 , sizeof(Head)) ;
	Head.MsgFlag = 0x01020304;
	Head.MsgID = EXP_MSG_PB_REQ_INSERT;
	Head.MsgLen = sizeof(Head) ; 
	Head.MsgSerial = GetTickCount() ;;
	char szSendBuf[2048];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ;
	char * p = szSendBuf ;
	p+=sizeof(Head) ;
	int DataLen = Req.ByteSize() ;
	bool f =Req.SerializeToArray(p ,sizeof(szSendBuf)- sizeof(Head)) ;

	int SendLen = sizeof(Head) ;
	SendLen += DataLen ;
	Head.MsgLen = SendLen ;
	Head.hton() ;
	memcpy(szSendBuf ,&Head, sizeof(Head)) ;
	g_Client.SendMsg(szSendBuf ,SendLen) ;
}
void CdsExpPBDlg::ExpUpdate()
{	
	InterfaceUpdateReq Req ;
	for( int i =0 ;i < 2 ;i ++ ) 
	{
		SysInterfaceStruct * pStruct =Req.add_cfgitems() ;
		pStruct->set_interfaceid(2) ;
		pStruct->set_dssvrid(1) ;
		pStruct->set_ifname("Name");
		pStruct->set_ifpath("Path") ;
		pStruct->set_isenable(0) ;
	}

	EXP_HEAD Head;
	memset(&Head, 0 , sizeof(Head)) ;
	Head.MsgFlag = 0x01020304;
	Head.MsgID = EXP_MSG_PB_REQ_UPDATE;
	Head.MsgLen = sizeof(Head) ; 
	Head.MsgSerial = GetTickCount() ;;
	char szSendBuf[2048];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ;
	char * p = szSendBuf ;
	p+=sizeof(Head) ;
	int DataLen = Req.ByteSize() ;
	bool f =Req.SerializeToArray(p ,sizeof(szSendBuf)- sizeof(Head)) ;

	int SendLen = sizeof(Head) ;
	SendLen += DataLen ;
	Head.MsgLen = SendLen ;
	Head.hton() ;
	memcpy(szSendBuf ,&Head, sizeof(Head)) ;
	g_Client.SendMsg(szSendBuf ,SendLen) ;
}
void CdsExpPBDlg::ExpDelete()
{
	InterfaceDeleteReq Req ;
	for( int i =0 ;i < 2 ;i ++ ) 
	{
		SysInterfaceStruct * pStruct =Req.add_cfgitems() ;
		pStruct->set_interfaceid(2) ;
		pStruct->set_dssvrid(1) ;
		pStruct->set_ifname("Name");
		pStruct->set_ifpath("Path") ;
		pStruct->set_isenable(0) ;
	}

	EXP_HEAD Head;
	memset(&Head, 0 , sizeof(Head)) ;
	Head.MsgFlag = 0x01020304;
	Head.MsgID = EXP_MSG_PB_REQ_DELETE;
	Head.MsgLen = sizeof(Head) ; 
	Head.MsgSerial = GetTickCount() ;;
	char szSendBuf[2048];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ;
	char * p = szSendBuf ;
	p+=sizeof(Head) ;
	int DataLen = Req.ByteSize() ;
	bool f =Req.SerializeToArray(p ,sizeof(szSendBuf)- sizeof(Head)) ;

	int SendLen = sizeof(Head) ;
	SendLen += DataLen ;
	Head.MsgLen = SendLen ;
	Head.hton() ;
	memcpy(szSendBuf ,&Head, sizeof(Head)) ;
	g_Client.SendMsg(szSendBuf ,SendLen) ;
}
void CdsExpPBDlg::OnBnClickedButton1()
{
	g_Client.m_Wnd = m_hWnd ;
	g_Client.StartThread() ;
}
void CdsExpPBDlg::OnBnClickedButton2()
{
	ExpSelect();
}
void CdsExpPBDlg::OnBnClickedButton3()
{
	ExpInsert();
}
void CdsExpPBDlg::OnBnClickedButton4()
{
	ExpUpdate();
}
void CdsExpPBDlg::OnBnClickedButton5()
{
	ExpDelete();
}

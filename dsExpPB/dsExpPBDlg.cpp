
// dsExpPBDlg.cpp : 实现文件
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


// CdsExpPBDlg 对话框




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


// CdsExpPBDlg 消息处理程序

BOOL CdsExpPBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdsExpPBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

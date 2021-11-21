
// dsExpSvrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "dsExpSvr.h"
#include "dsExpSvrDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "gxDSApp.h"

#include "gxThread.h"
#include "gxThreadAccept.h"
#include "gxThreadRecv.h"
#include "gxVirBusiness.h"

// CdsExpSvrDlg 对话框




CdsExpSvrDlg::CdsExpSvrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdsExpSvrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdsExpSvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdsExpSvrDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CdsExpSvrDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdsExpSvrDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CdsExpSvrDlg 消息处理程序

BOOL CdsExpSvrDlg::OnInitDialog()
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

void CdsExpSvrDlg::OnPaint()
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
HCURSOR CdsExpSvrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CgxDSApp g_DSApp;

CgxThreadRecv g_ThreadRecv ; 
CgxThreadAccept g_ThreadAccept ;
CgxListObj g_Recv ;	
CgxListObj g_Send ;	
int g_VirBusinCount =6;
CgxVirBusiness * g_Business= NULL ;

void CdsExpSvrDlg::OnBnClickedButton1()
{
	AfxOleInit() ;
	AfxSocketInit() ;
	g_Business = new CgxVirBusiness[g_VirBusinCount];
	for( int i = 0 ;i < g_VirBusinCount ;i ++) 
	{
		g_Business[i].SetList(NULL,&g_Recv,&g_Send) ;
		g_Business[i].StartThread() ; 
	}

	g_ThreadRecv.SetList(NULL,&g_Recv,&g_Send) ;
	g_ThreadAccept.m_Port = 6666 ; 
	g_ThreadAccept.SetThreadRecv(&g_ThreadRecv) ; 	

	g_ThreadRecv.StartThread() ;
	g_ThreadAccept.StartThread(); 

	// dsDBS Init
//	DSApp.SetDSSVRID(1);
	g_DSApp.SetDebug() ;
	int nRet = g_DSApp.Init(10001,"dssysdb","dssysdb","",3306,3,"dsuser","dspass123");
	if( nRet <0)
	{
		AfxMessageBox("初始化失败！");
		OnBnClickedButton2();
	}
}

void CdsExpSvrDlg::OnBnClickedButton2()
{
	if( g_Business!= NULL ) 
		for( int i = 0 ;i < g_VirBusinCount ;i ++) 
			g_Business[i].StopThread() ; 

	g_ThreadAccept.StopThread(0) ; 
	g_ThreadRecv.StopThread(0) ; 

	g_ThreadAccept.StopThread() ; 
	g_ThreadRecv.StopThread() ; 	
}

BOOL CdsExpSvrDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedButton2();
	return CDialogEx::DestroyWindow();
}

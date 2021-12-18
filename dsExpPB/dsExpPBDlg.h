
// dsExpPBDlg.h : 头文件
//

#pragma once


// CdsExpPBDlg 对话框
class CdsExpPBDlg : public CDialogEx
{
// 构造
public:
	CdsExpPBDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DSEXPPB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
public:
	void ExpSelect();
	void ExpInsert();
	void ExpUpdate();
	void ExpDelete();
};

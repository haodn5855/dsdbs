
// dsExpPB.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdsExpPBApp:
// �йش����ʵ�֣������ dsExpPB.cpp
//

class CdsExpPBApp : public CWinApp
{
public:
	CdsExpPBApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CdsExpPBApp theApp;

// tld_track.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctld_trackApp: 
// �йش����ʵ�֣������ tld_track.cpp
//

class Ctld_trackApp : public CWinApp
{
public:
	Ctld_trackApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctld_trackApp theApp;
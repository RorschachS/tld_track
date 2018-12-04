
// tld_track.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <string>
#include <windows.h>

using namespace std;

// Ctld_trackApp: 
// 有关此类的实现，请参阅 tld_track.cpp
//

class Ctld_trackApp : public CWinApp
{
public:
	Ctld_trackApp();
private:
	string GetCPUID();
	int getMAC(char * mac);
	string readLicense(string file);
	std::string TCHAR2STRING(TCHAR* str);
	void sendmail(char* msg);
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern Ctld_trackApp theApp;
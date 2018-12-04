// license_warning.cpp : 实现文件
//

#include "stdafx.h"
#include "tld_track.h"
#include "license_warning.h"
#include "afxdialogex.h"


// license_warning 对话框

IMPLEMENT_DYNAMIC(license_warning, CDialogEx)

license_warning::license_warning(CWnd* pParent /*=NULL*/)
	: CDialogEx(LICENSE_WARNING, pParent)
{

}

license_warning::~license_warning()
{
}

void license_warning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(license_warning, CDialogEx)
END_MESSAGE_MAP()


// license_warning 消息处理程序

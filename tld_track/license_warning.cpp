// license_warning.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tld_track.h"
#include "license_warning.h"
#include "afxdialogex.h"


// license_warning �Ի���

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


// license_warning ��Ϣ�������

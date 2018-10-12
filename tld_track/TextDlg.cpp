// TextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tld_track.h"
#include "TextDlg.h"
#include "afxdialogex.h"


// TextDlg 对话框



IMPLEMENT_DYNAMIC(TextDlg, CDialogEx)

TextDlg::TextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(TEXT_DIALOG, pParent)
{

}

TextDlg::~TextDlg()
{
}

void TextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TextDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &TextDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// TextDlg 消息处理程序


void TextDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(IDC_EDIT1)->GetWindowText(get_text);
	CDialogEx::OnOK();
}

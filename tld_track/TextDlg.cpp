// TextDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tld_track.h"
#include "TextDlg.h"
#include "afxdialogex.h"


// TextDlg �Ի���



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


// TextDlg ��Ϣ�������


void TextDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	GetDlgItem(IDC_EDIT1)->GetWindowText(get_text);
	CDialogEx::OnOK();
}

// LabelManager.cpp : 实现文件
//

#include "stdafx.h"
#include "tld_track.h"
#include "LabelManager.h"
#include "afxdialogex.h"


// LabelManager 对话框

IMPLEMENT_DYNAMIC(LabelManager, CDialogEx)

LabelManager::LabelManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

LabelManager::~LabelManager()
{
}

void LabelManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCVSLISTBOX1, MFC_Label_lst);
}


BEGIN_MESSAGE_MAP(LabelManager, CDialogEx)
END_MESSAGE_MAP()


// LabelManager 消息处理程序

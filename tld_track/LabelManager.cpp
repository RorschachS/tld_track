// LabelManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tld_track.h"
#include "LabelManager.h"
#include "afxdialogex.h"


// LabelManager �Ի���

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


// LabelManager ��Ϣ�������

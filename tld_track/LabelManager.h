#pragma once
#include "afxvslistbox.h"


// LabelManager �Ի���

class LabelManager : public CDialogEx
{
	DECLARE_DYNAMIC(LabelManager)

public:
	LabelManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LabelManager();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CVSListBox MFC_Label_lst;
};

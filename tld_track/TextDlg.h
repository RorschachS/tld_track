#pragma once


// TextDlg �Ի���

class TextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TextDlg)

public:
	TextDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TextDlg();
	CString get_text;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = TEXT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

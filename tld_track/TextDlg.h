#pragma once


// TextDlg 对话框

class TextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TextDlg)

public:
	TextDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TextDlg();
	CString get_text;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = TEXT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

#pragma once


// license_warning 对话框

class license_warning : public CDialogEx
{
	DECLARE_DYNAMIC(license_warning)

public:
	license_warning(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~license_warning();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = LICENSE_WARNING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

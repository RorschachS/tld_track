#pragma once


// license_warning �Ի���

class license_warning : public CDialogEx
{
	DECLARE_DYNAMIC(license_warning)

public:
	license_warning(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~license_warning();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = LICENSE_WARNING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

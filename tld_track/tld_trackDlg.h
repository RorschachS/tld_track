
// tld_trackDlg.h : ͷ�ļ�
//

#pragma once


// Ctld_trackDlg �Ի���
class Ctld_trackDlg : public CDialogEx
{
// ����
public:
	Ctld_trackDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLD_TRACK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSavepath();
	afx_msg void OnBnClickedAddlabel();
	afx_msg void OnBnClickedDeletelabel();
	afx_msg void OnBnClickedMovelabel();
};

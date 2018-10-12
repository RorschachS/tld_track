
// tld_trackDlg.h : ͷ�ļ�
//

#pragma once

#include "AMT.h"
#include <cv.h>
#include <cxcore.h>

#include <stdio.h>
#include <time.h>

#include <iostream>
#include <io.h>
#include <fstream>
#include<string>
#include <direct.h>
#include <conio.h>
#include <Windows.h>
#include<vector>
#include "tinyxml2.h"
#include<opencv2\highgui.hpp>
#include<opencv\cxcore.h>
#include<cvaux.h>
#include<cv.hpp>


#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

#define window_name "src1"
#define MAX_OBJ_TRACKING 20
#define MIN_RECT_WIDTH 10
#define MIN_RECT_HEIGHT 10



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

public:	//�Զ��庯��
	void run_tld();
	CString FicowGetDirectory();
	void DrawPicToHDC(IplImage * img, UINT ID);


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
	afx_msg void OnBnClickedAbort();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
};

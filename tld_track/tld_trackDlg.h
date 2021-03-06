
// tld_trackDlg.h : 头文件
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
#include "afxwin.h"
#include "afxcmn.h"
#include "afxbutton.h"

using namespace std;

#define window_name "src1"
#define MAX_OBJ_TRACKING 20
#define MIN_RECT_WIDTH 10
#define MIN_RECT_HEIGHT 10
#define ADD 0
#define DELET 1
#define ADJUST 2


// Ctld_trackDlg 对话框
class Ctld_trackDlg : public CDialogEx
{
// 构造
public:
	Ctld_trackDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLD_TRACK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:	//自定义函数
	//void run_tld();
	//DWORD _stdcall run_tld(LPVOID lpParameter);

	CString FicowGetDirectory();
	void DrawPicToHDC(IplImage * img, UINT ID);
	

// 实现
protected:
	HICON m_hIcon;
	HANDLE m_hThread;

	// 生成的消息映射函数
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
	afx_msg void OnBnClickedAbort();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedAdjustlabel();
	// 按钮显示内容
	CButton m_Enble;

	CComboBox LabelList;	
	afx_msg void OnCbnSelchangeLabelBox();
	CMFCButton mfcBtnColor;
	afx_msg void OnBnClickedMfcbuttonColor();
	afx_msg void OnStnClickedStaticLabel();
};

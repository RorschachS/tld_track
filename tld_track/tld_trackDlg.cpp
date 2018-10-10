
// tld_trackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tld_track.h"
#include "tld_trackDlg.h"
#include "afxdialogex.h"

typedef struct RectArray
{
	CvRect RectElement;
	int iIsActive;
	int iIsNew;
	int iObjID;



	int pos[4] = {};
	//Mat img;
	string label = "";
	bool isAnnotated;
	string path = "";
	string name = "";
	RectArray()   //构造函数
	{
		isAnnotated = false;
	}

}
RectArray;

typedef struct RectArray_List
{
	RectArray mRectArrayList[MAX_OBJ_TRACKING];
	int iTotal;
	int iNewRectGenerated;
}RectArray_List;
RectArray_List* pRectList = new RectArray_List;

typedef struct Line_type
{
	CvScalar LineColor[3];
	int iThickness[3];
}Line_type;
Line_type LineTypeList;

IplImage* CapImg = NULL;
IplImage* FrameImg = NULL;
IplImage* FrameGray = NULL;
IplImage* TempImg = NULL;
clock_t TimeStamp;

int track_object = 0;
int object_num = 0;
CvRect track_window;
CvRect sROI;
CvBox2D track_box;
CvConnectedComp track_comp;
int backproject_mode = 0;
CvPoint pre_pt, end_pt;
CvRect SelectRect;


inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

using namespace std;
using namespace tinyxml2;
using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



void On_Mouse(int event, int x, int y, int flags, void* param);



// add by Lu Dai



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ctld_trackDlg 对话框



Ctld_trackDlg::Ctld_trackDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TLD_TRACK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctld_trackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctld_trackDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(Btn_Open, &Ctld_trackDlg::OnBnClickedOpen)
	ON_BN_CLICKED(Btn_SavePath, &Ctld_trackDlg::OnBnClickedSavepath)
	ON_BN_CLICKED(Btn_AddLabel, &Ctld_trackDlg::OnBnClickedAddlabel)
	ON_BN_CLICKED(Btn_DeleteLabel, &Ctld_trackDlg::OnBnClickedDeletelabel)
	ON_BN_CLICKED(Btn_MoveLabel, &Ctld_trackDlg::OnBnClickedMovelabel)
END_MESSAGE_MAP()


// Ctld_trackDlg 消息处理程序

BOOL Ctld_trackDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Ctld_trackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ctld_trackDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ctld_trackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Ctld_trackDlg::run_tld()
{
	//cvNamedWindow("avi");
	char* filename = "D:\\yolo\\videos\\test_video1.avi";
	CvCapture* pCapture = cvCreateFileCapture(filename);
	//CvCapture* pCapture = cvCaptureFromCAM(0);

	EvAMT Amt;
	int FrameCount;
	CvRect initBox;

	if (!pCapture)
	{
		fprintf(stderr, "pCapture is Null!\n");
		return;
	}
	int iScale = 1.0;
	double BeginFrameIdx = 0;
	int iID = 0;

	FrameCount = (int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);// 视频总帧数
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_POS_FRAMES, BeginFrameIdx);//从第三帧开始

	AmtStructInit(&Amt);
	AmtAlgoParam Param = AmtParamInit(24, 5, 0, 0.1, 15, true);

	char* szChannleID = "AMT-1";
	CvPoint* pPoints;
	int iPointNum;

	if (!(CapImg = cvQueryFrame(pCapture)))
	{
		fprintf(stderr, "pFrame is Null!\n");
		return;
	}

	if (FrameImg == NULL)
	{
		FrameImg = cvCreateImage(cvSize(CapImg->width / iScale, CapImg->height / iScale), CapImg->depth, CapImg->nChannels);
		FrameGray = cvCreateImage(cvGetSize(FrameImg), FrameImg->depth, 1);
		TempImg = cvCreateImage(cvGetSize(FrameImg), FrameImg->depth, FrameImg->nChannels);
		AmtSetConfig(&Amt, szChannleID, FrameImg->width, FrameImg->height, Param, NULL);
	}

	int iKey = 0, frameNum = 0;
	CvFont font = cvFont(1, 1);   // init font
								  //char text[256];
	cvNamedWindow(window_name, 1);
	cvSetMouseCallback(window_name, On_Mouse);
	bool bPause = false; // 暂停标识
	int iPauseKey;
	double dProcTime = 0, dTime;
	int iProcCount = 0;
	for (;;)  //无限循环直到按下“ESC”退出
	{
#if 0
		cvWaitKey(100); //将视频变慢
#endif
		if (frameNum == 1)
		{
			cvWaitKey(0);
		}
		if (!(CapImg = cvQueryFrame(pCapture)))
		{
			fprintf(stderr, "pFrame is Null!\n");
			break;
		}
		cvResize(CapImg, FrameImg);

		//cvFlip(FrameImg);

		cvCvtColor(FrameImg, FrameGray, CV_RGB2GRAY);

		iKey = cvWaitKey(10);  // "ESC"
		if (iKey == 27)
		{
			break;
		}

		frameNum++;

		if (pRectList->iNewRectGenerated == 1) // if there is new rectangle generated 
		{
			for (int i = 0; i < MAX_OBJ_TRACKING; i++)
			{
				if ((pRectList->mRectArrayList[i].iIsNew == 1) && (pRectList->mRectArrayList[i].iIsActive == 1))
				{
					TimeStamp = clock();
					pRectList->mRectArrayList[i].iObjID = iID;  //将ID 付给这个框   ID是连接跟踪目标和框的唯一标识

					AmtCreateObject(&Amt, pRectList->mRectArrayList[i].RectElement, iID++, (long long)TimeStamp);
					pRectList->mRectArrayList[i].iIsNew = -1;
				}
			}
			object_num++;
			pRectList->iNewRectGenerated = -1;
		}

		dTime = GetTickCount();
		// ======================  TLD处理每一帧  ================================
		AmtExecute(&Amt, FrameGray, (long long)time(NULL) * 1000, AMT_ALL_AROUND);//此处如何给定初始位置
		dTime = GetTickCount() - dTime;
		dProcTime += dTime;
		iProcCount++;

		fprintf(stdout, "帧数: %d, time = %g ms\r", frameNum, dTime);

		int boundLineY = 2;
		int boundLineX = 2;
		// 销毁过线目标
		for (int i = 0; i < MAX_OBJECT_NUM; i++)
		{
			if (Amt.m_ObjStatus[i].mStatus) //如果目标处于激活状态
			{
				int iObjID = Amt.m_ObjStatus[i].mID;
				int iCountMiss = Amt.m_ObjStatus[i].mCountMiss;
				int iUnStableN = Amt.m_ObjStatus[i].mUnstableNum;
				CvPoint &mPos = Amt.m_ObjStatus[i].mPos;
				if (iCountMiss > 2 || iUnStableN > 4 || mPos.y < boundLineY || mPos.x < boundLineX || mPos.x > FrameImg->width - boundLineX)
				{
					int iRectIndex = -1;
					for (int j = 0; j < MAX_OBJ_TRACKING; j++)
					{
						if (pRectList->mRectArrayList[j].iObjID == iObjID)
						{
							iRectIndex = j;
							break;
						}
					}
					if (iRectIndex > 0)
					{
						pRectList->mRectArrayList[iRectIndex].iIsActive = -1;
						pRectList->mRectArrayList[iRectIndex].iIsNew = -1;
						pRectList->mRectArrayList[iRectIndex].iObjID = -1;
						pRectList->mRectArrayList[iRectIndex].RectElement.x = -1;
						pRectList->mRectArrayList[iRectIndex].RectElement.y = -1;
						pRectList->mRectArrayList[iRectIndex].RectElement.width = -1;
						pRectList->mRectArrayList[iRectIndex].RectElement.height = -1;
					}

					AmtCleanObject(&Amt, iObjID);




					continue;
				}

				//绘制目标轨迹
				AmtGetObjTrajectory(&Amt, iObjID, &pPoints, &iPointNum);

				for (int k = 0; k < iPointNum - 1; ++k)
				{
					cvLine(FrameImg, cvPoint(pPoints[k].x, pPoints[k].y), cvPoint(pPoints[k + 1].x, pPoints[k + 1].y), LineTypeList.LineColor[iObjID % 3], LineTypeList.iThickness[iObjID % 3]);
				}
				CvRect* pEndRect = &(Amt.m_ObjStatus[i].mBbox);
				cvRectangle(FrameImg, cvPoint(pEndRect->x, pEndRect->y), cvPoint(pEndRect->x + pEndRect->width, pEndRect->y + pEndRect->height), cvScalar(0, 255, 0), 1);
				pRectList->mRectArrayList[i].pos[0] = pEndRect->x;
				pRectList->mRectArrayList[i].pos[1] = pEndRect->y;
				pRectList->mRectArrayList[i].pos[2] = pEndRect->x + pEndRect->width;
				pRectList->mRectArrayList[i].pos[3] = pEndRect->y + pEndRect->height;

				//				cvPolyLine(FrameImg, &pPoints, &iPointNum, 1, 0, CV_RGB(0,255,0), 1);
			}
		}
		cvLine(FrameImg, cvPoint(boundLineX, 0), cvPoint(boundLineX, FrameImg->height - 1), cvScalar(255), 2, 8, 0);
		cvLine(FrameImg, cvPoint(FrameImg->width - boundLineX, 0), cvPoint(FrameImg->width - boundLineX, FrameImg->height - 1), cvScalar(255), 2, 8, 0);
		cvLine(FrameImg, cvPoint(0, boundLineY), cvPoint(FrameImg->width - 1, boundLineY), cvScalar(255), 2, 8, 0);
		cvShowImage(window_name, FrameImg);
		if (bPause == false)
			iPauseKey = cvWaitKey(10);
		else
			iPauseKey = cvWaitKey(0);

		// 触发暂停  P或F1
		if (80 == iPauseKey || 112 == iPauseKey)
			bPause = true;
		else
			bPause = false;


		tinyxml2::XMLDocument xmlDoc;
		//size_t cnt = 0;
		//for (int j = 0; j < MAX_OBJ_TRACKING; ++j)
		//{
		//	if (pRectList->mRectArrayList[j].isAnnotated)
		//	{
		//		cnt++;
		//pRectList->mRectArrayList[frameNum].name = to_string(frameNum);
		char name[6];
		sprintf(name, "%06d", frameNum);

		XMLNode * annotation = xmlDoc.NewElement("annotation");
		xmlDoc.InsertFirstChild(annotation);

		XMLElement * pElement = xmlDoc.NewElement("folder");
		pElement->SetText("VOCType");
		annotation->InsertFirstChild(pElement);

		pElement = xmlDoc.NewElement("filename");
		pElement->SetText(name); //pRectList->mRectArrayList[frameNum].name.c_str());
		annotation->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("source");
		XMLElement * pElement_sub = xmlDoc.NewElement("database");
		pElement_sub->SetText("VOC");
		pElement->InsertFirstChild(pElement_sub);
		annotation->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("size");
		pElement_sub = xmlDoc.NewElement("width");
		pElement_sub->SetText(FrameImg->width);//pRectList->mRectArrayList[frameNum].RectElement.width);
		pElement->InsertFirstChild(pElement_sub);
		pElement_sub = xmlDoc.NewElement("height");
		pElement_sub->SetText(FrameImg->height);//pRectList->mRectArrayList[frameNum].RectElement.height);
		pElement->InsertEndChild(pElement_sub);
		pElement_sub = xmlDoc.NewElement("depth");
		pElement_sub->SetText(3);
		pElement->InsertEndChild(pElement_sub);
		annotation->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("segmented"); // 是否分割
		pElement->SetText(0);
		annotation->InsertEndChild(pElement);

		for (int k = 0; k < object_num; ++k)
		{
			pElement = xmlDoc.NewElement("object");
			pElement_sub = xmlDoc.NewElement("name"); // 类别
			pElement_sub->SetText(pRectList->mRectArrayList[k].label.c_str());
			pElement->InsertFirstChild(pElement_sub);

			pElement_sub = xmlDoc.NewElement("pose"); // 姿态
			pElement_sub->SetText("Unspecified");
			pElement->InsertEndChild(pElement_sub);

			pElement_sub = xmlDoc.NewElement("truncated");
			pElement_sub->SetText(0);
			pElement->InsertEndChild(pElement_sub);

			pElement_sub = xmlDoc.NewElement("difficult");
			pElement_sub->SetText(0);
			pElement->InsertEndChild(pElement_sub);

			pElement_sub = xmlDoc.NewElement("bndbox");
			XMLElement* pElement_sub_sub = xmlDoc.NewElement("xmin");
			pElement_sub_sub->SetText(pRectList->mRectArrayList[k].pos[0]);
			pElement_sub->InsertFirstChild(pElement_sub_sub);
			pElement_sub_sub = xmlDoc.NewElement("ymin");
			pElement_sub_sub->SetText(pRectList->mRectArrayList[k].pos[1]);
			pElement_sub->InsertEndChild(pElement_sub_sub);
			pElement_sub_sub = xmlDoc.NewElement("xmax");
			pElement_sub_sub->SetText(pRectList->mRectArrayList[k].pos[2]);
			pElement_sub->InsertEndChild(pElement_sub_sub);
			pElement_sub_sub = xmlDoc.NewElement("ymax");
			pElement_sub_sub->SetText(pRectList->mRectArrayList[k].pos[3]);
			pElement_sub->InsertEndChild(pElement_sub_sub);
			pElement->InsertEndChild(pElement_sub);

			annotation->InsertEndChild(pElement);

		}



		string savefile = "D:\\tld_track\\tld_track_多目标\\tld_track\\Annotations/";
		//for (int x = 0; x < pRectList->mRectArrayList[j].name.length() - 4; ++x)
		//{
		//	filename += pRectList->mRectArrayList[j].name[x];

		savefile += name;
		//}
		savefile += ".xml";
		xmlDoc.SaveFile(savefile.c_str());



	}







	// #ifdef  EV_JPEG
	// 	evReleaseCapture(&pCapture); 
	// #else
	cvReleaseCapture(&pCapture);
	// #endif
	cvReleaseImage(&FrameImg);
	cvReleaseImage(&FrameGray);
	cvReleaseImage(&TempImg);
	cvDestroyAllWindows();
	AmtStructUnInit(&Amt);
}


void On_Mouse(int event, int x, int y, int flags, void* param)
{

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		pre_pt = cvPoint(x, y);
		printf("*************************Left Button Down, point is (%d %d)\n\n", pre_pt.x, pre_pt.y);
	}
	if (event == CV_EVENT_MOUSEMOVE && (flags && CV_EVENT_FLAG_LBUTTON))
	{
		end_pt = cvPoint(x, y);
		printf("*************************Mouse Move, point is (%d %d)\r\r", end_pt.x, end_pt.y);

		SelectRect.x = MIN(pre_pt.x, end_pt.x);
		SelectRect.y = MIN(pre_pt.y, end_pt.y);
		SelectRect.width = abs(pre_pt.x - end_pt.x);
		SelectRect.height = abs(pre_pt.y - end_pt.y);
		cvCopy(FrameImg, TempImg);
		//		cvLine(temp, pre_pt, end_pt, CV_RGB(255, 0, 0), 2, 8, 0);
		cvRectangle(TempImg, pre_pt, end_pt, CV_RGB(255, 0, 0), 2, 8, 0);
		cvShowImage(window_name, TempImg);
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		end_pt = cvPoint(x, y);
		printf("*************************Left Mouse Up, point is (%d %d)\n\n", end_pt.x, end_pt.y);

		SelectRect.x = MIN(pre_pt.x, end_pt.x);
		SelectRect.y = MIN(pre_pt.y, end_pt.y);
		SelectRect.width = abs(pre_pt.x - end_pt.x);
		SelectRect.height = abs(pre_pt.y - end_pt.y);



		int isfound = -1;

		if ((SelectRect.x >= 0) && (SelectRect.x<FrameImg->width) && (SelectRect.y >= 0) && (SelectRect.y<FrameImg->height) && (SelectRect.width >= MIN_RECT_WIDTH) && (SelectRect.height >= MIN_RECT_HEIGHT))
		{
			for (int i = 0; i<MAX_OBJ_TRACKING; i++)
			{
				if (pRectList->mRectArrayList[i].iIsActive == -1)
				{
					pRectList->mRectArrayList[i].RectElement.x = SelectRect.x;
					pRectList->mRectArrayList[i].RectElement.y = SelectRect.y;
					pRectList->mRectArrayList[i].RectElement.width = SelectRect.width;
					pRectList->mRectArrayList[i].RectElement.height = SelectRect.height;
					pRectList->mRectArrayList[i].iIsActive = 1;
					pRectList->mRectArrayList[i].iIsNew = 1;
					isfound = i;
					pRectList->iNewRectGenerated = 1;//indicating there is new Rect generated from windows input
					pRectList->mRectArrayList[i].isAnnotated = 1;


					string label = "";
					printf("输入label:");
					cin >> label;

					pRectList->mRectArrayList[i].label.assign(label, 0, sizeof(label));

					//pRectList->mRectArrayList[i].pos[0] = SelectRect.x + 0.5*SelectRect.width;
					//pRectList->mRectArrayList[i].pos[1] = SelectRect.y + 0.5*SelectRect.height;
					//pRectList->mRectArrayList[i].pos[2] = SelectRect.x + 0.5*SelectRect.width;
					//pRectList->mRectArrayList[i].pos[3] = SelectRect.x + 0.5*SelectRect.height;

					break;
				}
			}
			if (isfound < 0)
			{
				printf("There are too many boxes in executing!\n");
			}
		}

		cvCopy(FrameImg, TempImg); //将FrameImg中图像内容拷贝到TempImg中去
		cvRectangle(TempImg, pre_pt, end_pt, CV_RGB(0, 255, 0), 2, 8, 0);
		cvShowImage(window_name, TempImg);
		cvWaitKey(1);
		track_object = -1;


	}


}

void Ctld_trackDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码

	CString m_strFilePathName;
	// 存储最终文件路径
	char fName[1024];
	CFileDialog select_dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files(*.mp4)|*.mp4|所有文件(*.*)|*.*|"), NULL);
	if (select_dlg.DoModal())
	{
		m_strFilePathName = select_dlg.GetPathName();
	}

	int len = WideCharToMultiByte(CP_ACP, 0, m_strFilePathName, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, m_strFilePathName, -1, fName, len, NULL, NULL);

	printf("fName = %s\n", fName);

	cvWaitKey(100);
	//EnableMemLeakCheck();
	//// 	_CrtSetBreakAlloc(68);

	//for (int i = 0; i<MAX_OBJ_TRACKING; i++)
	//{
	//	pRectList->mRectArrayList[i].iIsActive = -1;
	//	pRectList->mRectArrayList[i].iIsNew = -1;
	//	pRectList->mRectArrayList[i].iObjID = -1;
	//}
	//pRectList->iTotal = -1;
	//pRectList->iNewRectGenerated = -1;

	//LineTypeList.iThickness[0] = 2;
	//LineTypeList.LineColor[0] = cvScalar(255, 0, 0);

	//LineTypeList.iThickness[1] = 3;
	//LineTypeList.LineColor[1] = cvScalar(0, 255, 0);

	//LineTypeList.iThickness[2] = 4;
	//LineTypeList.LineColor[2] = cvScalar(0, 0, 255);


	//run_tld();

	////	TestMeanShiftTrack();

	//_CrtDumpMemoryLeaks();
}


void Ctld_trackDlg::OnBnClickedSavepath()
{
	// TODO: 在此添加控件通知处理程序代码
	CString save_path = FicowGetDirectory();
	cvWaitKey(100);
	
}

CString Ctld_trackDlg::FicowGetDirectory()
{
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return NULL;
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if (strDirectoryPath.IsEmpty())
		return NULL;
	if (strDirectoryPath.Right(1) != "\\")
		strDirectoryPath += "\\";

	return strDirectoryPath;
}

void Ctld_trackDlg::OnBnClickedAddlabel()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctld_trackDlg::OnBnClickedDeletelabel()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctld_trackDlg::OnBnClickedMovelabel()
{
	// TODO: 在此添加控件通知处理程序代码
}

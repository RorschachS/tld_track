
// tld_trackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tld_track.h"
#include "tld_trackDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "TextDlg.h"
#include "LabelManager.h"



//Ctld_trackDlg* p_Dlg = NULL;
int isInRect(CvPoint point, CvPoint pre_pt, CvPoint end_pt);
void On_Mouse(int event, int x, int y, int flags, void* param);


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
IplImage* PauseShowImg = NULL;
clock_t TimeStamp;
CvvImage cimg;
HDC hDC;
CDC *pDC;
CRect DrawRect;

CRect rect1;
LabelManager mLabelManager;

int track_object = 0;
int object_num = 0;
int color_index[10][3] = { 
	0
						//{255,0,128},
						//{255,0,255},
						//{255,128,0},
						//{0,128,128},
						//{0,128,255},
						//{128,255,0},
						//{0,255,128},
						//{0,255,255},
						//{128,128,0},
						//{128,0,128}
};
//vector<T>color_index[10] = { {0,0,128},{ 0,0,255 } };
CvRect track_window;
CvRect sROI;
CvBox2D track_box;
CvConnectedComp track_comp;
int backproject_mode = 0;
CvPoint pre_pt, end_pt;
CvPoint mov_pre_pt, mov_end_pt;
CvRect SelectRect;

char file_path[200] = "";
CString save_path;
bool en_select = false;//不可标注
bool bPause = false;//false 暂停
int mouse_click;
int move_id = -1;
EvAMT Amt;
CString labellist=NULL;


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
	DDX_Control(pDX, IDABORT, m_Enble);
	DDX_Control(pDX, IDC_COMBO1, LabelList);
	DDX_Control(pDX, IDC_MFCBUTTON_COLOR, mfcBtnColor);
}

BEGIN_MESSAGE_MAP(Ctld_trackDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(Btn_Open, &Ctld_trackDlg::OnBnClickedOpen)
	ON_BN_CLICKED(Btn_SavePath, &Ctld_trackDlg::OnBnClickedSavepath)
	ON_BN_CLICKED(Btn_AddLabel, &Ctld_trackDlg::OnBnClickedAddlabel)
	ON_BN_CLICKED(Btn_DeleteLabel, &Ctld_trackDlg::OnBnClickedDeletelabel)
	ON_BN_CLICKED(IDABORT, &Ctld_trackDlg::OnBnClickedAbort)
	ON_WM_TIMER()

	ON_BN_CLICKED(Btn_AdjustLabel, &Ctld_trackDlg::OnBnClickedAdjustlabel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Ctld_trackDlg::OnCbnSelchangeLabelBox)
	ON_BN_CLICKED(IDC_MFCBUTTON_COLOR, &Ctld_trackDlg::OnBnClickedMfcbuttonColor)
	ON_STN_CLICKED(IDC_STATIC_Label, &Ctld_trackDlg::OnStnClickedStaticLabel)
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
	ShowWindow(SW_MAXIMIZE);
	
	//通过定时器刷新picture control显示视频
	//pDC = GetDlgItem(IDC_PIC_SHOW)->GetDC();
   //hDC = pDC->GetSafeHdc();
   //GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&DrawRect);
   //pDC->SelectStockObject(NULL_BRUSH); //不使用画刷

//****************OpenCV显示窗口嵌入MFC的picture control控件******************

	CWnd  *pWnd1 = GetDlgItem(IDC_PIC_SHOW);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	pWnd1->GetClientRect(&rect1);//GetClientRect为获得控件相自身的坐标大小
	namedWindow(window_name, WINDOW_AUTOSIZE);//设置窗口名
	HWND hWndl = (HWND)cvGetWindowHandle(window_name);//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent1 = ::GetParent(hWndl);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl, GetDlgItem(IDC_PIC_SHOW)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);//ShowWindow指定窗口中显示

	//CDC *pDC = GetDlgItem(IDC_PIC_SHOW)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
	//hdc = pDC->GetSafeHdc();                      // 获取设备上下文句柄
	//GetDlgItem(IDC_STATIC)->GetClientRect(&rect); //获取box1客户区





	LabelList.AddString(_T("hat"));
	LabelList.AddString(_T("head"));
	LabelList.SetCurSel(0);

	mfcBtnColor.m_bTransparent = FALSE;
	mfcBtnColor.m_bDontUseWinXPTheme = TRUE;
	mfcBtnColor.m_bDrawFocus = FALSE;
	
	//*************************************

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


void Ctld_trackDlg::DrawPicToHDC(IplImage * img, UINT ID)
{
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &DrawRect);
};


//void Ctld_trackDlg::run_tld()

//void run_tld()
DWORD _stdcall run_tld(LPVOID lpParameter)
{
	//Ctld_trackDlg * pDlg = (Ctld_trackDlg*)lpParameter;
	//cvNamedWindow("avi");
	
	char* filename = file_path;
	//char* filename = "D:\\yolo\\videos\\test_video1.avi";
	CvCapture* pCapture = cvCreateFileCapture(filename);
	//CvCapture* pCapture = cvCaptureFromCAM(0);
	//EvAMT Amt;
	int FrameCount;
	CvRect initBox;
	if (!pCapture)
	{
		//fprintf(stderr, "pCapture is Null!\n");
		AfxMessageBox(_T("读取失败,请安装XviD Codec"));
		return 0;
	}
	int iScale = 1.0;
	double BeginFrameIdx = 3;
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
		return 0;
	}

	if (FrameImg == NULL)
	{
		
		//FrameImg = cvCreateImage(cvSize(CapImg->width / iScale, CapImg->height / iScale), CapImg->depth, CapImg->nChannels);
		FrameImg = cvCreateImage(cvSize(rect1.Width(), rect1.Height()), CapImg->depth, CapImg->nChannels);
		FrameGray = cvCreateImage(cvGetSize(FrameImg), FrameImg->depth, 1);
		TempImg = cvCreateImage(cvGetSize(FrameImg), FrameImg->depth, FrameImg->nChannels);
		AmtSetConfig(&Amt, szChannleID, FrameImg->width, FrameImg->height, Param, NULL);
	}

	int iKey = 0, frameNum = 0;
	CvFont font = cvFont(1, 1);   // init font
								  //char text[256];
	//cvNamedWindow(window_name, 1);

	
	//en_select = false;


	int iPauseKey = 1;
	double dProcTime = 0, dTime;
	int iProcCount = 0;
	for (;;)  //无限循环直到按下“ESC”退出
	{

		//cvWaitKey(40); //将视频变慢
		if (!bPause)
		{

		if (!(CapImg = cvQueryFrame(pCapture)))
		{
			fprintf(stderr, "pFrame is Null!\n");
			AfxMessageBox(_T("播放结束"));
			break;
		}
		cvResize(CapImg, FrameImg);

		//cvFlip(FrameImg);

		cvCvtColor(FrameImg, FrameGray, CV_RGB2GRAY);

		//iKey = cvWaitKey(10);  // "ESC"
		//if (iKey == 27)
		//{
		//	break;
		//}

		

			frameNum++;
			if (frameNum == BeginFrameIdx-1)
			{
				bPause = true; continue;
			}


			Mat saveimg = cvarrToMat(CapImg);
			

			if (pRectList->iNewRectGenerated == 1) // if there is new rectangle generated 
			{
				for (int i = 0; i < MAX_OBJ_TRACKING; i++)
				{
					if ((pRectList->mRectArrayList[i].iIsNew == 1) && (pRectList->mRectArrayList[i].iIsActive == 1))
					{
						TimeStamp = clock();
						pRectList->mRectArrayList[i].iObjID = i;  //将ID 付给这个框   ID是连接跟踪目标和框的唯一标识

						AmtCreateObject(&Amt, pRectList->mRectArrayList[i].RectElement, i, (long long)TimeStamp);
						pRectList->mRectArrayList[i].iIsNew = -1;
					}
				}
				pRectList->iNewRectGenerated = -1;
			}

			dTime = GetTickCount();
			// ======================  TLD处理每一帧  ================================
			AmtExecute(&Amt, FrameGray, (long long)time(NULL) * 1000, AMT_ALL_AROUND);//此处如何给定初始位置
			dTime = GetTickCount() - dTime;
			dProcTime += dTime;
			iProcCount++;

			//fprintf(stdout, "帧数: %d, time = %g ms\r", frameNum, dTime);
			int boundLineY = 2;
			int boundLineX = 2;

			
	
			
			for (int i = 0; i < MAX_OBJECT_NUM; i++)
			{
				if (Amt.m_ObjStatus[i].mStatus) //如果目标处于激活状态
				{

					int iObjID = Amt.m_ObjStatus[i].mID;
					int iCountMiss = Amt.m_ObjStatus[i].mCountMiss;
					int iUnStableN = Amt.m_ObjStatus[i].mUnstableNum;
					CvPoint &mPos = Amt.m_ObjStatus[i].mPos;
					// 销毁过线目标(目标框太小）
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
					//AmtGetObjTrajectory(&Amt, iObjID, &pPoints, &iPointNum);

					//for (int k = 0; k < iPointNum - 1; ++k)
					//{
					//	cvLine(FrameImg, cvPoint(pPoints[k].x, pPoints[k].y), cvPoint(pPoints[k + 1].x, pPoints[k + 1].y), LineTypeList.LineColor[iObjID % 3], LineTypeList.iThickness[iObjID % 3]);
					//}
						CvRect* pEndRect = &(Amt.m_ObjStatus[i].mBbox);
						string temp_label = pRectList->mRectArrayList[i].label;
						
						cvRectangle(FrameImg, cvPoint(pEndRect->x, pEndRect->y), cvPoint(pEndRect->x + pEndRect->width, pEndRect->y + pEndRect->height), cvScalar(color_index[i][0], color_index[i][1], color_index[i][2]), 2);
										
						pRectList->mRectArrayList[i].pos[0] = pEndRect->x;
						pRectList->mRectArrayList[i].pos[1] = pEndRect->y;
						pRectList->mRectArrayList[i].pos[2] = pEndRect->x + pEndRect->width;
						pRectList->mRectArrayList[i].pos[3] = pEndRect->y + pEndRect->height;
					
					//cvPolyLine(FrameImg, &pPoints, &iPointNum, 1, 0, CV_RGB(0,255,0), 1);
				}
			}
			cvLine(FrameImg, cvPoint(boundLineX, 0), cvPoint(boundLineX, FrameImg->height - 1), cvScalar(255), 2, 8, 0);
			cvLine(FrameImg, cvPoint(FrameImg->width - boundLineX, 0), cvPoint(FrameImg->width - boundLineX, FrameImg->height - 1), cvScalar(255), 2, 8, 0);
			cvLine(FrameImg, cvPoint(0, boundLineY), cvPoint(FrameImg->width - 1, boundLineY), cvScalar(255), 2, 8, 0);


			//cv::Mat m_dst;
			//GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
			//cv::Rect dst(rect1.left, rect1.top, rect1.right, rect1.bottom);
			//cv::resize(FrameImg, m_dst, cv::Size(rect.Width(), rect.Height()));
			//IplImage* show_img=cvCreateImage(CvSize(rect1.Width(), rect1.Height()));
			//cvResize(FrameImg, show_img);




			cvShowImage(window_name, FrameImg);


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
			string xml_save_path = CT2A(save_path);
			xml_save_path += name;
			string img_save_path = xml_save_path;
			//}
			xml_save_path += ".xml";
			img_save_path += ".jpg";


			XMLNode * annotation = xmlDoc.NewElement("annotation");
			xmlDoc.InsertFirstChild(annotation);

			XMLElement * pElement = xmlDoc.NewElement("folder");
			pElement->SetText("VOCType");
			annotation->InsertFirstChild(pElement);

			pElement = xmlDoc.NewElement("filename");
			pElement->SetText(name); //pRectList->mRectArrayList[frameNum].name.c_str());
			annotation->InsertEndChild(pElement);

			pElement = xmlDoc.NewElement("path");
			pElement->SetText(img_save_path.data()); //pRectList->mRectArrayList[frameNum].name.c_str());
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

			//string savefile = save_path.GetBuffer;// = (LPCTSTR)save_path;


			//for (int x = 0; x < pRectList->mRectArrayList[j].name.length() - 4; ++x)
			//{
			//	filename += pRectList->mRectArrayList[j].name[x];

			//int params[3];
			//params[0] = CV_IMWRITE_JPEG_QUALITY;
			//params[1] = 85;//设置s压缩度
			//params[2] = 0;
			//IplImage* saveImage = FrameImg;
			//cvCvtColor(FrameImg, FrameImg, CV_RGB2BGR);
	//		Mat saveimg = cvarrToMat(FrameImg);
			xmlDoc.SaveFile(xml_save_path.c_str());
			imwrite(img_save_path.c_str(), saveimg);
			//}//if（bPause）结束
			//else
			//{
			//	;
			//}
		}
	}//for循环

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



	return 0;
}


void On_Mouse(int event, int x, int y, int flags, void* param)
{
	
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		pre_pt = cvPoint(x, y);
		if (en_select == true && mouse_click == ADD)
		{
			;
		}
		else if (en_select == true && mouse_click == ADJUST)

		{
			//pre_pt = cvPoint(x, y);
			for (int i = 0; i < MAX_OBJ_TRACKING; i++)
			{

				//pre_pt和end_pt两点在选择框内
				//if ((pRectList->mRectArrayList[i].iIsActive == 1 )&&(abs(pre_pt.x - pRectList->mRectArrayList[i].RectElement.x)<0.5*(pRectList->mRectArrayList[i].RectElement.width))&&(abs(pre_pt.y - pRectList->mRectArrayList[i].RectElement.y)<0.5*(pRectList->mRectArrayList[i].RectElement.height)))
				if ((Amt.m_ObjStatus[i].mStatus == 1) && (abs(pre_pt.x - Amt.m_ObjStatus[i].mPos.x)<0.5*(Amt.m_ObjStatus[i].mBbox.width)) && (abs(pre_pt.y - Amt.m_ObjStatus[i].mBbox.y)<0.5*(Amt.m_ObjStatus[i].mBbox.y)))
				{
					move_id = i; break;
					//cvWaitKey(10);
				}
				else
				{
					move_id = -1;
				}
			}
		}
		else if (en_select == true && mouse_click == DELET)
		{
			for (int i = 0; i < MAX_OBJ_TRACKING; i++)
			{
				//if ((Amt.m_ObjStatus[i].mStatus == 1) && (abs(pre_pt.x - Amt.m_ObjStatus[i].mPos.x) < 0.5*(Amt.m_ObjStatus[i].mBbox.width)) && (abs(pre_pt.y - Amt.m_ObjStatus[i].mBbox.y) < 0.5*(Amt.m_ObjStatus[i].mBbox.y)))
				if ((Amt.m_ObjStatus[i].mStatus == 1) && (isInRect(pre_pt,CvPoint(Amt.m_ObjStatus[i].mBbox.x, Amt.m_ObjStatus[i].mBbox.y), CvPoint(Amt.m_ObjStatus[i].mBbox.x+ Amt.m_ObjStatus[i].mBbox.width, Amt.m_ObjStatus[i].mBbox.y+ Amt.m_ObjStatus[i].mBbox.height))==1))
				{
					move_id = i; break;
					//cvWaitKey(10);
				}
				else
				{
					move_id = -1;
				}
			}
		}
		else
		{
			;
		}


		//printf("*************************Left Button Down, point is (%d %d)\n\n", pre_pt.x, pre_pt.y);
	}
	if (event == CV_EVENT_MOUSEMOVE && (flags && CV_EVENT_FLAG_LBUTTON))
	{
		end_pt = cvPoint(x, y);
		if (en_select == true && mouse_click == ADD) {
			
			//printf("*************************Mouse Move, point is (%d %d)\r\r", end_pt.x, end_pt.y);

			SelectRect.x = MIN(pre_pt.x, end_pt.x);
			SelectRect.y = MIN(pre_pt.y, end_pt.y);
			SelectRect.width = abs(pre_pt.x - end_pt.x);
			SelectRect.height = abs(pre_pt.y - end_pt.y);
			cvCopy(FrameImg, TempImg);
			//		cvLine(temp, pre_pt, end_pt, CV_RGB(255, 0, 0), 2, 8, 0);
			cvRectangle(TempImg, pre_pt, end_pt, CV_RGB(255, 0, 0), 2, 8, 0);
			cvShowImage(window_name, TempImg);
		}
		else if (en_select == true && mouse_click == ADJUST)
		{
			//end_pt = cvPoint(x, y);
			
			//const int mov_x = pRectList->mRectArrayList[move_id].RectElement.x;
			mov_pre_pt.x = Amt.m_ObjStatus[move_id].mBbox.x + (end_pt.x - pre_pt.x);
			mov_pre_pt.y = Amt.m_ObjStatus[move_id].mBbox.y + (end_pt.y - pre_pt.y);
			mov_end_pt.x = mov_pre_pt.x + Amt.m_ObjStatus[move_id].mBbox.width;
			mov_end_pt.y = mov_pre_pt.y + Amt.m_ObjStatus[move_id].mBbox.height;

			cvCopy(CapImg, TempImg);
					//		cvLine(temp, pre_pt, end_pt, CV_RGB(255, 0, 0), 2, 8, 0);
			cvRectangle(TempImg, mov_pre_pt, mov_end_pt, CV_RGB(255, 0, 0), 2, 8, 0);
			cvCircle(TempImg, mov_pre_pt,5,cvScalar(0, 0, 255),-1);
			cvCircle(TempImg, mov_end_pt,5, cvScalar(0, 0, 255),-1);
			cvCircle(TempImg, CvPoint(mov_pre_pt.x,mov_end_pt.y),5, cvScalar(0, 0, 255),-1);
			cvCircle(TempImg, CvPoint(mov_end_pt.x, mov_pre_pt.y),5, cvScalar(0, 0, 255),-1);
			cvShowImage(window_name, TempImg);
				
			}

		else
		{
			;
		}
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		
		if (en_select == true && mouse_click == ADD)
		{
			end_pt = cvPoint(x, y);

			SelectRect.x = MIN(pre_pt.x, end_pt.x);
			SelectRect.y = MIN(pre_pt.y, end_pt.y);
			SelectRect.width = abs(pre_pt.x - end_pt.x);
			SelectRect.height = abs(pre_pt.y - end_pt.y);



			int isfound = -1;

			if ((SelectRect.x >= 0) && (SelectRect.x < FrameImg->width) && (SelectRect.y >= 0) && (SelectRect.y < FrameImg->height) && (SelectRect.width >= MIN_RECT_WIDTH) && (SelectRect.height >= MIN_RECT_HEIGHT))
			{
				for (int i = 0; i < MAX_OBJ_TRACKING; i++)
				{
					if (pRectList->mRectArrayList[i].iIsActive == -1)
					{
						/*CString temp_label = NULL;
						TextDlg text_dlg;
						if (text_dlg.DoModal() == IDOK)
						{
							temp_label = text_dlg.get_text;
						
						string label2 = CT2A(temp_label);*/
						string label =	CT2A(labellist);
						pRectList->mRectArrayList[i].label.assign(label, 0, sizeof(label));

						pRectList->mRectArrayList[i].RectElement.x = SelectRect.x;
						pRectList->mRectArrayList[i].RectElement.y = SelectRect.y;
						pRectList->mRectArrayList[i].RectElement.width = SelectRect.width;
						pRectList->mRectArrayList[i].RectElement.height = SelectRect.height;
						pRectList->mRectArrayList[i].iIsActive = 1;
						pRectList->mRectArrayList[i].iIsNew = 1;
						isfound = i;
						pRectList->iNewRectGenerated = 1;//indicating there is new Rect generated from windows input
						pRectList->mRectArrayList[i].isAnnotated = 1;
						//Amt.m_ObjStatus[i].mStatus = 1;

				/*		}
						else if (text_dlg.DoModal() == IDCANCEL)
						{
							;
						}*/
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
			object_num++;
			en_select = false;

		}


		else if (en_select == true && mouse_click == ADJUST)
		{
			end_pt = cvPoint(x, y);
			Amt.m_ObjStatus[move_id].mBbox.x = mov_pre_pt.x;
			Amt.m_ObjStatus[move_id].mBbox.y = mov_pre_pt.y; 
			Amt.m_ObjStatus[move_id].mPos.x += end_pt.x - pre_pt.x;
			Amt.m_ObjStatus[move_id].mPos.y += end_pt.y - pre_pt.y;

			pRectList->mRectArrayList[move_id].RectElement.x = mov_pre_pt.x;
			pRectList->mRectArrayList[move_id].RectElement.y = mov_pre_pt.y;

			pRectList->mRectArrayList[move_id].RectElement.width = SelectRect.width;
			pRectList->mRectArrayList[move_id].RectElement.height = SelectRect.height;

			cvCopy(FrameImg, TempImg); //将FrameImg中图像内容拷贝到TempImg中去
			cvRectangle(TempImg, mov_pre_pt, mov_end_pt,CV_RGB(0, 255, 0), 2, 8, 0);

			cvCircle(TempImg, mov_pre_pt, 5, cvScalar(0, 0, 255),-1);
			cvCircle(TempImg, mov_end_pt, 5, cvScalar(0, 0, 255),-1);
			cvCircle(TempImg, CvPoint(mov_pre_pt.x, mov_end_pt.y),5, cvScalar(0, 0, 255),-1);
			cvCircle(TempImg, CvPoint(mov_end_pt.x, mov_pre_pt.y),5, cvScalar(0, 0, 255),-1);
			
			cvShowImage(window_name, TempImg);
			TimeStamp = clock();
			pRectList->mRectArrayList[move_id].iObjID = move_id;  //将ID 付给这个框   ID是连接跟踪目标和框的唯一标识

			AmtCreateObject(&Amt, pRectList->mRectArrayList[move_id].RectElement, move_id, (long long)TimeStamp);
			en_select = false;
		}
		else if (en_select == true && mouse_click == DELET)
		{
			//AmtCleanObject(&Amt, move_id);
			//pRectList->iNewRectGenerated = -1;
			//pRectList->mRectArrayList[move_id].iIsActive = -1;
			//pRectList->mRectArrayList[move_id].iObjID = -1;
			//pRectList->mRectArrayList[move_id].iIsNew = -1;
			//pRectList->mRectArrayList[move_id].label.clear();
			//pRectList->mRectArrayList[move_id].isAnnotated = -1;
			int iRectIndex = -1;
			for (int j = 0; j < MAX_OBJ_TRACKING; j++)
			{
				if (pRectList->mRectArrayList[j].iObjID == move_id)
				{
					iRectIndex = j;
					break;
				}
			}
			if (iRectIndex >= 0)//标记目标个数大于0
			{
				pRectList->mRectArrayList[iRectIndex].iIsActive = -1;
				pRectList->mRectArrayList[iRectIndex].iIsNew = -1;
				pRectList->mRectArrayList[iRectIndex].iObjID = -1;
				pRectList->mRectArrayList[iRectIndex].label.clear();
				pRectList->mRectArrayList[iRectIndex].isAnnotated = -1;
				pRectList->mRectArrayList[iRectIndex].RectElement.x = -1;
				pRectList->mRectArrayList[iRectIndex].RectElement.y = -1;
				pRectList->mRectArrayList[iRectIndex].RectElement.width = -1;
				pRectList->mRectArrayList[iRectIndex].RectElement.height = -1;
				AmtCleanObject(&Amt, move_id);
				object_num--;
				PauseShowImg = cvCloneImage(CapImg);
				for (int i = 0; i < MAX_OBJECT_NUM; i++)
				{
					if (Amt.m_ObjStatus[i].mStatus) //如果目标处于激活状态
					{
						cvRectangle(PauseShowImg, cvPoint(Amt.m_ObjStatus[i].mBbox.x, Amt.m_ObjStatus[i].mBbox.y), cvPoint(Amt.m_ObjStatus[i].mBbox.x + Amt.m_ObjStatus[i].mBbox.width, Amt.m_ObjStatus[i].mBbox.y + Amt.m_ObjStatus[i].mBbox.height), cvScalar(0, 255, 0), 1);
					}
				}
			}
			

		
			cvCopy(PauseShowImg, TempImg);
			cvShowImage(window_name, TempImg);

			
		}
	}
	else
	{
		;
	}

}




int isInRect(CvPoint point, CvPoint pre_pt, CvPoint end_pt)
{
	if (point.x<0||point.y<0)
	{
		return -1;
	}
	else 
	{
		int min_x = MIN(pre_pt.x, end_pt.x);
		int max_x = MAX(pre_pt.x, end_pt.x);
		int min_y = MIN(pre_pt.y, end_pt.y);
		int max_y = MAX(pre_pt.y, end_pt.y);
		if (point.x>min_x&&point.x<max_x&&point.y>min_y&&point.y<max_y)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		
	}
}




void Ctld_trackDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码


	// 获取存储文件路径并赋值给file_path变量
	CString myPathName = NULL;
	CFileDialog select_dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files(*.avi)|*.avi|所有文件(*.*)|*.*|"), NULL);
	if (select_dlg.DoModal()==IDOK)
	{
		myPathName = select_dlg.GetPathName();
		//AfxMessageBox("运行正常");
	}

	int len = WideCharToMultiByte(CP_ACP, 0, myPathName, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, myPathName, -1, file_path, len, NULL, NULL);
	//	file_path = fName;
	//	printf("fName = %s\n", fName);

	// TODO: 在此添加控件通知处理程序代码
	EnableMemLeakCheck();
	// 	_CrtSetBreakAlloc(68);

	for (int i = 0; i < MAX_OBJ_TRACKING; i++)
	{
		pRectList->mRectArrayList[i].iIsActive = -1;
		pRectList->mRectArrayList[i].iIsNew = -1;
		pRectList->mRectArrayList[i].iObjID = -1;
	}
	pRectList->iTotal = -1;
	pRectList->iNewRectGenerated = -1;

	LineTypeList.iThickness[0] = 2;
	LineTypeList.LineColor[0] = cvScalar(255, 0, 0);

	LineTypeList.iThickness[1] = 3;
	LineTypeList.LineColor[1] = cvScalar(0, 255, 0);

	LineTypeList.iThickness[2] = 4;
	LineTypeList.LineColor[2] = cvScalar(0, 0, 255);

	cvSetMouseCallback(window_name, On_Mouse);
	m_hThread = CreateThread(NULL, 0, run_tld, this, 0, NULL);
	//waitKey(100);
	//SuspendThread(m_hThread);
	//SuspendThread(m_hThread);


	//run_tld();

	//	TestMeanShiftTrack();

	//_CrtDumpMemoryLeaks();
}


void Ctld_trackDlg::OnBnClickedSavepath()
{
	// TODO: 在此添加控件通知处理程序代码
	save_path = FicowGetDirectory();
	//cvWaitKey(100);

}

CString Ctld_trackDlg::FicowGetDirectory()
{
	BROWSEINFO bi;
	//	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return NULL;

	SHGetPathFromIDList(idl, save_path.GetBuffer(MAX_PATH));
	save_path.ReleaseBuffer();
	if (save_path.IsEmpty())
		return NULL;
	if (save_path.Right(1) != "\\")
		save_path += "\\";
	return save_path;
}

void Ctld_trackDlg::OnBnClickedAddlabel()
{
	// TODO: 在此添加控件通知处理程序代码
	en_select = true;
	mouse_click = ADD;
	int labelIndex = LabelList.GetCurSel();
	LabelList.GetLBText(labelIndex, labellist);
	//cvWaitKey(100);

	//cvSetMouseCallback(window_name, On_Mouse);

}


void Ctld_trackDlg::OnBnClickedDeletelabel()
{
	// TODO: 在此添加控件通知处理程序代码
	en_select = true;
	mouse_click = DELET;
}





void Ctld_trackDlg::OnBnClickedAbort()
{
	// TODO: 在此添加控件通知处理程序代码


	static int count = 1;  //用来保存按钮状态，用0、1表示
	if (count % 2 == 1)
	{
		//	//设置控件名
		m_Enble.SetWindowTextW(_T("暂停"));
		//	GetDlgItem(IDABORT)->SetDlgItemTextW(IDABORT, (LPCTSTR)"暂停");
		count = 0;  //更新按钮状态
		//KillTimer(1);  //移除定时器
		bPause = false; //修改暂停标志位，使播放
		ResumeThread(m_hThread);

	}
	else
	{
		m_Enble.SetWindowTextW(_T("播放"));
		//GetDlgItem(IDABORT)->SetDlgItemTextW(IDABORT, (LPCTSTR)"播放");
		count = 1;  //更新按钮状态
		//SetTimer(1, 100, NULL);  //设置定时器
		bPause = true; //修改暂停标志位，使暂停
		SuspendThread(m_hThread);
	}




	//run_tld();
}



void Ctld_trackDlg::OnBnClickedAdjustlabel()
{
	// TODO: 在此添加控件通知处理程序代码

	en_select = true;
	mouse_click = ADJUST;
}


void Ctld_trackDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
		//run_tld();
		//	DrawPicToHDC(FrameImg, IDC_PIC_SHOW);

		CDialogEx::OnTimer(nIDEvent);
}



void Ctld_trackDlg::OnCbnSelchangeLabelBox()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_label = NULL;
	////char* comp_label = "编辑";
	//CString comp_label = NULL;
	//comp_label.Format('%s', "编辑");
	//int temp_index = LabelList.GetCurSel();
	//LabelList.GetLBText(temp_index, temp_label);
	//if (temp_label.Compare(comp_label)==0)
	//{
	//	mLabelManager.DoModal();
	//}
	//else {
	int temp_index = LabelList.GetCurSel();
	COLORREF temp_clr = RGB(color_index[temp_index][0], color_index[temp_index][1], color_index[temp_index][2]);
	mfcBtnColor.SetFaceColor(temp_clr);
	//}
}




void Ctld_trackDlg::OnBnClickedMfcbuttonColor()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF m_clr=(255,255,255);
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;   // CC_RGBINIT可以让上次选择的颜色作为初始颜色显示出来
	m_setClrDlg.m_cc.rgbResult=m_clr;        //记录上次选择的颜色
	if (m_setClrDlg.DoModal()==IDOK)
	{
		m_clr = m_setClrDlg.m_cc.rgbResult;            // 保存用户选择的颜色
	}
	else if (m_setClrDlg.DoModal() == IDCANCEL)
	{
		m_clr = m_setClrDlg.GetColor();
	}

	mfcBtnColor.SetFaceColor(m_clr);
	int i = LabelList.GetCurSel();
	color_index[i][0] = BYTE(m_clr);
	color_index[i][1] = BYTE(m_clr>>8);
	color_index[i][2] = BYTE(m_clr>>16);

}


void Ctld_trackDlg::OnStnClickedStaticLabel()
{
	// TODO: 在此添加控件通知处理程序代码

	//mLabelManager.MFC_Label_lst
	mLabelManager.DoModal();
}

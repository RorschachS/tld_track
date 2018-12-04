
// tld_track.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "tld_track.h"
#include "tld_trackDlg.h"
#include "license_warning.h"
#include "CSendMail.h"

#include "HMAC_MD5_API.h"
#include "BASE64_API.h"
#include<intrin.h>

#include <winsock2.h>   //该头文件定义了Socket编程的功能  
#include <Nb30.h>       //该头文件声明了netbios的所有的函数 
#include <stdio.h>      
#include <stdlib.h>    
#pragma comment(lib,"netapi32.lib")   //连接Netapi32.lib库，MAC获取中用到了NetApi32.DLL的功能 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ctld_trackApp

BEGIN_MESSAGE_MAP(Ctld_trackApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	
END_MESSAGE_MAP()

// Ctld_trackApp 构造

Ctld_trackApp::Ctld_trackApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

}


// 唯一的一个 Ctld_trackApp 对象

Ctld_trackApp theApp;


// Ctld_trackApp 初始化

BOOL Ctld_trackApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//std::string str = theApp.GetCPUID();
	//MD5 md5 = MD5(str);
	//std::string strHash = md5.hexdigest();
	////printf(strHash);// << "哈希摘要：" << strHash << endl;


	char addstr[20] = "hbdqwfdf32tg";

	char   MAC[18];
	getMAC(MAC);   //调用getMAC()函数获得，输出MAC地址  
	//printf(" MAC物理地址 : %s", MAC);
	string mac = MAC;
	TCHAR b[40];
	TCHAR md5_cpu[40];


	INT32 CpuBuf[4];
	__cpuidex(CpuBuf, 01, 0);

	

	
	string cpuid = GetCPUID();
	char cpu[18];
	int i;
	for (i = 0; i < cpuid.length(); i++)
		cpu[i] = cpuid[i];
	cpu[i] = '\0';

	HMAC_MD5_BASE64((const BYTE*)MAC, sizeof(MAC), (const BYTE*)addstr, sizeof(addstr), b);
	HMAC_MD5_BASE64((const BYTE*)cpu, sizeof(cpu), (const BYTE*)addstr, sizeof(addstr), md5_cpu);
	//printf("b=%s,sizeof(mac)=%d\n", b, sizeof(MAC));
	string bb = TCHAR2STRING(b);
	char   license_dir[MAX_PATH];
	getcwd(license_dir, MAX_PATH);
	string license_name = "\\license.dat";
	string license = license_dir + license_name;
	string cc=readLicense(license);
	

	//if (cc.compare("-1")==0)
	//{
	//	AfxMessageBox(_T("未找到License文件！"));
	//	return FALSE;
	//}
	//else if (bb.compare(cc)!=0)
	//{
	//	license_warning warning_dlg;
	//	INT_PTR warning_dlg_flag = warning_dlg.DoModal();
	//	if (warning_dlg_flag == 2)
	//		return FALSE;
	//	else if (warning_dlg_flag == 1)
	//	{ 
	//		sendmail(cpu);
	//		return FALSE;
	//	}
	//}


	Ctld_trackDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


string Ctld_trackApp::GetCPUID()
{
	string strCPUId;
	unsigned long s1, s2;
	char buf[32] = { 0 };
	__asm {
		mov eax, 01h   //eax=1:取CPU序列号
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, eax
	}
	if (s1) {
		memset(buf, 0, 32);
		sprintf_s(buf, 32, "%08X", s1);
		strCPUId += buf;
	}
	if (s2) {
		memset(buf, 0, 32);
		sprintf_s(buf, 32, "%08X", s2);
		strCPUId += buf;
	}
	__asm {
		mov eax, 03h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, ecx
	}
	if (s1) {
		memset(buf, 0, 32);
		sprintf_s(buf, 32, "%08X", s1);
		strCPUId += buf;
	}
	if (s2) {
		memset(buf, 0, 32);
		sprintf_s(buf, 32, "%08X", s2);
		strCPUId += buf;
	}
	return strCPUId;
}


int  Ctld_trackApp::getMAC(char * mac)
{
	NCB ncb;     //定义一个NCB(网络控制块)类型的结构体变量ncb  
	typedef struct _ASTAT_     //自定义一个结构体_ASTAT_  
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, *PASTAT;
	ASTAT Adapter;

	typedef struct _LANA_ENUM     //自定义一个结构体_LANA_ENUM  
	{
		UCHAR length;
		UCHAR lana[MAX_LANA];     //存放网卡MAC地址   
	}LANA_ENUM;
	LANA_ENUM lana_enum;

	//取得网卡信息列表       
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));     //将已开辟内存空间ncb 的值均设为值 0  
	memset(&lana_enum, 0, sizeof(lana_enum));     //清空一个结构类型的变量lana_enum，赋值为0  
												  //对结构体变量ncb赋值  
	ncb.ncb_command = NCBENUM;     //统计系统中网卡的数量  
	ncb.ncb_buffer = (unsigned char *)&lana_enum; //ncb_buffer成员指向由LANA_ENUM结构填充的缓冲区  
	ncb.ncb_length = sizeof(LANA_ENUM);
	//向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡，每个网卡的编号（MAC地址）   
	uRetCode = Netbios(&ncb); //调用netbois(ncb)获取网卡序列号      
	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	//对每一个网卡，以其网卡编号为输入编号，获取其MAC地址     
	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;   //对网卡发送NCBRESET命令，进行初始化  
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb);
	}
	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	//   准备取得接口卡的状态块取得MAC地址  
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;    //对网卡发送NCBSTAT命令，获取网卡信息  
	ncb.ncb_lana_num = lana_enum.lana[0];     //指定网卡号，这里仅仅指定第一块网卡，通常为有线网卡   
	strcpy((char*)ncb.ncb_callname, "*");     //远程系统名赋值为*  
	ncb.ncb_buffer = (unsigned char *)&Adapter; //指定返回的信息存放的变量  
	ncb.ncb_length = sizeof(Adapter);
	//接着发送NCBASTAT命令以获取网卡的信息  
	uRetCode = Netbios(&ncb);
	//取得网卡的信息，并且如果网卡正常工作的话，返回标准的冒号分隔格式。     
	if (uRetCode != NRC_GOODRET)
		return uRetCode;
	//把网卡MAC地址格式转化为常用的16进制形式,输出到字符串mac中   
	sprintf(mac, "%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]
	);
	return 0;
}

string Ctld_trackApp::readLicense(string file)
{
	ifstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string cc;
	char c;
	if (infile.is_open()) {

		while (infile.peek() != EOF)
		{
			//if (!infile.eof())break;
			infile >> c;
			//buf.put(c);
			cc.push_back(c);

		}
		//license = buf.str();
		infile.close();
	}
	else
	{
		cc="-1";
	}
	return cc;
}

/******************************************************************************************
Function:        TCHAR2STRING
Description:     TCHAR转string
Input:           str:待转化的TCHAR*类型字符串
Return:          转化后的string类型字符串
*******************************************************************************************/
std::string Ctld_trackApp::TCHAR2STRING(TCHAR* str)
{
	std::string strstr;
	try
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

		char* chRtn = new char[iLen * sizeof(char)];

		WideCharToMultiByte(CP_ACP, 0, str, -1, chRtn, iLen, NULL, NULL);

		strstr = chRtn;
	}
	catch (std::exception e)
	{
	}

	return strstr;
}


void Ctld_trackApp::sendmail(char* msg)
{
	sMailInfo sm;
	sm.m_pcUserName = "731226277";//"";
	sm.m_pcUserPassWord = "SJ20170080...";// "";
	sm.m_pcSenderName = "doge";
	sm.m_pcSender = "731226277@qq.com";//发送者的邮箱地址  
	sm.m_pcReceiver = "731226277@qq.com";//接收者的邮箱地址  
	sm.m_pcTitle = "测试邮件";//邮箱标题  
	sm.m_pcBody = msg;
	sm.m_pcIPAddr = "";//服务器的IP可以留空 
	sm.m_pcIPName = "smtp.qq.com";//服务器的名称（IP与名称二选一，优先取名称）  

	CSendMail csm;
	csm.SendMail(sm);

}


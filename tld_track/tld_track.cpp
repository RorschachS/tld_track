
// tld_track.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "tld_track.h"
#include "tld_trackDlg.h"
#include "license_warning.h"
#include "CSendMail.h"

#include "HMAC_MD5_API.h"
#include "BASE64_API.h"
#include<intrin.h>

#include <winsock2.h>   //��ͷ�ļ�������Socket��̵Ĺ���  
#include <Nb30.h>       //��ͷ�ļ�������netbios�����еĺ��� 
#include <stdio.h>      
#include <stdlib.h>    
#pragma comment(lib,"netapi32.lib")   //����Netapi32.lib�⣬MAC��ȡ���õ���NetApi32.DLL�Ĺ��� 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ctld_trackApp

BEGIN_MESSAGE_MAP(Ctld_trackApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	
END_MESSAGE_MAP()

// Ctld_trackApp ����

Ctld_trackApp::Ctld_trackApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

}


// Ψһ��һ�� Ctld_trackApp ����

Ctld_trackApp theApp;


// Ctld_trackApp ��ʼ��

BOOL Ctld_trackApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//std::string str = theApp.GetCPUID();
	//MD5 md5 = MD5(str);
	//std::string strHash = md5.hexdigest();
	////printf(strHash);// << "��ϣժҪ��" << strHash << endl;


	char addstr[20] = "hbdqwfdf32tg";

	char   MAC[18];
	getMAC(MAC);   //����getMAC()������ã����MAC��ַ  
	//printf(" MAC�����ַ : %s", MAC);
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
	//	AfxMessageBox(_T("δ�ҵ�License�ļ���"));
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


string Ctld_trackApp::GetCPUID()
{
	string strCPUId;
	unsigned long s1, s2;
	char buf[32] = { 0 };
	__asm {
		mov eax, 01h   //eax=1:ȡCPU���к�
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
	NCB ncb;     //����һ��NCB(������ƿ�)���͵Ľṹ�����ncb  
	typedef struct _ASTAT_     //�Զ���һ���ṹ��_ASTAT_  
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, *PASTAT;
	ASTAT Adapter;

	typedef struct _LANA_ENUM     //�Զ���һ���ṹ��_LANA_ENUM  
	{
		UCHAR length;
		UCHAR lana[MAX_LANA];     //�������MAC��ַ   
	}LANA_ENUM;
	LANA_ENUM lana_enum;

	//ȡ��������Ϣ�б�       
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));     //���ѿ����ڴ�ռ�ncb ��ֵ����Ϊֵ 0  
	memset(&lana_enum, 0, sizeof(lana_enum));     //���һ���ṹ���͵ı���lana_enum����ֵΪ0  
												  //�Խṹ�����ncb��ֵ  
	ncb.ncb_command = NCBENUM;     //ͳ��ϵͳ������������  
	ncb.ncb_buffer = (unsigned char *)&lana_enum; //ncb_buffer��Աָ����LANA_ENUM�ṹ���Ļ�����  
	ncb.ncb_length = sizeof(LANA_ENUM);
	//����������NCBENUM����Ի�ȡ��ǰ������������Ϣ�����ж��ٸ�������ÿ�������ı�ţ�MAC��ַ��   
	uRetCode = Netbios(&ncb); //����netbois(ncb)��ȡ�������к�      
	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	//��ÿһ�������������������Ϊ�����ţ���ȡ��MAC��ַ     
	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;   //����������NCBRESET������г�ʼ��  
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb);
	}
	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	//   ׼��ȡ�ýӿڿ���״̬��ȡ��MAC��ַ  
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;    //����������NCBSTAT�����ȡ������Ϣ  
	ncb.ncb_lana_num = lana_enum.lana[0];     //ָ�������ţ��������ָ����һ��������ͨ��Ϊ��������   
	strcpy((char*)ncb.ncb_callname, "*");     //Զ��ϵͳ����ֵΪ*  
	ncb.ncb_buffer = (unsigned char *)&Adapter; //ָ�����ص���Ϣ��ŵı���  
	ncb.ncb_length = sizeof(Adapter);
	//���ŷ���NCBASTAT�����Ի�ȡ��������Ϣ  
	uRetCode = Netbios(&ncb);
	//ȡ����������Ϣ����������������������Ļ������ر�׼��ð�ŷָ���ʽ��     
	if (uRetCode != NRC_GOODRET)
		return uRetCode;
	//������MAC��ַ��ʽת��Ϊ���õ�16������ʽ,������ַ���mac��   
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
	infile.open(file.data());   //���ļ����������ļ��������� 
	//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

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
Description:     TCHARתstring
Input:           str:��ת����TCHAR*�����ַ���
Return:          ת�����string�����ַ���
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
	sm.m_pcSender = "731226277@qq.com";//�����ߵ������ַ  
	sm.m_pcReceiver = "731226277@qq.com";//�����ߵ������ַ  
	sm.m_pcTitle = "�����ʼ�";//�������  
	sm.m_pcBody = msg;
	sm.m_pcIPAddr = "";//��������IP�������� 
	sm.m_pcIPName = "smtp.qq.com";//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  

	CSendMail csm;
	csm.SendMail(sm);

}


// config.cpp: 实现文件
//
//#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "LoadCode.h"
#include "config.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;
// config 对话框

IMPLEMENT_DYNAMIC(config, CDialogEx)

config::config(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

config::~config()
{
}

void config::DoDataExchange(CDataExchange* pDX) 
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST3, m_list3);
	DDX_Control(pDX, IDC_LIST4, m_tool_list);
	DDX_Control(pDX, IDC_LIST5, m_fir_list);
}


BEGIN_MESSAGE_MAP(config, CDialogEx)
	ON_BN_CLICKED(AVB_EXE_BTN, &config::OnBnClickedBtn)
	ON_BN_CLICKED(AT_PERM_BTN, &config::OnBnClickedPermBtn)
	ON_BN_CLICKED(AT_RSA_PERM_BTN, &config::OnBnClickedRsaPermBtn)
	ON_BN_CLICKED(IDC_OK_BTN, &config::OnBnClickedOkBtn)
	ON_BN_CLICKED(IDC_ESC_BTN, &config::OnBnClickedEscBtn)
	ON_BN_CLICKED(TOOL_EXE_BTN, &config::OnBnClickedExeBtn)
	ON_BN_CLICKED(FIRMWARE_BTN3, &config::OnBnClickedBtn3)
END_MESSAGE_MAP()


// config 消息处理程序


//路径解析，单斜杠转双斜杠
void PathConversion(string &s)
{
	string::size_type pos = 0;
	while ((pos = s.find('\\', pos)) != string::npos)
	{
		s.insert(pos, "\\");
		pos = pos + 2;
	}
}

//弹框选择文件路径
string choosePath(CListBox & m_list, CString message)
{
	CFileDialog dlgFile(TRUE, NULL, NULL,
		OFN_HIDEREADONLY, _T("ALL Files(*.*)|*.*||"), NULL);//选择文件路径

	dlgFile.m_ofn.lpstrTitle = message;//文件打开对话框标题
	string str;
	if (IDOK == dlgFile.DoModal())//判断是否成功选中文件
	{
		m_list.ResetContent();
		CString strLocalFilePath_Temp;
		strLocalFilePath_Temp = dlgFile.GetPathName();
		//m_list.AddString(dlgFile.GetFolderPath());
		m_list.AddString(strLocalFilePath_Temp);
		str = CW2A(strLocalFilePath_Temp.GetString());
		//PathConversion(str);
		//exe_path = str;
		//AfxMessageBox(exe_path);
	}
	return str;
}

void config::OnBnClickedExeBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	tool_exe_path = choosePath(m_tool_list, _T("AndroidTool.exe程序路径"));
}


void config::OnBnClickedBtn3()
{
	// TODO: 在此添加控件通知处理程序代码

	firmware_path = choosePath(m_fir_list, _T("选择固件路径"));
}


void config::OnBnClickedBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	exe_path = choosePath(m_list1, _T("选择AvbKeyWriter.exe程序路径"));
	
}


void config::OnBnClickedPermBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	at_perm_path = choosePath(m_list2, _T("选择烧录文件路径"));
}


void config::OnBnClickedRsaPermBtn()
{
	at_rsa_perm_path = choosePath(m_list3, _T("选择烧录文件路径"));
	// TODO: 在此添加控件通知处理程序代码
	/*CFileDialog dlgFile(TRUE, NULL, NULL,
		OFN_HIDEREADONLY, _T("ALL Files(*.*)|*.*||"), NULL);//选择文件路径

	dlgFile.m_ofn.lpstrTitle = _T("烧录文件选择路径");//文件打开对话框标题
	if (IDOK == dlgFile.DoModal())//判断是否成功选中文件  
	{
		CString strLocalFilePath_Temp;
		strLocalFilePath_Temp = dlgFile.GetPathName();
		m_list3.AddString(strLocalFilePath_Temp);
		string str(CW2A(strLocalFilePath_Temp.GetString()));
		//PathConversion(str);
		at_rsa_perm_path = str;
	}
	return;*/
}

void config::OnBnClickedOkBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开文件，将路径写入文件
	FILE *fp;
	errno_t err;

	err = fopen_s(&fp, "path.xml", "w");
	if (err == 0)
	{
		char str[1024];
		memset(str, 0, sizeof(str));

		sprintf_s(str, R"(<path>
			<tool_exe_path>%s</tool_exe_path>
			<firmware_path>%s</firmware_path>
			<exe_path>%s</exe_path>
			<at_perm_path>%s</at_perm_path>
			<at_rsa_perm_path>%s</at_rsa_perm_path>
			</path>)", 
			tool_exe_path.c_str(), firmware_path.c_str(),exe_path.c_str(),
			at_perm_path.c_str(), at_rsa_perm_path.c_str());

		fprintf(fp, "%s", str);

		fclose(fp);
	}
	else
	{
		AfxMessageBox(_T("xml文件打开失败"));
		return;
	}


	EndDialog(0);
}


void config::OnBnClickedEscBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	tool_exe_path.empty();//固件程序的运行路径
	firmware_path.empty();//固件的路径
	exe_path.empty();//AvbKeyWriter程序的运行路径
	at_perm_path.empty();//烧录文件1路径
	at_rsa_perm_path.empty();//烧录文件2路径
	EndDialog(0);
}

BOOL config::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_TAB))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);

}




// LoadCodeDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LoadCode.h"
#include "LoadCodeDlg.h"
#include "afxdialogex.h"
#include "config.h"
#include "Passwd_Exit.h"
#include "passwd.h"
#include "python.h"


#include <string.h>
#include <string>
#include <thread>
#include <iostream>
#include <windows.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// config 对话框
string tool_exe_path;//固件程序的运行路径
string firmware_path;//固件的路径
string exe_path;//AvbKeyWriter程序的运行路径
string at_perm_path;//烧录文件1路径
string at_rsa_perm_path;//烧录文件2路径

//执行python脚本
void python_start();



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


// CLoadCodeDlg 对话框



CLoadCodeDlg::CLoadCodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADCODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START, m_start_btn);
}

BEGIN_MESSAGE_MAP(CLoadCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CLoadCodeDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_CLOSE, &CLoadCodeDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_CONFIG, &CLoadCodeDlg::OnBnClickedConfig)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLoadCodeDlg 消息处理程序

BOOL CLoadCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoadCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CLoadCodeDlg::OnPaint()
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
HCURSOR CLoadCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
///////////////////////////////////////////////////////////////////
thread t1;
void CLoadCodeDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (exe_path.IsEmpty() || at_perm_path.IsEmpty() || at_rsa_perm_path.IsEmpty())
	{
		AfxMessageBox(_T("请配置config！"));
		return;
	}*/
	FILE * fp;
	errno_t err;
	err = fopen_s(&fp, "path.xml", "r");
	if (err != 0)//文件打开失败
	{
		AfxMessageBox(_T("请输入烧录文件路径！"));
		return;
	}
	else
	{
		t1 = thread(python_start);
		thread::id gid = t1.get_id();
		t1.detach();
		
		
		// 解决颜色显示问题
		m_start_btn.m_bTransparent = FALSE;
		m_start_btn.m_bDontUseWinXPTheme = TRUE;
		// 消除黑圈
		m_start_btn.m_bDrawFocus = FALSE;
		// 消除边框
		m_start_btn.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
		// 设置颜色
		m_start_btn.SetFaceColor(RGB(0, 255, 0), true);
		m_start_btn.EnableWindow(FALSE);//然后按键失效

		/*while (true)
		{
			//此时调用的python程序挂掉，重启python程序
			if (gid != t1.get_id())
			{
				t1 = thread(python_start);
				t1.detach();
				return;
			}
		}*/
	}
	

}


void CLoadCodeDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	Passwd_Exit dlg;
	dlg.DoModal();
	
	//m_start_btn.EnableWindow(TRUE);//然后按键失效还原
	// 设置颜色灰色
	//m_start_btn.SetFaceColor(RGB(225, 225, 225), true);

}

void CLoadCodeDlg::OnBnClickedConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	passwd dlg;
	dlg.DoModal(); 
}

//这个函数用来处理按键的操作
BOOL CLoadCodeDlg::PreTranslateMessage(MSG * pMsg)
{
	/*if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}*/
	//处理esc的按键操作
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		Passwd_Exit dlg;
		dlg.DoModal();
		return TRUE;
	}
	/*if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_TAB))
	{
		return TRUE;
	}*/

	return CDialogEx::PreTranslateMessage(pMsg);

}

////这个函数处理退出的操作
void CLoadCodeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Passwd_Exit dlg;
	dlg.DoModal();
	//CDialogEx::OnClose();
}

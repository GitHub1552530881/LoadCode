// Passwd_Exit.cpp: 实现文件
//

#include "pch.h"
#include "LoadCode.h"
#include "Passwd_Exit.h"
#include "afxdialogex.h"
#include "LoadCodeDlg.h"
#include <thread>
using namespace std;

//终止python程序
void python_kill();

// Passwd_Exit 对话框

IMPLEMENT_DYNAMIC(Passwd_Exit, CDialogEx)

Passwd_Exit::Passwd_Exit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXIT_PASSWD, pParent)
{

}

Passwd_Exit::~Passwd_Exit()
{
}

void Passwd_Exit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(Passwd_Exit, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &Passwd_Exit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ESC, &Passwd_Exit::OnBnClickedEsc)
END_MESSAGE_MAP()


// Passwd_Exit 消息处理程序


void Passwd_Exit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT1, str);

	//密码正确可以关闭程序
	if (str == "sa")
	{
		python_kill();
		exit(-1);
	}
	else
	{
		AfxMessageBox(_T("密码错误，请重新输入！"));
		return;
	}
}


void Passwd_Exit::OnBnClickedEsc()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);

	//return;
}

BOOL Passwd_Exit::PreTranslateMessage(MSG * pMsg)
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

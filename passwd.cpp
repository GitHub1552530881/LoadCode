// passwd.cpp: 实现文件
//

#include "pch.h"
#include "LoadCode.h"
#include "passwd.h"
#include "afxdialogex.h"
#include "config.h"


// passwd 对话框

IMPLEMENT_DYNAMIC(passwd, CDialogEx)

passwd::passwd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PASSWD, pParent)
{

}

passwd::~passwd()
{
}

void passwd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}


BEGIN_MESSAGE_MAP(passwd, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &passwd::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ESC, &passwd::OnBnClickedEsc)
END_MESSAGE_MAP()


// passwd 消息处理程序


void passwd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	config dig;

	if (str == "sa")//密码正确
	{
		
		dig.DoModal();
		EndDialog(0);
	}
	else
	{
		AfxMessageBox(_T("密码错误，请重新输入！"));
		return;
	}
}


void passwd::OnBnClickedEsc()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}

//屏蔽确认键
BOOL passwd::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	/*if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}*/
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_TAB))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);

}

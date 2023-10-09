#pragma once
#include <string.h>
#include <string>
using namespace std;

// config 对话框
extern string tool_exe_path;//固件程序的运行路径
extern string firmware_path;//固件的路径
extern string exe_path;//AvbKeyWriter程序的运行路径
extern string at_perm_path;//烧录文件1路径
extern string at_rsa_perm_path;//烧录文件2路径

class config : public CDialogEx
{
	DECLARE_DYNAMIC(config)

public:
	config(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~config();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtn();
	afx_msg void OnBnClickedPermBtn();
	afx_msg void OnBnClickedRsaPermBtn();
	CListBox m_list1;
	CListBox m_list2;
	CListBox m_list3;
	CListBox m_tool_list;
	CListBox m_fir_list;
	afx_msg void OnBnClickedOkBtn();
	afx_msg void OnBnClickedEscBtn();
	afx_msg void OnBnClickedExeBtn();
	afx_msg void OnBnClickedBtn3();


	virtual BOOL PreTranslateMessage(MSG* pMsg);

	
};

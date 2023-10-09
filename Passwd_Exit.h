#pragma once

// Passwd_Exit 对话框

class Passwd_Exit : public CDialogEx
{
	DECLARE_DYNAMIC(Passwd_Exit)

public:
	Passwd_Exit(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Passwd_Exit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXIT_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEsc();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#pragma once


// passwd 对话框

class passwd : public CDialogEx
{
	DECLARE_DYNAMIC(passwd)

public:
	passwd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~passwd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEsc();
	CEdit m_edit1;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

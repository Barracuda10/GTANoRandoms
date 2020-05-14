#pragma once
#include "afxwin.h"


// Log 对话框

class Log : public CDialogEx
{
	DECLARE_DYNAMIC(Log)

public:
	Log(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Log();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_LogContent;
	virtual BOOL OnInitDialog();
	CEdit m_LogCtrl;
	afx_msg void OnEnSetfocusLog();
	CFont m_font;
};

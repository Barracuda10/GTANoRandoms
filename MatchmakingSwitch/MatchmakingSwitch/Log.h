#pragma once
#include "afxwin.h"


// Log dialog

class Log : public CDialogEx
{
	DECLARE_DYNAMIC(Log)

public:
	Log(CWnd* pParent = NULL);   // standard constructor
	virtual ~Log();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_LogContent;
	virtual BOOL OnInitDialog();
	CEdit m_LogCtrl;
	afx_msg void OnEnSetfocusLog();
	CFont m_font;
};

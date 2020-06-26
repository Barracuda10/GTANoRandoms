#pragma once
#include "afxwin.h"


// Manual dialog

class Manual : public CDialogEx
{
	DECLARE_DYNAMIC(Manual)

public:
	Manual(CWnd* pParent = NULL);   // standard constructor
	virtual ~Manual();
private:
	Manual *pManual;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUAL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont m_font;
	CString m_help;
	afx_msg void OnEnSetfocusLog();
	afx_msg void OnClose();
	CEdit m_helpCtrl;
};

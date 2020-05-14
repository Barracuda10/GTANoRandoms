#pragma once
#include "afxwin.h"


// Manual 对话框

class Manual : public CDialogEx
{
	DECLARE_DYNAMIC(Manual)

public:
	Manual(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Manual();
private:
	Manual *pManual;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUAL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont m_font;
	CString m_help;
	afx_msg void OnEnSetfocusLog();
	afx_msg void OnClose();
	CEdit m_helpCtrl;
};

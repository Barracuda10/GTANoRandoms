#pragma once
#include "afxwin.h"


// Log �Ի���

class Log : public CDialogEx
{
	DECLARE_DYNAMIC(Log)

public:
	Log(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Log();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_LogContent;
	virtual BOOL OnInitDialog();
	CEdit m_LogCtrl;
	afx_msg void OnEnSetfocusLog();
	CFont m_font;
};

#pragma once
#include "afxwin.h"


// Manual �Ի���

class Manual : public CDialogEx
{
	DECLARE_DYNAMIC(Manual)

public:
	Manual(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Manual();
private:
	Manual *pManual;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUAL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont m_font;
	CString m_help;
	afx_msg void OnEnSetfocusLog();
	afx_msg void OnClose();
	CEdit m_helpCtrl;
};

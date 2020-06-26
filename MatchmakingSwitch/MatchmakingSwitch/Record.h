#pragma once
#include "afxcmn.h"


// Record dialog

class Record : public CDialogEx
{
	DECLARE_DYNAMIC(Record)

public:
	Record(CWnd* pParent = NULL);   // standard constructor
	virtual ~Record();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECORD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString input_key;
	int input_vk_code;
	int input_modifiers;
	afx_msg void OnBnClickedOk();
	CHotKeyCtrl m_Recorder;
	CString m_RecordLabel;
	virtual BOOL OnInitDialog();
	CFont m_Font;
	afx_msg void OnSetfocusEdit1();
	CString m_alert;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnEnSetfocusAlert();
	CString previous_hotkey;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

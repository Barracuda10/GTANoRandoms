#pragma once
#include "afxcmn.h"


// Record 对话框

class Record : public CDialogEx
{
	DECLARE_DYNAMIC(Record)

public:
	Record(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Record();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECORD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
};

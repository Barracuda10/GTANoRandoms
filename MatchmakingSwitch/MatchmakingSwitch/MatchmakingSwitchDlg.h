
// MatchmakingSwitchDlg.h : header file
//

#pragma once
#include "afxbutton.h"
#include "afxwin.h"
#include "Log.h"
#include "Manual.h"


// CMatchmakingSwitchDlg dialog
class CMatchmakingSwitchDlg : public CDialogEx
{
// Construction
public:
	CMatchmakingSwitchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MATCHMAKINGSWITCH_DIALOG, IDH = IDR_HTML_MATCHMAKINGSWITCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	Log *pLog;
	Manual *pManual;
public:
	afx_msg void OnOptionsChangehotkey();
	int get_vk_code;
	int get_modifiers;
	CString get_hotkey;
	CString get_LogContent;
	afx_msg void OnOptionsLog();
	afx_msg void OnClose();
	virtual void PreInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnDestroy();
	CMFCButton m_Button;
	CFont m_font;
	CString m_Process;
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnBnSetfocusButton1();
	CFont m_font_edit;
	afx_msg void OnBnClickedButton1();
	virtual void OnOK();
	afx_msg void OnHelpManual();
	afx_msg void OnHelpEdithostsfile();
	~CMatchmakingSwitchDlg();
	CString path;
	CEdit m_ProcessCtrl;
	UINT turnoffSoundState;
	UINT turnonSoundState;
	afx_msg void OnOptionsTurnoffsound();
	afx_msg void OnOptionsTurnonsound();
	CBrush m_brush;
	CFont m_font_button;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	int buttonWidthDiff;
	int buttonHeightDiff;
	int messageWidthDiff;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

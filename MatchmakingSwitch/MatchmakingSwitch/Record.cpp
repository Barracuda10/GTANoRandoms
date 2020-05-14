// Record.cpp : 实现文件
//

#include "stdafx.h"
#include "MatchmakingSwitch.h"
#include "Record.h"
#include "afxdialogex.h"


// CRecord 对话框

IMPLEMENT_DYNAMIC(Record, CDialogEx)

Record::Record(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RECORD_DIALOG, pParent)
	, input_vk_code(0)
	, input_modifiers(0)
	, m_RecordLabel(_T(""))
	, m_alert(_T(""))
	, previous_hotkey(_T(""))
{

}

Record::~Record()
{
}

void Record::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY1, m_Recorder);
	DDX_Text(pDX, IDC_EDIT1, m_RecordLabel);
	DDX_Text(pDX, IDC_ALERT, m_alert);
}


BEGIN_MESSAGE_MAP(Record, CDialogEx)
	ON_BN_CLICKED(IDOK, &Record::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT1, &Record::OnSetfocusEdit1)
	ON_WM_HELPINFO()
	ON_EN_SETFOCUS(IDC_ALERT, &Record::OnEnSetfocusAlert)
END_MESSAGE_MAP()


// CRecord 消息处理程序


void Record::OnBnClickedOk()
{
	OnEnSetfocusAlert();

	if (previous_hotkey != m_RecordLabel) {
		CHotKeyCtrl* hotkey = (CHotKeyCtrl*)GetDlgItem(IDC_HOTKEY1);
		WORD vk_Code, Modifiers;
		hotkey->GetHotKey(vk_Code, Modifiers);
		input_key = m_Recorder.GetHotKeyName();
		input_vk_code = vk_Code;
		input_modifiers = Modifiers;
	}
	
	CDialogEx::OnOK();
}


BOOL Record::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 24;
	StrCpyW(lf.lfFaceName, L"Impact");
	m_Font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_EDIT1)->SetFont(&m_Font);
	GetDlgItem(IDC_HOTKEY1)->SetFont(&m_Font);

	UpdateData(true);
	m_alert = _T("Click to change key");
	UpdateData(false);

	GetDlgItem(IDC_HOTKEY1)->EnableWindow(FALSE);
	GetDlgItem(IDC_HOTKEY1)->ShowWindow(SW_HIDE);
	
	previous_hotkey = m_RecordLabel;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Record::OnSetfocusEdit1()
{
	GetDlgItem(IDC_HOTKEY1)->EnableWindow(TRUE);
	GetDlgItem(IDC_HOTKEY1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HOTKEY1)->SetFocus();

	UpdateData(true);
	m_alert = _T("Recording...");
	UpdateData(false);
}


BOOL Record::OnHelpInfo(HELPINFO* pHelpInfo)
{
	return TRUE;
}


void Record::OnEnSetfocusAlert()
{
	::HideCaret(GetDlgItem(IDC_ALERT)->GetSafeHwnd());

	if (GetDlgItem(IDC_HOTKEY1)->IsWindowEnabled()) {
		CHotKeyCtrl* hotkey = (CHotKeyCtrl*)GetDlgItem(IDC_HOTKEY1);
		WORD vk_Code, Modifiers;
		hotkey->GetHotKey(vk_Code, Modifiers);
		CString display_key = m_Recorder.GetHotKeyName();
		GetDlgItem(IDC_HOTKEY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_HOTKEY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);

		if (vk_Code == 0x00) {
			display_key = "None";
		}
		if (vk_Code == 0x13) {
			display_key = "Pause";
		}
		UpdateData(true);
		m_RecordLabel = display_key;

		/*
		available
		get_modifiers == 1 Shift + *
		get_modifiers == 3 Shift + Ctrl + *
		get_modifiers == 4 Alt + *
		get_modifiers == 6 Ctrl + Alt + *

		Unavailable
		get_modifiers == 2 Ctrl + *
		get_modifiers == 5 Shift + Alt + *
		get_modifiers == 7 Ctrl + Shift + Alt + *
		*/
		if (vk_Code >= 0x21 && vk_Code <= 0x2D || Modifiers == 1 || Modifiers == 3 || Modifiers == 4 || Modifiers == 6) {
			m_alert = _T("This key may not work!!!");
		}
		else {
			m_alert = _T("Click to change key");
		}
		UpdateData(false);

		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
	}
}

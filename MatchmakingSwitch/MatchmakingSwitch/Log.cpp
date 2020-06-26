// Log.cpp : implementation file
//

#include "stdafx.h"
#include "MatchmakingSwitch.h"
#include "Log.h"
#include "afxdialogex.h"


// Log dialog

IMPLEMENT_DYNAMIC(Log, CDialogEx)

Log::Log(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOG_DIALOG, pParent)
	, m_LogContent(_T(""))
{
	//Register Class
	WNDCLASS wc;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("MatchmakingSwitchLog");
	AfxRegisterClass(&wc);
}

Log::~Log()
{
}

void Log::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOG, m_LogContent);
	DDX_Control(pDX, IDC_LOG, m_LogCtrl);
}


BEGIN_MESSAGE_MAP(Log, CDialogEx)
	ON_EN_SETFOCUS(IDC_LOG, &Log::OnEnSetfocusLog)
END_MESSAGE_MAP()


// Log message handlers


BOOL Log::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LOGFONT lf;//Set font
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 18;
	StrCpyW(lf.lfFaceName, L"Calibri");
	m_font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_LOG)->SetFont(&m_font);

	m_LogCtrl.LineScroll(m_LogCtrl.GetLineCount(), 0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Log::OnEnSetfocusLog()
{
	m_LogCtrl.SetSel(-1, -1);
	::HideCaret(GetDlgItem(IDC_LOG)->GetSafeHwnd());
}


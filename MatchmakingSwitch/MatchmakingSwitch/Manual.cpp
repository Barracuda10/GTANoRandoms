// Manual.cpp : 实现文件
//

#include "stdafx.h"
#include "MatchmakingSwitch.h"
#include "Manual.h"
#include "afxdialogex.h"


// Manual 对话框

IMPLEMENT_DYNAMIC(Manual, CDialogEx)

Manual::Manual(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MANUAL_DIALOG, pParent)
	, m_help(_T(""))
{
	//Register Class
	WNDCLASS wc;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("MatchmakingSwitchManual");
	AfxRegisterClass(&wc);
}

Manual::~Manual()
{
}

void Manual::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOG, m_help);
	DDX_Control(pDX, IDC_LOG, m_helpCtrl);
}


BEGIN_MESSAGE_MAP(Manual, CDialogEx)
	ON_EN_SETFOCUS(IDC_LOG, &Manual::OnEnSetfocusLog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Manual 消息处理程序


BOOL Manual::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LOGFONT lf;//Set font
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 18;
	StrCpyW(lf.lfFaceName, L"Calibri");
	m_font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_LOG)->SetFont(&m_font);

	UpdateData(true);
	m_help = "This is a tool can help you create a public session and allow anyone to join but except randoms.\r\n";
	m_help += "\r\nHow to create a No Randoms Session:\r\n";
	m_help += "-  Join a public session with other players in it and with matchmaking on.\r\n";
	m_help += "-  Make a solo session and after that wait for few seconds turn matchmaking off.\r\n";
	m_help += "-  Turn matchmaking back on before leaving this session or host a job.\r\n";
	m_help += "\r\nNotice:\r\n";
	m_help += "-  This program may require run as administrator.\r\n";
	m_help += "-  Due to game default set matchmaking on, it will automatically turn matchmaking back on when starting it and closing it.\r\n";
	m_help += "-  This program will not work when using VPN or proxy.\r\n";

	/*WINDOWPLACEMENT wndpl, wndpl_parent;
	GetWindowPlacement(&wndpl_parent);
	GetWindowPlacement(&wndpl);

	CString temp;
	temp.Format(_T("%d"), wndpl.rcNormalPosition.bottom - wndpl_parent.rcNormalPosition.top);
	m_help += temp + _T(" ");
	temp.Format(_T("%d"), wndpl.rcNormalPosition.right - wndpl_parent.rcNormalPosition.left);
	m_help += temp;*/


	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Manual::OnEnSetfocusLog()
{
	m_helpCtrl.SetSel(0, 0);
	::HideCaret(GetDlgItem(IDC_LOG)->GetSafeHwnd());
}


void Manual::OnClose()
{
	DestroyWindow();

	CDialogEx::OnClose();
}

// Manual.cpp : implementation file
//

#include "stdafx.h"
#include "MatchmakingSwitch.h"
#include "Manual.h"
#include "afxdialogex.h"


// Manual dialog

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


// Manual message handlers


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
	m_help = "This is a tool for creating a public session that allow friends to join but except random players.\r\n";
	//m_help += "Just click the button in the center or use hot key(default disabled) to activate.\r\n";
	m_help += "\r\nCreate a No Randoms Session:\r\n";
	/*m_help += "\r\nMethod 1 [RECOMMAND]\r\n";*/
	m_help += "-  Activate the tool, Go join a Public Session or Find a new session.\r\n";
	m_help += "-  It should be a empty session. (Usually it will join a friend, to avoid this enable Firewall Solo Session, then find a new session, it will be a empty session, Remember to disable it later)\r\n";
	m_help += "-  Done. Random players will not join this session.\r\n";
	m_help += "\r\nFirewall Solo Session:\r\n";
	m_help += "-  Enable it will block anyone from joining the session and disconnect anyone joined. No one can join while this is enabled.\r\n";
	/*m_help += "\r\nMETHOD 2\r\n";
	m_help += "-  Join a public session with other players in it and with matchmaking on.\r\n";
	m_help += "-  Make a solo session and after that wait for few seconds turn matchmaking off.\r\n";
	m_help += "-  Done.Randoms will not join this session.\r\n";*/
	m_help += "\r\nNote:\r\n";
	m_help += "-  This tool only work while system firewall is enabled.\r\n";
	m_help += "-  Always keep this tool activated while still in the session. Turn it off can sometimes cause randoms to join.\r\n";
	m_help += "-  Deactivate it before hosting a job or heist that requires to launch session.\r\n";
	m_help += "-  This tool might not work while using VPN or proxy.\r\n";
	m_help += "-  This tool will automatically deactivate itself before start and exit.\r\n";
	/*m_help += "-  Due to game default set matchmaking on, this program will automatically deactivate itself when starting it and closing it.\r\n";
	m_help += "-  This program require system firewall enabled, If can not enable system firewall, Use Mode 2 instead.\r\n";
	m_help += "-  Mode 2 may require run as administrator.\r\n";*/
	

	
	
	
	/*m_help += "\r\n\r\nAdvanced: Switch Cloud Services\r\n";
	m_help += "-  Choose Cloudservices Switch in options.\r\n";
	m_help += "-  Turn off Cloudservices can make the game temporarily unable to save game data.\r\n";
	m_help += "-  To discard this unsaved game data disconnect internet and let the game kick to story mode.\r\n";
	m_help += "-  To upload and save this unsaved game data just turn it back on.\r\n";
	m_help += "-  Due to game default set cloudservices on, this program will automatically turn cloudservices back on when starting it and closing it.\r\n";*/

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
				  // EXCEPTION: OCX Property Pages should return FALSE
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

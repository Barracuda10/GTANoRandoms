
// MatchmakingSwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MatchmakingSwitch.h"
#include "MatchmakingSwitchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//This header use to access other dialogs
#include "Record.h"
//#include "Log.h"
//#include "Manual.h"

//These headers use to play sound
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

//These headers use to set cursor
#define ID_HAND MAKEINTRESOURCE(32649)



// CMatchmakingSwitchDlg dialog

BEGIN_DHTML_EVENT_MAP(CMatchmakingSwitchDlg)
END_DHTML_EVENT_MAP()


CMatchmakingSwitchDlg::CMatchmakingSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MATCHMAKINGSWITCH_DIALOG, pParent)
	, get_vk_code(0)
	, get_modifiers(0)
	, get_hotkey(_T(""))
	, get_LogContent(_T(""))
	, m_Process(_T(""))
	, pLog(new Log())
	//, pManual(new Manual())
	, path(_T(""))
	, turnoffSoundState(0)
	, turnonSoundState(0)
	, buttonWidthDiff(0)
	, buttonHeightDiff(0)
	, messageWidthDiff(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMatchmakingSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button);
	DDX_Text(pDX, IDC_EDIT1, m_Process);
	DDX_Control(pDX, IDC_EDIT1, m_ProcessCtrl);
}

BEGIN_MESSAGE_MAP(CMatchmakingSwitchDlg, CDialogEx)

	ON_COMMAND(ID_OPTIONS_CHANGEHOTKEY, &CMatchmakingSwitchDlg::OnOptionsChangehotkey)
	ON_COMMAND(ID_OPTIONS_LOG, &CMatchmakingSwitchDlg::OnOptionsLog)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_WM_HOTKEY()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT1, &CMatchmakingSwitchDlg::OnEnSetfocusEdit1)
	ON_BN_SETFOCUS(IDC_BUTTON1, &CMatchmakingSwitchDlg::OnBnSetfocusButton1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMatchmakingSwitchDlg::OnBnClickedButton1)
	ON_COMMAND(ID_HELP_MANUAL, &CMatchmakingSwitchDlg::OnHelpManual)
	ON_COMMAND(ID_HELP_EDITHOSTSFILE, &CMatchmakingSwitchDlg::OnHelpEdithostsfile)
	ON_COMMAND(ID_OPTIONS_TURNOFFSOUND, &CMatchmakingSwitchDlg::OnOptionsTurnoffsound)
	ON_COMMAND(ID_OPTIONS_TURNONSOUND, &CMatchmakingSwitchDlg::OnOptionsTurnonsound)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMatchmakingSwitchDlg message handlers


BOOL CMatchmakingSwitchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	LOGFONT lf;//Set font
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 32;
	StrCpyW(lf.lfFaceName, L"Tahoma");
	m_font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);

	lf.lfHeight = 16;
	StrCpyW(lf.lfFaceName, L"Tahoma");
	m_font_edit.CreateFontIndirect(&lf);
	GetDlgItem(IDC_EDIT1)->SetFont(&m_font_edit);



	//Turn matchmaking back on
	UpdateData(true);
	SYSTEMTIME systime;
	CString time;
	bool first_line = true;

	TCHAR systemDir[128];//GetSystemDirectory
	GetSystemDirectory(systemDir, 128 * sizeof(TCHAR));
	TCHAR *next_token = NULL;
	TCHAR* path_t = _tcstok_s(systemDir, _T("\\"), &next_token);
	_tcscat_s(path_t, 128, _T("\\Windows\\System32\\drivers\\etc\\hosts"));
	path = path_t;

	TCHAR path_temp[128];
	_tcscpy_s(path_temp, path);

	CFile file;
	if (file.Open(path_temp, CFile::modeReadWrite) == 0) {
		MessageBox(_T("No Permission or File not Found, Please Run this Program as Administrator and Try Again"), NULL, MB_OK);
		pLog->m_LogContent += time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
		first_line = false;
		return 1;
	}
	int fileLength = file.GetLength();
	char* get_FileContent = new char[fileLength];
	file.Read(get_FileContent, fileLength);

	UpdateData(false);

	CString fileContent;
	fileContent.Format(_T("%S"), get_FileContent);
	free(get_FileContent);
	get_FileContent = NULL;

	if (fileContent.Find(_T("127.0.0.1 mm-gta5-prod.ros.rockstargames.com")) == -1) {
		file.SeekToEnd();
		file.Write("\r\n#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 48);
		WinExec("ipconfig /flushdns", SW_HIDE);
	}
	else {
		int position = fileContent.Find(_T("127.0.0.1 mm-gta5-prod.ros.rockstargames.com"));
		if (fileContent.Find(_T("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com")) == -1) {
			file.Seek(position, CFile::begin);
			file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
			WinExec("ipconfig /flushdns", SW_HIDE);
		}
	}
	file.Close();
	m_Button.SetFaceColor(RGB(129, 0, 24), true);
	m_Button.SetTextColor(RGB(49, 49, 49));




	get_vk_code = AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyVk"), NULL);
	get_modifiers = AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyModifiers"), NULL);

	GetLocalTime(&systime);
	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

	if (get_vk_code >= 0x21 && get_vk_code <= 0x2D || get_modifiers == 1 || get_modifiers == 3 || get_modifiers == 4 || get_modifiers == 6) {
		if (first_line) {
			pLog->m_LogContent += time + _T("This key may not work!!!");
			first_line = false;
		}
		else{
			pLog->m_LogContent += _T("\r\n") + time + _T("This key may not work!!!");
		}
	}

	GetLocalTime(&systime);
	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

	ATOM HotkeyID = GlobalAddAtom(_T("INITIAL_BIND")) - 0xc000;//Register hotkey
	if (FALSE == RegisterHotKey(GetSafeHwnd(), 1, get_modifiers | MOD_NOREPEAT, get_vk_code)) {
		if (first_line) {
			pLog->m_LogContent += time + _T("Register hotkey failed!!!");
			first_line = false;
		}
		else {
			pLog->m_LogContent += _T("\r\n") + time + _T("Register hotkey failed!!!");
		}
		m_Process = _T("Register hotkey failed!!!");
	}
	else {
		get_hotkey = AfxGetApp()->GetProfileString(_T("Settings"), _T("Key"), _T("None"));//Get hotkey string
		if (get_vk_code != 0x00) {
			if (first_line) {
				pLog->m_LogContent += time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
				first_line = false;
			}
			else {
				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
			}
			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
		}
		else {
			if (first_line) {
				pLog->m_LogContent += time + _T("Hotkey disabled");
				first_line = false;
			}
			else {
				pLog->m_LogContent += _T("\r\n") + time + _T("Hotkey disabled");
			}
		}
	}
	UpdateData(false);




	pLog->Create(IDD_LOG_DIALOG, GetDesktopWindow());
	pLog->ShowWindow(SW_HIDE);
	//WINDOWPLACEMENT wndpl, wndpl_parent;
	//GetWindowPlacement(&wndpl_parent);
	//pLog->GetWindowPlacement(&wndpl);
	//CRect dlgrect_parent;
	//GetWindowRect(dlgrect_parent);//Parent Dialog Original Size 396*300
	//CRect dlgrect;
	//pLog->GetWindowRect(dlgrect);//Dialog Original Size 414*262
	//wndpl.rcNormalPosition.left = wndpl_parent.rcNormalPosition.left - fabs((dlgrect_parent.Width() - dlgrect.Width()) / 2);
	//wndpl.rcNormalPosition.top = wndpl_parent.rcNormalPosition.top + fabs((dlgrect_parent.Height() - dlgrect.Height()) / 2);
	//wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgrect.Width();
	//wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgrect.Height();
	//wndpl.showCmd = SW_HIDE;

	//pLog->SetWindowPlacement(&wndpl);


	turnoffSoundState = AfxGetApp()->GetProfileInt(_T("Settings"), _T("TurnoffSoundState"), MF_CHECKED);
	turnonSoundState = AfxGetApp()->GetProfileInt(_T("Settings"), _T("TurnonSoundState"), MF_CHECKED);
	CMenu* menu = GetMenu()->GetSubMenu(1);
	menu->CheckMenuItem(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND | turnoffSoundState);
	menu->CheckMenuItem(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND | turnonSoundState);
	
	m_brush.CreateSolidBrush(RGB(49, 49, 49));

	CRect rcParent;//Get control size differences
	GetClientRect(&rcParent); //560 400

	WINDOWPLACEMENT wndpl;

	m_Button.GetWindowPlacement(&wndpl);
	buttonWidthDiff = rcParent.Width() - wndpl.rcNormalPosition.right;
	buttonHeightDiff = rcParent.Height() - wndpl.rcNormalPosition.bottom;

	m_ProcessCtrl.GetWindowPlacement(&wndpl);
	messageWidthDiff = rcParent.Width() - wndpl.rcNormalPosition.right;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMatchmakingSwitchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

		dc.FillSolidRect(rect, RGB(191, 191, 191));
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMatchmakingSwitchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CMatchmakingSwitchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_EDIT1) {
		pDC->SetTextColor(RGB(206, 206, 206));
		pDC->SetBkColor(RGB(49, 49, 49));
		hbr = m_brush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC) {
		pDC->SetTextColor(RGB(75, 75, 75));
		pDC->SetBkColor(RGB(49, 49, 49));
		hbr = m_brush;
	}

	if (nCtlColor == CTLCOLOR_DLG) {
		return m_brush;
	}
	return hbr;
}


static bool matchmaking = true;


void CMatchmakingSwitchDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	UpdateData(true);
	pLog->UpdateData(true);
	SYSTEMTIME systime;
	CString time;

	if (nHotKeyId) {//If match the HotKeyId then continue
		TCHAR path_t[128];
		_tcscpy_s(path_t, path);
		if (matchmaking) {
			CFile file;
			if (file.Open(path_t, CFile::modeReadWrite) == 0) {
				MessageBox(_T("No Permission or File not Found, Please Run it as Administrator and Try Again"), NULL, MB_OK);
				pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
				return;
			}
			int fileLength = file.GetLength();
			char* get_FileContent = new char[fileLength];
			file.Read(get_FileContent, fileLength);

			CString fileContent;
			fileContent.Format(_T("%S"), get_FileContent);
			free(get_FileContent);
			get_FileContent = NULL;
			int position = fileContent.Find(_T("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com"));
			file.Seek(position, CFile::begin);

			file.Write("127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 45);
			file.Close();
			WinExec("ipconfig /flushdns", SW_HIDE);


			GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("MATCHMAKING: OFF"));
			m_Button.SetFaceColor(RGB(0, 129, 105), true);


			GetLocalTime(&systime);
			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

			pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking service no longer available");
			pLog->UpdateData(false);
			pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
			
			if (turnoffSoundState == MF_CHECKED)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);

			if (get_vk_code != 0) {
				GetLocalTime(&systime);
				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
			}

			matchmaking = !matchmaking;
		}
		else {
			CFile file;
			if (file.Open(path_t, CFile::modeReadWrite) == 0) {
				MessageBox(_T("No Permission or File not Found, Please Run it as Administrator and Try Again"), NULL, MB_OK);
				pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
				return;
			}
			int fileLength = file.GetLength();
			char* get_FileContent = new char[fileLength];
			file.Read(get_FileContent, fileLength);

			CString fileContent;
			fileContent.Format(_T("%S"), get_FileContent);
			free(get_FileContent);
			get_FileContent = NULL;
			int position = fileContent.Find(_T("127.0.0.1 mm-gta5-prod.ros.rockstargames.com"));
			file.Seek(position, CFile::begin);

			file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
			file.Close();
			WinExec("ipconfig /flushdns", SW_HIDE);


			GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("MATCHMAKING: ON"));
			m_Button.SetFaceColor(RGB(129, 0, 24), true);


			GetLocalTime(&systime);
			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

			pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking turned on");
			pLog->UpdateData(false);
			pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);

			if (turnonSoundState == MF_CHECKED)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);

			if (get_vk_code != 0) {
				GetLocalTime(&systime);
				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
				UpdateData(false);
			}

			matchmaking = !matchmaking;
		}
		UpdateData(false);
		pLog->UpdateData(false);
		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMatchmakingSwitchDlg::OnBnClickedButton1()
{
	UpdateData(true);
	pLog->UpdateData(true);
	SYSTEMTIME systime;
	CString time;

	TCHAR path_t[128];
	_tcscpy_s(path_t, path);

	if (matchmaking) {
		CFile file;
		if (file.Open(path_t, CFile::modeReadWrite) == 0) {
			MessageBox(_T("No Permission or File not Found, Please Run it as Administrator and Try Again"), NULL, MB_OK);
			pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
			return;
		}
		int fileLength = file.GetLength();
		char* get_FileContent = new char[fileLength];
		file.Read(get_FileContent, fileLength);

		CString fileContent;
		fileContent.Format(_T("%S"), get_FileContent);
		free(get_FileContent);
		get_FileContent = NULL;
		int position = fileContent.Find(_T("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com"));
		file.Seek(position, CFile::begin);

		file.Write("127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 45);
		file.Close();
		WinExec("ipconfig /flushdns", SW_HIDE);


		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("MATCHMAKING: OFF"));
		m_Button.SetFaceColor(RGB(0, 129, 105), true);


		GetLocalTime(&systime);
		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

		pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking service no longer available");
		pLog->UpdateData(false);
		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);

		if (get_vk_code != 0) {
			GetLocalTime(&systime);
			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
		}

		matchmaking = !matchmaking;
	}
	else {
		CFile file;
		if (file.Open(path_t, CFile::modeReadWrite) == 0) {
			MessageBox(_T("No Permission or File not Found, Please Run it as Administrator and Try Again"), NULL, MB_OK);
			pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
			return;
		}
		int fileLength = file.GetLength();
		char* get_FileContent = new char[fileLength];
		file.Read(get_FileContent, fileLength);

		CString fileContent;
		fileContent.Format(_T("%S"), get_FileContent);
		free(get_FileContent);
		get_FileContent = NULL;
		int position = fileContent.Find(_T("127.0.0.1 mm-gta5-prod.ros.rockstargames.com"));
		file.Seek(position, CFile::begin);

		file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
		file.Close();
		WinExec("ipconfig /flushdns", SW_HIDE);


		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("MATCHMAKING: ON"));
		m_Button.SetFaceColor(RGB(129, 0, 24), true);


		GetLocalTime(&systime);
		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

		pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking turned on");
		pLog->UpdateData(false);
		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);

		if (get_vk_code != 0) {
			GetLocalTime(&systime);
			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
		}

		matchmaking = !matchmaking;
	}
	UpdateData(false);
	pLog->UpdateData(false);
	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
}


void CMatchmakingSwitchDlg::OnOptionsChangehotkey()
{
	UnregisterHotKey(GetSafeHwnd(), 1); //Unbind all hotkey

	INT_PTR result;
	Record RecDlg;
	RecDlg.m_RecordLabel = get_hotkey;
	RecDlg.input_vk_code = get_vk_code;
	RecDlg.input_modifiers = get_modifiers;
	result = RecDlg.DoModal();
	if (IDCANCEL == result) {  //If press cancel then quit
		ATOM HotkeyID = GlobalAddAtom(_T("INITIAL_REBIND")) - 0xc000; //Register hotkey
		RegisterHotKey(GetSafeHwnd(), 1, AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyModifiers"), NULL) | MOD_NOREPEAT, AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyVk"), NULL));//Register hotkey
		return;
	}
	if (RecDlg.input_vk_code == get_vk_code) {
		ATOM HotkeyID = GlobalAddAtom(_T("INITIAL_REBIND")) - 0xc000; //Register hotkey
		RegisterHotKey(GetSafeHwnd(), 1, AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyModifiers"), NULL) | MOD_NOREPEAT, AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyVk"), NULL));//Register hotkey
		return;
	}

	ATOM HotkeyID = GlobalAddAtom(_T("REBIND")) - 0xc000; //Register new hotkey
	BOOL done = RegisterHotKey(GetSafeHwnd(), 1, RecDlg.input_modifiers | MOD_NOREPEAT, RecDlg.input_vk_code);
	get_modifiers = RecDlg.input_modifiers;
	get_vk_code = RecDlg.input_vk_code;
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("KeyModifiers"), get_modifiers);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("KeyVk"), get_vk_code);

	UpdateData(true);
	pLog->UpdateData(true);
	SYSTEMTIME systime;
	CString time;
	GetLocalTime(&systime);
	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

	get_hotkey = RecDlg.input_key; //Add keys which have issue here
	if (get_vk_code == 0x00) {
		get_hotkey = "None";
	}
	if (get_vk_code == 0x13) {
		get_hotkey = "Pause";
	}
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
	
	if (get_vk_code >= 0x21 && get_vk_code <= 0x2D || get_modifiers == 1 || get_modifiers == 3 || get_modifiers == 4 || get_modifiers == 6) {
		pLog->m_LogContent += _T("\r\n") + time + _T("This key may not work!!!");
		m_Process = _T("This key may not work!!!");
		UpdateData(false);
		pLog->UpdateData(false);
		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
	}

	AfxGetApp()->WriteProfileString(_T("Settings"), _T("Key"), get_hotkey);

	
	if (done) {
		if (get_vk_code != 0x00) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
		}
		else {
			pLog->m_LogContent += _T("\r\n") + time + _T("Hotkey disabled");
		}
	}
	else {
		pLog->m_LogContent += _T("\r\n") + time + _T("Register hotkey failed!!!");
		m_Process = _T("Register hotkey failed!!!");
	}
	if (get_vk_code == 0x00) {
		m_Process = _T("");
	}
	UpdateData(false);
	pLog->UpdateData(false);
	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
}


void CMatchmakingSwitchDlg::OnOptionsLog()
{
	bool pWnd = pLog->IsWindowVisible();
	if (!pWnd) {
		//pLog = new Log();
		pLog->Create(IDD_LOG_DIALOG, GetDesktopWindow());
		WINDOWPLACEMENT wndpl, wndpl_parent;
		GetWindowPlacement(&wndpl_parent);
		pLog->GetWindowPlacement(&wndpl);
		CRect dlgrect_parent;
		GetWindowRect(dlgrect_parent);//Parent Dialog Original Size 396*300
		CRect dlgrect;
		pLog->GetWindowRect(dlgrect);//Log Dialog Original Size 414*262
		wndpl.rcNormalPosition.left = wndpl_parent.rcNormalPosition.left - fabs((dlgrect_parent.Width() - dlgrect.Width()) / 2);
		wndpl.rcNormalPosition.top = wndpl_parent.rcNormalPosition.top + fabs((dlgrect_parent.Height() - dlgrect.Height()) / 2);
		wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgrect.Width();
		wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgrect.Height();

		pLog->SetWindowPlacement(&wndpl);
		pLog->ShowWindow(SW_SHOW);
		pLog->SetFocus();
	}
	else {
		pLog->BringWindowToTop();
		pLog->ShowWindow(SW_NORMAL);
	}
}


void CMatchmakingSwitchDlg::OnHelpManual()
{
	CWnd* pWnd = FindWindow(_T("MatchmakingSwitchManual"), _T("Manual"));
	if (!pWnd) {
		pManual = new Manual();
		pManual->Create(IDD_MANUAL_DIALOG, GetDesktopWindow());
		WINDOWPLACEMENT wndpl, wndpl_parent;
		GetWindowPlacement(&wndpl_parent);
		pManual->GetWindowPlacement(&wndpl);
		CRect dlgrect_parent;
		GetWindowRect(dlgrect_parent);//Parent Dialog Original Size 396*300
		CRect dlgrect;
		pManual->GetWindowRect(dlgrect);//Manual Dialog Original Size 414*262

		wndpl.rcNormalPosition.left = wndpl_parent.rcNormalPosition.left - fabs((dlgrect_parent.Width() - dlgrect.Width()) / 2);
		wndpl.rcNormalPosition.top = wndpl_parent.rcNormalPosition.top + fabs((dlgrect_parent.Height() - dlgrect.Height()) / 2);
		wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgrect.Width();
		wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgrect.Height();

		pManual->SetWindowPlacement(&wndpl);
		pManual->ShowWindow(SW_SHOW);
		pManual->SetFocus();
	}
	else {
		pWnd->BringWindowToTop();
		pWnd->ShowWindow(SW_NORMAL);
	}
}


void CMatchmakingSwitchDlg::OnHelpEdithostsfile()
{
	CWnd* pWnd = FindWindow(_T("Notepad"), _T("hosts - Notepad"));
	if (!pWnd) {
		WinExec("cmd.exe /c start notepad.exe %WINDIR%\\System32\\drivers\\etc\\hosts", SW_HIDE);
	}
	else {
		pWnd->BringWindowToTop();
		pWnd->ShowWindow(SW_NORMAL);
	}
}


BOOL CMatchmakingSwitchDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	return TRUE;
}


void CMatchmakingSwitchDlg::OnClose()
{
	WINDOWPLACEMENT wndpl;//Save last window position
	GetWindowPlacement(&wndpl);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("WindowPosX"), wndpl.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("WindowPosY"), wndpl.rcNormalPosition.top);
	wndpl.rcNormalPosition.right = 396 + wndpl.rcNormalPosition.left;

	CDialogEx::OnClose();
}


void CMatchmakingSwitchDlg::PreInitDialog()
{
	WINDOWPLACEMENT wndpl;//Restore last window position
	GetWindowPlacement(&wndpl);
	RECT workarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
	CRect dlgrect;
	GetWindowRect(dlgrect);//Dialog Original Size 396*300

	wndpl.rcNormalPosition.left = AfxGetApp()->GetProfileInt(_T("Settings"), _T("WindowPosX"), (workarea.right - dlgrect.Width()) / 2);
	wndpl.rcNormalPosition.top = AfxGetApp()->GetProfileInt(_T("Settings"), _T("WindowPosY"), (workarea.bottom - dlgrect.Height()) / 2);
	if (wndpl.rcNormalPosition.left < 0)
		wndpl.rcNormalPosition.left = 0;
	if (wndpl.rcNormalPosition.top < 0)
		wndpl.rcNormalPosition.top = 0;
	wndpl.rcNormalPosition.left = min(workarea.right - dlgrect.Width(), wndpl.rcNormalPosition.left + workarea.left);
	wndpl.rcNormalPosition.top = min(workarea.bottom - dlgrect.Height(), wndpl.rcNormalPosition.top + workarea.top);

	wndpl.rcNormalPosition.right = dlgrect.Width() + wndpl.rcNormalPosition.left;
	wndpl.rcNormalPosition.bottom = dlgrect.Height() +wndpl.rcNormalPosition.top;
	SetWindowPlacement(&wndpl);

	//Get Dialog Size
	/*CString str;
	str.Format(_T("%d"), wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left);
	MessageBox(str, NULL, MB_OK);
	str.Format(_T("%d"),wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top);
	MessageBox(str, NULL, MB_OK);*/

	CDialogEx::PreInitDialog();
}


void CMatchmakingSwitchDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	TCHAR path_t[128];
	_tcscpy_s(path_t, path);

	CFile file;
	if (file.Open(path_t, CFile::modeReadWrite) == 0) {
		MessageBox(_T("No Permission or File not Found, Please Run it as Administrator and Try Again"), NULL, MB_OK);
		return;
	}
	int fileLength = file.GetLength();
	char* get_FileContent = new char[fileLength];
	file.Read(get_FileContent, fileLength);

	CString fileContent;
	fileContent.Format(_T("%S"), get_FileContent);
	free(get_FileContent);
	get_FileContent = NULL;
	int position = fileContent.Find(_T("127.0.0.1 mm-gta5-prod.ros.rockstargames.com"));
	if (fileContent.Find(_T("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com")) == -1) {
		file.Seek(position, CFile::begin);
		file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
		WinExec("ipconfig /flushdns", SW_HIDE);
	}
	file.Close();


	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("TurnoffSoundState"), turnoffSoundState);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("TurnonSoundState"), turnonSoundState);
	UnregisterHotKey(GetSafeHwnd(), 1);
}


void CMatchmakingSwitchDlg::OnOptionsTurnoffsound()
{
	CMenu* menu = GetMenu()->GetSubMenu(1);

	if (menu->GetMenuState(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND) == MF_CHECKED) {
		turnoffSoundState = MF_UNCHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else if (menu->GetMenuState(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND) == MF_UNCHECKED) {
		turnoffSoundState = MF_CHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND | MF_CHECKED);
	}
}


void CMatchmakingSwitchDlg::OnOptionsTurnonsound()
{
	CMenu* menu = GetMenu()->GetSubMenu(1);
	
	if (menu->GetMenuState(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND) == MF_CHECKED) {
		turnonSoundState = MF_UNCHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else if (menu->GetMenuState(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND) == MF_UNCHECKED) {
		turnonSoundState = MF_CHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND | MF_CHECKED);
	}
}


void CMatchmakingSwitchDlg::OnEnSetfocusEdit1()
{
	::HideCaret(GetDlgItem(IDC_EDIT1)->GetSafeHwnd());
}


void CMatchmakingSwitchDlg::OnBnSetfocusButton1()
{
	GetDlgItem(IDC_EDIT1)->SetFocus();
}


BOOL CMatchmakingSwitchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//Set cursor on button to hand shape
	if (pWnd == GetDlgItem(IDC_BUTTON1)) {
		SetCursor(LoadCursor(NULL, IDC_HAND));
		return true;
	}
	if (pWnd == GetDlgItem(IDC_EDIT1)) {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return true;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CMatchmakingSwitchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_Button.m_hWnd != NULL) {
		WINDOWPLACEMENT wndpl;
		m_Button.GetWindowPlacement(&wndpl);

		wndpl.rcNormalPosition.right = cx - buttonWidthDiff;
		wndpl.rcNormalPosition.bottom = cy - buttonHeightDiff;

		m_Button.SetWindowPlacement(&wndpl);
	}
	if (m_ProcessCtrl.m_hWnd != NULL) {
		WINDOWPLACEMENT wndpl;
		m_ProcessCtrl.GetWindowPlacement(&wndpl);

		wndpl.rcNormalPosition.right = cx - messageWidthDiff;
		m_ProcessCtrl.SetWindowPlacement(&wndpl);
	}
}



void CMatchmakingSwitchDlg::OnOK()
{
	//CDialogEx::OnOK();
}


CMatchmakingSwitchDlg::~CMatchmakingSwitchDlg() {
	if (pManual != NULL) {
		delete pManual;
		pManual = NULL;
	}
	if (pLog != NULL) {
		delete pLog;
		pLog = NULL;
	}
}
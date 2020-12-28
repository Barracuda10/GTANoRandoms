
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

//These headers use to operate system firewall
#include<netfw.h>

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
// Complete Word: Ctrl + Space



// CMatchmakingSwitchDlg dialog



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
	, deactivatingSoundState(0)
	, activatingSoundState(0)
	, buttonWidthDiff(0)
	, buttonHeightDiff(0)
	, messageWidthDiff(0)
	, hostsIndex(0)
	, mode(0)
	, firewallSoloSessionState(0)
	, fwOn(0)
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
	ON_COMMAND(ID_OPTIONS_TURNOFFSOUND, &CMatchmakingSwitchDlg::OnOptionsDeactivatingsound)
	ON_COMMAND(ID_OPTIONS_TURNONSOUND, &CMatchmakingSwitchDlg::OnOptionsActivatingsound)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	/*ON_COMMAND(ID_OPTIONS_MATCHMAKINGSWITCH, &CMatchmakingSwitchDlg::OnOptionsMatchmakingSwitch)
	ON_COMMAND(ID_OPTIONS_CLOUDSERVICESSWITCH, &CMatchmakingSwitchDlg::OnOptionsCloudservicesSwitch)*/
	ON_COMMAND(ID_OPTIONS_FIREWALLSOLOSESSION, &CMatchmakingSwitchDlg::OnOptionsFirewallsolosession)
	ON_COMMAND(ID_HELP_OPENFIREWALLSETTINGS, &CMatchmakingSwitchDlg::OnHelpOpenfirewallsettings)
	ON_COMMAND(ID_HELP_VIEWFIREWALLRULES, &CMatchmakingSwitchDlg::OnHelpViewfirewallrules)
	/*ON_COMMAND(ID_OPTIONS_FIREWALLRULESMODE, &CMatchmakingSwitchDlg::OnOptionsFirewallrulesmode)
	ON_COMMAND(ID_OPTIONS_HOSTSREDIRECTMODE, &CMatchmakingSwitchDlg::OnOptionsHostsredirectmode)*/
END_MESSAGE_MAP()


// CMatchmakingSwitchDlg message handlers


struct RulesSet {
	CString RuleName;
	BOOL enable = FALSE;

	LPCSTR cmdline_enable[2];
	LPCSTR cmdline_disable[2];
}RulesSet[4];
int rIndex = 0;


struct Hosts {
	CStringA host;
	BOOL switchState[2] = { false, false };
	CString state[2] = { _T("DEACTIVATED"), _T("ACTIVATE") };
	//CString state[2] = { _T("ENABLE"), _T("BLOCKED") };
	COLORREF buttonColor[2];
	/*CString advancedModeText;*/
}Hosts[8];

//CString buttonText[2] = { _T("MATCHMAKING: "),  _T("CLOUDSERVICES: ") };
//CString buttonText[2] = { _T("RANDOMSWITCH: "),  _T("GAMESAVESWITCH: ") };
//CString buttonText[2] = { _T("MATCHINGSWITCH: "),  _T("SAVINGFAILEDSWITCH: ") };
//CString buttonText[2] = { _T("MATCHINGDISABLE: "),  _T("SAVINGDISABLE: ") };
CString buttonText[2] = { _T("NO RANDOMS "),  _T("CLOUD SAVE ") };
COLORREF BkColor[2] = { RGB(45, 45, 45) , RGB(129, 144, 157) };
COLORREF buttonTextColor[2] = { RGB(49, 49, 49) , RGB(194, 194, 194) };
COLORREF processTextColor[2] = { RGB(206, 206, 206), RGB(206, 206, 206) };


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


	//Get UI Language
	//0x0401 ar-SA
	//0x0404 zh-TW
	//0x0409 en-US
	//0x040A es-ES_tradnl
	//0x040C fr-FR
	//0x040F is-IS
	//0x0410 it-IT
	//0x0411 ja-JP
	//0x0419 ru-RU
	//0x0421 id-ID
	//0x0804 zh-CN
	//0x0809 en-GB
	//0x080A es-MX
	//0x0C04 zh-HK
	//0x0C09 en-AU
	//0x0C0A es-ES
	//0x1004 zh-SG
	//LANGID langID = GetSystemDefaultLangID(); //zh-CN
	//LANGID langID = GetSystemDefaultLCID(); //zh-CN
	//LANGID langID = GetSystemDefaultUILanguage(); //en-US
	/*if (GetSystemDefaultUILanguage() == 0x0804) {
		//MessageBox(_T("zh-CN"), NULL, MB_OK);
	}
	else if (langID == 0x0409) {
		MessageBox(_T("en-US"), NULL, MB_OK);
	}
	else {
		MessageBox(_T("DEFAULT"), NULL, MB_OK);
	}*/


	m_ToolTipCtrl.Create(this);
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("Switch MatchingSwitch between on and off, when it's off randoms can join, when it's on randoms will not join."));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("Press to activate and stop random players from joining, press again to deactivate and allow randoms to join."));
	//m_ToolTipCtrl.SetDelayTime(500);
	m_ToolTipCtrl.SetMaxTipWidth(220);
	m_ToolTipCtrl.Activate(true);


	deactivatingSoundState = AfxGetApp()->GetProfileInt(_T("Settings"), _T("DeactivatingSoundState"), MF_CHECKED);
	activatingSoundState = AfxGetApp()->GetProfileInt(_T("Settings"), _T("ActivatingSoundState"), MF_CHECKED);
	CMenu* menu = GetMenu()->GetSubMenu(1);
	menu->CheckMenuItem(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND | deactivatingSoundState);
	menu->CheckMenuItem(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND | activatingSoundState);


	hostsIndex = 0;

	if (hostsIndex == 0) {
		RulesSet[0].cmdline_enable[0] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (MM-TCP-In)\" new enable=yes";
		RulesSet[0].cmdline_enable[1] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (MM-TCP-Out)\" new enable=yes";
		RulesSet[1].cmdline_enable[0] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (UDP-In)\" new enable=yes";
		RulesSet[1].cmdline_enable[1] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (UDP-Out)\" new enable=yes";
		RulesSet[0].cmdline_disable[0] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (MM-TCP-In)\" new enable=no";
		RulesSet[0].cmdline_disable[1] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (MM-TCP-Out)\" new enable=no";
		RulesSet[1].cmdline_disable[0] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (UDP-In)\" new enable=no";
		RulesSet[1].cmdline_disable[1] = "netsh advfirewall firewall set rule name=\"GTANoRandoms (UDP-Out)\" new enable=no";
	}
	else {
		RulesSet[0].cmdline_enable[0] = "netsh advfirewall firewall set rule name=\"GTABlockCloudSave (CS-TCP-In)\" new enable=yes";
		RulesSet[0].cmdline_enable[1] = "netsh advfirewall firewall set rule name=\"GTABlockCloudSave (CS-TCP-Out)\" new enable=yes";
		RulesSet[1].cmdline_enable[0] = "ipconfig /release";
		RulesSet[0].cmdline_disable[0] = "netsh advfirewall firewall set rule name=\"GTABlockCloudSave (CS-TCP-In)\" new enable=no";
		RulesSet[0].cmdline_disable[1] = "netsh advfirewall firewall set rule name=\"GTABlockCloudSave (CS-TCP-Out)\" new enable=no";
		RulesSet[1].cmdline_disable[0] = "ipconfig /renew";
	}


	BOOL ruledAdded = AfxGetApp()->GetProfileInt(_T("Settings"), _T("RuleAdded"), 0);
	if (hostsIndex == 0) {
		if (ruledAdded == 0) {
			WinExec("netsh advfirewall firewall add rule name=\"GTANoRandoms (UDP-In)\" dir=in action=block protocol=udp localport=6672 enable=no", SW_HIDE);
			WinExec("netsh advfirewall firewall add rule name=\"GTANoRandoms (UDP-Out)\" dir=out action=block protocol=udp localport=6672 enable=no", SW_HIDE);
			WinExec("netsh advfirewall firewall add rule name=\"GTANoRandoms (MM-TCP-In)\" dir=in action=block protocol=tcp remoteip=192.81.241.172 enable=no", SW_HIDE);
			WinExec("netsh advfirewall firewall add rule name=\"GTANoRandoms (MM-TCP-Out)\" dir=out action=block protocol=tcp remoteip=192.81.241.172 enable=no", SW_HIDE);
			ruledAdded = true;
		}
	}
	else {
		if (ruledAdded == 0) {
			WinExec("netsh advfirewall firewall add rule name=\"GTABlockCloudSave (CS-TCP-In)\" dir=in action=block protocol=tcp remoteip=192.81.241.170,192.81.241.171 enable=no", SW_HIDE);
			WinExec("netsh advfirewall firewall add rule name=\"GTABlockCloudSave (CS-TCP-Out)\" dir=out action=block protocol=tcp remoteip=192.81.241.170,192.81.241.171 enable=no", SW_HIDE);
			ruledAdded = true;
		}
	}
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("RuleAdded"), ruledAdded);


	//mode = AfxGetApp()->GetProfileInt(_T("Settings"), _T("SwitchMode"), 0);
	mode = 0;
	/*if (mode == 0) {
		menu->CheckMenuRadioItem(ID_OPTIONS_FIREWALLRULESMODE, ID_OPTIONS_HOSTSREDIRECTMODE, ID_OPTIONS_FIREWALLRULESMODE, MF_BYCOMMAND);
	}
	else if (mode == 1) {
		menu->CheckMenuRadioItem(ID_OPTIONS_FIREWALLRULESMODE, ID_OPTIONS_HOSTSREDIRECTMODE, ID_OPTIONS_HOSTSREDIRECTMODE, MF_BYCOMMAND);
	}*/


	Hosts[0].host = "127.0.0.1 mm-gta5-prod.ros.rockstargames.com";
	Hosts[1].host = "127.0.0.1 cs-gta5-prod.ros.rockstargames.com";
	/*Hosts[0].advancedModeText = _T("MM: ");
	Hosts[1].advancedModeText = _T("CS: ");*/
	Hosts[0].buttonColor[0] = RGB(67, 86, 84);
	Hosts[0].buttonColor[1] = RGB(0, 129, 115);
	Hosts[1].buttonColor[0] = RGB(219, 219, 219);
	Hosts[1].buttonColor[1] = RGB(83, 95, 103);


	if (hostsIndex == 0) {
		AfxGetMainWnd()->SetWindowTextW(_T("GTANoRandoms")); // _T("MatchmakingSwitch"), _T("MatchingSwitch")
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState[0]]);
	}
	else {
		AfxGetMainWnd()->SetWindowTextW(_T("GTABlockCloudSave")); // _T("CloudServicesSwitch"), _T("AutosaveSwitch"), _T("SavingfailedSwitch")
		//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("Switch SavingfailedSwitch between on and off, when it's off game is able to save, when it's on game can not save."));
		m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("Press to block game cloud save services, press again to unblock."));
		menu->ModifyMenu(ID_OPTIONS_FIREWALLSOLOSESSION, MF_BYCOMMAND, ID_OPTIONS_FIREWALLSOLOSESSION, _T("Disconnect"));

		/*if (GetSystemDefaultUILanguage() == 0x0804) {
			AfxGetMainWnd()->SetWindowTextW(_T("GTA屏蔽云存档"));
			lf.lfHeight = 32;
			StrCpyW(lf.lfFaceName, L"MS UI Gothic");
			m_font.CreateFontIndirect(&lf);
			GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);

			m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("点击以屏蔽云存档服务, 再按一次还原"));
			menu->ModifyMenu(ID_OPTIONS_FIREWALLSOLOSESSION, MF_BYCOMMAND, ID_OPTIONS_FIREWALLSOLOSESSION, _T("断开网络"));
			buttonText[1] = _T("云存档");
			Hosts[1].state[0] = _T("可用");
			Hosts[1].state[1] = _T("已屏蔽");
			GetDlgItem(IDC_EDIT1)->ShowWindow(FALSE);

			Hosts[1].buttonColor[0] = RGB(202, 202, 202);
			Hosts[1].buttonColor[1] = RGB(33, 165, 186);
			BkColor[1] = RGB(99, 104, 120);
			buttonTextColor[1] = RGB(99, 104, 120);
		}*/

		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState[0]]);
	}

	m_Button.SetFaceColor(Hosts[hostsIndex].buttonColor[Hosts[hostsIndex].switchState[0]], true);
	m_Button.SetTextColor(buttonTextColor[hostsIndex]);


	//Turn matchmaking back on
	UpdateData(true);
	SYSTEMTIME systime;
	CString time;
	bool first_line = true;

	//TCHAR systemDir[128];//GetSystemDirectory
	//GetSystemDirectory(systemDir, 128 * sizeof(TCHAR));
	//TCHAR *next_token = NULL;
	//TCHAR* path_t = _tcstok_s(systemDir, _T("\\"), &next_token);
	//_tcscat_s(path_t, 128, _T("\\Windows\\System32\\drivers\\etc\\hosts"));
	//path = path_t;


	//if (mode == 1) {
	//	TCHAR path_temp[128];
	//	_tcscpy_s(path_temp, path);

	//	CFile file;
	//	if (file.Open(path_temp, CFile::modeReadWrite) == 0) {
	//		GetLocalTime(&systime);
	//		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
	//		MessageBox(_T("No Permission or File not Found, Please Run this Program as Administrator and Try Again"), NULL, MB_OK);
	//		pLog->m_LogContent += time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
	//		first_line = false;
	//		return 1;
	//	}
	//	int fileLength = file.GetLength();
	//	char* get_text = new char[fileLength];
	//	memset(get_text, 0, sizeof(get_text));
	//	file.Read(get_text, fileLength);


	//	CStringA fileContent;
	//	fileContent.Format("%s", get_text);
	//	free(get_text);
	//	get_text = NULL;


	//	for (int hostIndex = 0; hostIndex < 2; hostIndex++) {
	//		if (fileContent.Find(Hosts[hostIndex].host) == -1) { //If host not exist
	//			file.SeekToEnd();
	//			CStringA hostOutput = "\r\n#" + Hosts[hostIndex].host + "\r";
	//			file.Write(hostOutput, hostOutput.GetLength());
	//		}
	//		else {
	//			int position = fileContent.Find(Hosts[hostIndex].host);//TODO: Make it not replace text because the length is extended
	//			if (fileContent.Find("#" + Hosts[hostIndex].host) == -1) { //If host is not comment
	//				//position = fileContent.Find(Hosts[hostIndex].host);
	//				//file.Seek(position - 1, CFile::begin);

	//				//CStringA hostOutput = "#";
	//				////CStringA hostOutput = "#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r";// + Hosts[0].host + "\r";
	//				//file.Write(hostOutput, hostOutput.GetLength());
	//				//file.Flush();
	//				//WinExec("ipconfig /flushdns", SW_HIDE);


	//				position = fileContent.Find(Hosts[hostIndex].host);
	//				file.Seek(position, CFile::begin);
	//				CStringA hostOutput = "#" + Hosts[hostIndex].host + "\r";
	//				file.Write(hostOutput, hostOutput.GetLength());
	//				//file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
	//			}
	//			if (fileContent.Find("##" + Hosts[hostIndex].host) != -1) { //If host is corrupted
	//				int linestart = fileContent.Find("##" + Hosts[hostIndex].host);
	//				CStringA fileContentCopy = fileContent;
	//				fileContentCopy.Delete(linestart + 1, fileContentCopy.GetLength());
	//				while (true) {
	//					file.Seek(linestart, CFile::begin);
	//					file.Write(" ", 1);
	//					linestart--;
	//					if (linestart < 0) {
	//						break;
	//					}
	//					if (fileContentCopy.Find("\n", linestart) != -1) {
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}

	//	/*if (fileContent.Find("127.0.0.1 mm-gta5-prod.ros.rockstargames.com") == -1) {
	//		file.SeekToEnd();
	//		file.Write("\r\n#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 48);
	//		WinExec("ipconfig /flushdns", SW_HIDE);
	//	}
	//	else {
	//		int position = fileContent.Find("127.0.0.1 mm-gta5-prod.ros.rockstargames.com");
	//		if (fileContent.Find("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com") == -1) {
	//			position = fileContent.Find("127.0.0.1 mm-gta5-prod.ros.rockstargames.com");
	//			file.Seek(position, CFile::begin);

	//			file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
	//			WinExec("ipconfig /flushdns", SW_HIDE);
	//		}
	//		if (fileContent.Find("##127.0.0.1 mm-gta5-prod.ros.rockstargames.com") != -1) {
	//			int linestart = fileContent.Find("##127.0.0.1 mm-gta5-prod.ros.rockstargames.com");
	//			fileContent.Delete(linestart + 1, fileContent.GetLength());
	//			while (true) {
	//				file.Seek(linestart, CFile::begin);
	//				file.Write(" ", 1);
	//				linestart--;
	//				if (linestart < 0) {
	//					break;
	//				}
	//				if (fileContent.Find("\n", linestart) != -1) {
	//					break;
	//				}
	//			}
	//			WinExec("ipconfig /flushdns", SW_HIDE);
	//		}
	//	}*/


	//	file.Close();
	//	WinExec("ipconfig /flushdns", SW_HIDE);
	//}
	



	get_vk_code = AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyVk"), NULL);
	get_modifiers = AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyModifiers"), NULL);
	get_hotkey = AfxGetApp()->GetProfileString(_T("Settings"), _T("Key"), _T("None"));

	if (get_vk_code != 0) {
		GetLocalTime(&systime);
		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

		if (get_vk_code >= 0x21 && get_vk_code <= 0x2D || get_modifiers == 1 || get_modifiers == 3 || get_modifiers == 4 || get_modifiers == 6) {
			if (first_line) {
				pLog->m_LogContent += time + _T("This key may not work!!!");
				first_line = false;
			}
			else {
				pLog->m_LogContent += _T("\r\n") + time + _T("This key may not work!!!");
			}
			m_Process = _T("This key may not work!!!");
		}

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
					pLog->m_LogContent += time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
					first_line = false;
				}
				else {
					pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
				}
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Activate");
				/*if (firewallSoloSessionState) {
					m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
				}*/
			}
			else {
				if (first_line) {
					pLog->m_LogContent += time + _T("Hotkey disabled");
					first_line = false;
				}
				else {
					pLog->m_LogContent += _T("\r\n") + time + _T("Hotkey disabled");
				}
				/*if (firewallSoloSessionState) {
					m_Process = _T("Firewall Solo Session Enabled");
				}*/
			}
		}
	}




	if (mode == 0) {
		WinExec(RulesSet[0].cmdline_disable[0], SW_HIDE);
		WinExec(RulesSet[0].cmdline_disable[1], SW_HIDE);
		WinExec(RulesSet[1].cmdline_disable[0], SW_HIDE);
		WinExec(RulesSet[1].cmdline_disable[1], SW_HIDE);


		//Check system firewall
		if (!CheckSystemFirewall()) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
		}
		/*HRESULT hr = S_OK;
		INetFwMgr* fwMgr = NULL;
		INetFwPolicy* fwPolicy = NULL;
		INetFwProfile* fwProfile;
		hr = CoCreateInstance(
			__uuidof(NetFwMgr),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwMgr),
			(void**)&fwMgr
		);
		hr = fwMgr->get_LocalPolicy(&fwPolicy);
		if (FAILED(hr)) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
		}
		hr = fwPolicy->get_CurrentProfile(&fwProfile);
		if (FAILED(hr)) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
		}
		VARIANT_BOOL fwEnabled = NULL;
		hr = fwProfile->get_FirewallEnabled(&fwEnabled);
		if (FAILED(hr)) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
		}
		if (fwEnabled != VARIANT_FALSE) {
			fwOn = TRUE;
		}
		else {
			fwOn = FALSE;
		}*/

		if (!fwOn) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Firewall is off!!! Turn it on to make the tool work");
			pLog->m_LogContent += _T("\r\n") + time + _T("This tool can not work without firewall on");
			m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
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

	m_brush.CreateSolidBrush(BkColor[hostsIndex]);


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
		pDC->SetTextColor(processTextColor[hostsIndex]);
		pDC->SetBkColor(BkColor[hostsIndex]);
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


//void CMatchmakingSwitchDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
//{
//	UpdateData(true);
//	pLog->UpdateData(true);
//	SYSTEMTIME systime;
//	CString time;
//
//	if (nHotKeyId) {//If match the HotKeyId then continue
//		TCHAR path_t[128];
//		_tcscpy_s(path_t, path);
//		if (!Hosts[hostsIndex].switchState) {
//			CFile file;
//			if (file.Open(path_t, CFile::modeReadWrite) == 0) {
//				GetLocalTime(&systime);
//				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//				pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
//				return;
//			}
//			int fileLength = file.GetLength();
//			char* get_text = new char[fileLength];
//			memset(get_text, 0, sizeof(get_text));
//			file.Read(get_text, fileLength);
//
//			CStringA fileContent;
//			fileContent.Format("%s", get_text);
//			//int position = fileContent.Find("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com");
//			int position = fileContent.Find("#" + Hosts[hostsIndex].host);
//			file.Seek(position, CFile::begin);
//
//			CStringA hostOutput = Hosts[hostsIndex].host + "\r";
//			file.Write(hostOutput, hostOutput.GetLength());
//			//file.Write("127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 45);
//
//
//			//Check if it was done correctly
//			file.SeekToBegin();
//			fileLength = file.GetLength();
//			get_text = new char[fileLength];
//			memset(get_text, 0, sizeof(get_text));
//			file.Read(get_text, fileLength);
//
//			fileContent.Format("%s", get_text);
//			free(get_text);
//			get_text = NULL;
//			position = fileContent.Find("#" + Hosts[hostsIndex].host);
//			if (position != -1) {
//				MessageBox(_T("There was a error completing this process"), NULL, MB_OK);
//				GetLocalTime(&systime);
//				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//				pLog->m_LogContent += _T("\r\n") + time + _T("There was a error completing this process");
//				pLog->UpdateData(false);
//				file.Close();
//				return;
//			}
//
//
//			file.Close();
//			WinExec("ipconfig /flushdns", SW_HIDE);
//
//
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//			if (hostsIndex == 0)
//				pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking services no longer available");
//			else
//				pLog->m_LogContent += _T("\r\n") + time + _T("Cloud services no longer available");
//			
//			pLog->UpdateData(false);
//			pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//			
//			if (turnoffSoundState == MF_CHECKED)
//				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
//
//			if (get_vk_code != 0) {
//				GetLocalTime(&systime);
//				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
//				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
//
//				if (firewallSoloSessionState) {
//					m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//				}
//			}
//			else {
//				if (firewallSoloSessionState) {
//					m_Process = _T("Firewall Solo Session Enabled");
//				}
//			}
//
//			Hosts[hostsIndex].switchState = !Hosts[hostsIndex].switchState;
//			GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState]);
//			m_Button.SetFaceColor(RGB(0, 129, 105), true);
//		}
//		else {
//			CFile file;
//			if (file.Open(path_t, CFile::modeReadWrite) == 0) {
//				GetLocalTime(&systime);
//				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//				pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
//				return;
//			}
//			int fileLength = file.GetLength();
//			char* get_text = new char[fileLength];
//			memset(get_text, 0, sizeof(get_text));
//			file.Read(get_text, fileLength);
//
//			CStringA fileContent;
//			fileContent.Format("%s", get_text);
//			//int position = fileContent.Find("127.0.0.1 mm-gta5-prod.ros.rockstargames.com");
//			free(get_text);
//			get_text = NULL;
//			int position = fileContent.Find(Hosts[hostsIndex].host);
//			file.Seek(position, CFile::begin);
//
//			CStringA hostOutput = "#" + Hosts[hostsIndex].host + "\r";
//			file.Write(hostOutput, hostOutput.GetLength());
//			//file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
//
//
//			//Check if it was done correctly
//			file.SeekToBegin();
//			fileLength = file.GetLength();
//			get_text = new char[fileLength];
//			memset(get_text, 0, sizeof(get_text));
//			file.Read(get_text, fileLength);
//
//			fileContent.Format("%s", get_text);
//			free(get_text);
//			get_text = NULL;
//			position = fileContent.Find("#" + Hosts[hostsIndex].host);
//			bool error = false;
//			if (position != -1) {
//				position = fileContent.Find("##" + Hosts[hostsIndex].host);
//				if (position != -1) {
//					int linestart = fileContent.Find("#" + Hosts[hostsIndex].host);
//					CStringA fileContentCopy = fileContent;
//					fileContentCopy.Delete(linestart + 1, fileContentCopy.GetLength());
//					while (true) {
//						file.Seek(linestart, CFile::begin);
//						file.Write(" ", 1);
//						linestart--;
//						if (linestart < 0) {
//							break;
//						}
//						if (fileContentCopy.Find("\n", linestart) != -1) {
//							break;
//						}
//					}
//					error = true;
//				}
//			}
//			else {
//				error = true;
//			}
//			if (error) {
//				MessageBox(_T("There was a error completing this process"), NULL, MB_OK);
//				GetLocalTime(&systime);
//				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//				pLog->m_LogContent += _T("\r\n") + time + _T("There was a error completing this process");
//				pLog->UpdateData(false);
//				file.Close();
//				return;
//			}
//
//
//			file.Close();
//			WinExec("ipconfig /flushdns", SW_HIDE);
//
//
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//			if (hostsIndex == 0)
//				pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking services is now available");
//			else
//				pLog->m_LogContent += _T("\r\n") + time + _T("Cloud services is now available");
//
//			pLog->UpdateData(false);
//			pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//
//			if (turnonSoundState == MF_CHECKED)
//				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
//
//			if (get_vk_code != 0) {
//				GetLocalTime(&systime);
//				time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
//				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
//
//				if (firewallSoloSessionState) {
//					m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//				}
//			}
//			else {
//				if (firewallSoloSessionState) {
//					m_Process = _T("Firewall Solo Session Enabled");
//				}
//			}
//
//			Hosts[hostsIndex].switchState = !Hosts[hostsIndex].switchState;
//			GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState]);
//			m_Button.SetFaceColor(RGB(129, 0, 24), true);
//		}
//		UpdateData(false);
//		pLog->UpdateData(false);
//		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//	}
//	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
//}


void CMatchmakingSwitchDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (nHotKeyId) {//If match the HotKeyId then continue
		PlaySound(nullptr, nullptr, SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
		Switching(1);
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


//void CMatchmakingSwitchDlg::OnBnClickedButton1()
//{
//	UpdateData(true);
//	pLog->UpdateData(true);
//	SYSTEMTIME systime;
//	CString time;
//
//	TCHAR path_t[128];
//	_tcscpy_s(path_t, path);
//
//	if (!Hosts[hostsIndex].switchState) {
//		CFile file;
//		if (file.Open(path_t, CFile::modeReadWrite) == 0) {
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//			pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
//			return;
//		}
//		int fileLength = file.GetLength();
//		char* get_text = new char[fileLength];
//		memset(get_text, 0, sizeof(get_text));
//		file.Read(get_text, fileLength);
//
//		CStringA fileContent;
//		fileContent.Format("%s", get_text);
//		int position = fileContent.Find("#" + Hosts[hostsIndex].host);
//		file.Seek(position, CFile::begin);
//
//		CStringA hostOutput = Hosts[hostsIndex].host + "\r";
//		file.Write(hostOutput, hostOutput.GetLength());
//
//
//		//Check if it was done correctly
//		file.SeekToBegin();
//		fileLength = file.GetLength();
//		get_text = new char[fileLength];
//		memset(get_text, 0, sizeof(get_text));
//		file.Read(get_text, fileLength);
//
//		fileContent.Format("%s", get_text);
//		free(get_text);
//		get_text = NULL;
//		position = fileContent.Find("#" + Hosts[hostsIndex].host);
//		if (position != -1) {
//			MessageBox(_T("There was a error completing this process"), NULL, MB_OK);
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//			pLog->m_LogContent += _T("\r\n") + time + _T("There was a error completing this process");
//			pLog->UpdateData(false);
//			file.Close();
//			return;
//		}
//
//
//		file.Close();
//		WinExec("ipconfig /flushdns", SW_HIDE);
//
//
//		GetLocalTime(&systime);
//		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//		if (hostsIndex == 0)
//			pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking services no longer available");
//		else
//			pLog->m_LogContent += _T("\r\n") + time + _T("Cloud services no longer available");
//
//		pLog->UpdateData(false);
//		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//
//		if (get_vk_code != 0) {
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
//
//			if (firewallSoloSessionState) {
//				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//			}
//		}
//		else {
//			if (firewallSoloSessionState) {
//				m_Process = _T("Firewall Solo Session Enabled");
//			}
//		}
//
//		Hosts[hostsIndex].switchState = !Hosts[hostsIndex].switchState;
//		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(Hosts[hostsIndex].buttonText + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState]);
//		m_Button.SetFaceColor(RGB(0, 129, 105), true);
//	}
//	else {
//		CFile file;
//		if (file.Open(path_t, CFile::modeReadWrite) == 0) {
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//			pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
//			return;
//		}
//		int fileLength = file.GetLength();
//		char* get_text = new char[fileLength];
//		memset(get_text, 0, sizeof(get_text));
//		file.Read(get_text, fileLength);
//
//		CStringA fileContent;
//		fileContent.Format("%s", get_text);
//		free(get_text);
//		get_text = NULL;
//		int position = fileContent.Find(Hosts[hostsIndex].host);
//		file.Seek(position, CFile::begin);
//
//		CStringA hostOutput = "#" + Hosts[hostsIndex].host + "\r";
//		file.Write(hostOutput, hostOutput.GetLength());
//
//
//		//Check if it was done correctly
//		file.SeekToBegin();
//		fileLength = file.GetLength();
//		get_text = new char[fileLength];
//		memset(get_text, 0, sizeof(get_text));
//		file.Read(get_text, fileLength);
//
//		fileContent.Format("%s", get_text);
//		free(get_text);
//		get_text = NULL;
//		position = fileContent.Find("#" + Hosts[hostsIndex].host);
//		bool error = false;
//		if (position != -1) {
//			position = fileContent.Find("##" + Hosts[hostsIndex].host);
//			if (position != -1) {
//				int linestart = fileContent.Find("#" + Hosts[hostsIndex].host);
//				CStringA fileContentCopy = fileContent;
//				fileContentCopy.Delete(linestart + 1, fileContentCopy.GetLength());
//				while (true) {
//					file.Seek(linestart, CFile::begin);
//					file.Write(" ", 1);
//					linestart--;
//					if (linestart < 0) {
//						break;
//					}
//					if (fileContentCopy.Find("\n", linestart) != -1) {
//						break;
//					}
//				}
//				error = true;
//			}
//		}
//		else {
//			error = true;
//		}
//		if (error) {
//			MessageBox(_T("There was a error completing this process"), NULL, MB_OK);
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//			pLog->m_LogContent += _T("\r\n") + time + _T("There was a error completing this process");
//			pLog->UpdateData(false);
//			file.Close();
//			return;
//		}
//
//
//		file.Close();
//		WinExec("ipconfig /flushdns", SW_HIDE);
//		
//
//		GetLocalTime(&systime);
//		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//		if (hostsIndex == 0)
//			pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking services is now available");
//		else
//			pLog->m_LogContent += _T("\r\n") + time + _T("Cloud services is now available");
//
//		pLog->UpdateData(false);
//		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//
//		if (get_vk_code != 0) {
//			GetLocalTime(&systime);
//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
//
//			if (firewallSoloSessionState) {
//				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//			}
//		}
//		else {
//			if (firewallSoloSessionState) {
//				m_Process = _T("Firewall Solo Session Enabled");
//			}
//		}
//
//		Hosts[hostsIndex].switchState = !Hosts[hostsIndex].switchState;
//		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(Hosts[hostsIndex].buttonText + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState]);
//		m_Button.SetFaceColor(RGB(129, 0, 24), true);
//	}
//	UpdateData(false);
//	pLog->UpdateData(false);
//	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//}


void CMatchmakingSwitchDlg::OnBnClickedButton1()
{
	Switching(0);
}


void CMatchmakingSwitchDlg::Switching(UINT type)
{
	UpdateData(true);
	pLog->UpdateData(true);
	SYSTEMTIME systime;
	CString time;

	GetLocalTime(&systime);
	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

	/*TCHAR path_t[128];
	_tcscpy_s(path_t, path);*/


	if (mode == 0) {
		if (!Hosts[hostsIndex].switchState[mode]) {
			//Check system firewall
			if (!CheckSystemFirewall()) {
				pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
			}

			WinExec(RulesSet[0].cmdline_enable[0], SW_HIDE);
			WinExec(RulesSet[0].cmdline_enable[1], SW_HIDE);

			Hosts[hostsIndex].switchState[mode] = !Hosts[hostsIndex].switchState[mode];
		}
		else {
			WinExec(RulesSet[0].cmdline_disable[0], SW_HIDE);
			WinExec(RulesSet[0].cmdline_disable[1], SW_HIDE);

			Hosts[hostsIndex].switchState[mode] = !Hosts[hostsIndex].switchState[mode];
		}
	}
	//else if (mode == 1) {
	//	if (!Hosts[hostsIndex].switchState[mode]) {
	//		CFile file;
	//		if (file.Open(path_t, CFile::modeReadWrite) == 0) {
	//			pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
	//			return;
	//		}
	//		int fileLength = file.GetLength();
	//		char* get_text = new char[fileLength];
	//		memset(get_text, 0, sizeof(get_text));
	//		file.Read(get_text, fileLength);

	//		CStringA fileContent;
	//		fileContent.Format("%s", get_text);
	//		int position = fileContent.Find("#" + Hosts[hostsIndex].host);
	//		file.Seek(position, CFile::begin);

	//		CStringA hostOutput = Hosts[hostsIndex].host + "\r";
	//		file.Write(hostOutput, hostOutput.GetLength());


	//		//Check if it was done correctly
	//		file.SeekToBegin();
	//		fileLength = file.GetLength();
	//		get_text = new char[fileLength];
	//		memset(get_text, 0, sizeof(get_text));
	//		file.Read(get_text, fileLength);

	//		fileContent.Format("%s", get_text);
	//		free(get_text);
	//		get_text = NULL;
	//		position = fileContent.Find("#" + Hosts[hostsIndex].host);
	//		if (position != -1) {
	//			MessageBox(_T("There was a error completing this process, Please try again"), NULL, MB_OK);
	//			pLog->m_LogContent += _T("\r\n") + time + _T("There was a error completing this process");
	//			pLog->UpdateData(false);
	//			file.Close();
	//			return;
	//		}


	//		file.Close();
	//		WinExec("ipconfig /flushdns", SW_HIDE);

	//		Hosts[hostsIndex].switchState[mode] = !Hosts[hostsIndex].switchState[mode];
	//	}
	//	else {
	//		CFile file;
	//		if (file.Open(path_t, CFile::modeReadWrite) == 0) {
	//			GetLocalTime(&systime);
	//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
	//			pLog->m_LogContent += _T("\r\n") + time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
	//			return;
	//		}
	//		int fileLength = file.GetLength();
	//		char* get_text = new char[fileLength];
	//		memset(get_text, 0, sizeof(get_text));
	//		file.Read(get_text, fileLength);

	//		CStringA fileContent;
	//		fileContent.Format("%s", get_text);
	//		/*free(get_text);
	//		get_text = NULL;*/
	//		int position = fileContent.Find(Hosts[hostsIndex].host);
	//		file.Seek(position, CFile::begin);

	//		CStringA hostOutput = "#" + Hosts[hostsIndex].host + "\r";
	//		file.Write(hostOutput, hostOutput.GetLength());


	//		//Check if it was done correctly
	//		file.SeekToBegin();
	//		fileLength = file.GetLength();
	//		get_text = new char[fileLength];
	//		memset(get_text, 0, sizeof(get_text));
	//		file.Read(get_text, fileLength);

	//		fileContent.Format("%s", get_text);
	//		free(get_text);
	//		get_text = NULL;
	//		position = fileContent.Find("#" + Hosts[hostsIndex].host);
	//		bool error = false;
	//		if (position != -1) {
	//			position = fileContent.Find("##" + Hosts[hostsIndex].host);
	//			if (position != -1) {
	//				int linestart = fileContent.Find("#" + Hosts[hostsIndex].host);
	//				CStringA fileContentCopy = fileContent;
	//				fileContentCopy.Delete(linestart + 1, fileContentCopy.GetLength());
	//				while (true) {
	//					file.Seek(linestart, CFile::begin);
	//					file.Write(" ", 1);
	//					linestart--;
	//					if (linestart < 0) {
	//						break;
	//					}
	//					if (fileContentCopy.Find("\n", linestart) != -1) {
	//						break;
	//					}
	//				}
	//				error = true;
	//			}
	//		}
	//		else {
	//			error = true;
	//		}
	//		if (error) {
	//			MessageBox(_T("There was a error completing this process, Please try again"), NULL, MB_OK);
	//			GetLocalTime(&systime);
	//			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
	//			pLog->m_LogContent += _T("\r\n") + time + _T("There was a error completing this process");
	//			pLog->UpdateData(false);
	//			file.Close();
	//			return;
	//		}


	//		file.Close();
	//		WinExec("ipconfig /flushdns", SW_HIDE);

	//		Hosts[hostsIndex].switchState[mode] = !Hosts[hostsIndex].switchState[mode];
	//	}
	//}
	//

	if (Hosts[hostsIndex].switchState[mode]) {
		if (type == 1 && activatingSoundState == MF_CHECKED)
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);

		if (hostsIndex == 0)
			pLog->m_LogContent += _T("\r\n") + time + _T("Stop random players join activated, Matchmaking services no longer available");
		else
			pLog->m_LogContent += _T("\r\n") + time + _T("Cloud services blocked");

		if (get_vk_code != 0) {
			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to deactivate");
			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Deactivate");

			if (firewallSoloSessionState) {
				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
			}
		}
		else {
			if (firewallSoloSessionState) {
				m_Process = _T("Firewall Solo Session Enabled");
			}
		}
		if (!fwOn && mode == 0) {
			m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
		}

		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState[mode]]);
		m_Button.SetFaceColor(Hosts[hostsIndex].buttonColor[Hosts[hostsIndex].switchState[mode]], true);
	}
	else {
		if (type == 1 && deactivatingSoundState == MF_CHECKED)
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);

		if (hostsIndex == 0)
			pLog->m_LogContent += _T("\r\n") + time + _T("Matchmaking services is now available");
		else
			pLog->m_LogContent += _T("\r\n") + time + _T("Cloud services enabled");

		if (get_vk_code != 0) {
			GetLocalTime(&systime);
			time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Activate");

			if (firewallSoloSessionState) {
				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
			}
		}
		else {
			if (firewallSoloSessionState) {
				m_Process = _T("Firewall Solo Session Enabled");
			}
		}
		if (!fwOn && mode == 0) {
			m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
		}

		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState[mode]]);
		m_Button.SetFaceColor(Hosts[hostsIndex].buttonColor[Hosts[hostsIndex].switchState[mode]], true);
	}
	UpdateData(false);
	pLog->UpdateData(false);
	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
}


BOOL CMatchmakingSwitchDlg::CheckSystemFirewall()
{
	HRESULT hr = S_OK;
	INetFwMgr* fwMgr = NULL;
	INetFwPolicy* fwPolicy = NULL;
	INetFwProfile* fwProfile;
	hr = CoCreateInstance(__uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwMgr), (void**)&fwMgr);
	BOOL error = false;
	hr = fwMgr->get_LocalPolicy(&fwPolicy);
	if (FAILED(hr))
		error = true;
	hr = fwPolicy->get_CurrentProfile(&fwProfile);
	if (FAILED(hr))
		error = true;
	VARIANT_BOOL fwEnabled = NULL;
	hr = fwProfile->get_FirewallEnabled(&fwEnabled);
	if (FAILED(hr))
		error = true;

	if (fwEnabled != VARIANT_FALSE)
		fwOn = TRUE;
	else
		fwOn = FALSE;

	if (fwPolicy != NULL)
		fwPolicy->Release();
	if (fwMgr != NULL)
		fwMgr->Release();
	if (fwProfile != NULL)
		fwProfile->Release();

	if (error)
		return FALSE;
	else
		return TRUE;
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
	if (IDCANCEL == result || RecDlg.input_vk_code == get_vk_code) {  //If press cancel then quit or hotkey did not changed
		if (get_vk_code != 0) {
			ATOM HotkeyID = GlobalAddAtom(_T("INITIAL_REBIND")) - 0xc000; //Register hotkey
			RegisterHotKey(GetSafeHwnd(), 1, AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyModifiers"), NULL) | MOD_NOREPEAT, AfxGetApp()->GetProfileInt(_T("Settings"), _T("KeyVk"), NULL));//Register hotkey
		}
		else {
			UnregisterHotKey(GetSafeHwnd(), 1); //Unbind all hotkey
		}
		return;
	}
	
	UpdateData(true);
	pLog->UpdateData(true);
	SYSTEMTIME systime;
	CString time;
	GetLocalTime(&systime);
	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);

	get_modifiers = RecDlg.input_modifiers;
	get_vk_code = RecDlg.input_vk_code;
	get_hotkey = RecDlg.input_key;

	if (get_vk_code != 0) {
		ATOM HotkeyID = GlobalAddAtom(_T("REBIND")) - 0xc000; //Register new hotkey
		BOOL done = RegisterHotKey(GetSafeHwnd(), 1, RecDlg.input_modifiers | MOD_NOREPEAT, RecDlg.input_vk_code);
		
		/*AfxGetApp()->WriteProfileInt(_T("Settings"), _T("KeyModifiers"), get_modifiers);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("KeyVk"), get_vk_code);*/

		//Add keys which have issue here
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
		}

		/*AfxGetApp()->WriteProfileString(_T("Settings"), _T("Key"), get_hotkey);*/


		if (done) {
			if (Hosts[hostsIndex].switchState[mode]) {
				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to deactivate");
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Deactivate");
			}
			else {
				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Activate");
			}
			if (firewallSoloSessionState) {
				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
			}
			if (!fwOn && mode == 0) {
				m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
			}
		}
		else {
			pLog->m_LogContent += _T("\r\n") + time + _T("Register hotkey failed!!!");
			m_Process = _T("Register hotkey failed!!!");
		}
	}
	else {
		get_hotkey = "None";
		UnregisterHotKey(GetSafeHwnd(), 1); //Unbind all hotkey
		pLog->m_LogContent += _T("\r\n") + time + _T("Hotkey disabled");
		m_Process = _T("");
		if (firewallSoloSessionState) {
			m_Process = _T("Firewall Solo Session Enabled");
		}
		if (!fwOn && mode == 0) {
			m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
		}
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
	CWnd* pWnd = FindWindow(_T("MatchmakingSwitchManual"), _T("Usage"));
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


void CMatchmakingSwitchDlg::OnHelpOpenfirewallsettings()
{
	WinExec("cmd.exe /c start Firewall.cpl", SW_HIDE);
}


void CMatchmakingSwitchDlg::OnHelpViewfirewallrules()
{
	WinExec("cmd.exe /c start WF.msc", SW_HIDE);
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
	////Check file existence and permission
	//TCHAR systemDir[128];//GetSystemDirectory
	//GetSystemDirectory(systemDir, 128 * sizeof(TCHAR));
	//TCHAR *next_token = NULL;
	//TCHAR* path_t = _tcstok_s(systemDir, _T("\\"), &next_token);
	//_tcscat_s(path_t, 128, _T("\\Windows\\System32\\drivers\\etc\\hosts"));

	//CFile file;
	//if (file.Open(path_t, CFile::modeReadWrite) == 0) {
	//	MessageBox(_T("No Permission or File not Found, Please Run this Program as Administrator and Try Again"), NULL, MB_OK);
	//	exit(0);
	//}


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

	
	if (mode == 0) {
		WinExec(RulesSet[0].cmdline_disable[0], SW_HIDE);
		WinExec(RulesSet[0].cmdline_disable[1], SW_HIDE);
		WinExec(RulesSet[1].cmdline_disable[0], SW_HIDE);
		WinExec(RulesSet[1].cmdline_disable[1], SW_HIDE);
	}
	//else if (mode == 1) {
	//	TCHAR path_t[128];
	//	_tcscpy_s(path_t, path);

	//	CFile file;
	//	if (file.Open(path_t, CFile::modeReadWrite) == 0) {
	//		MessageBox(_T("No Permission or File not Found, Please Run it as Administrator and Try Again"), NULL, MB_OK);
	//		return;
	//	}
	//	int fileLength = file.GetLength();
	//	char* get_text = new char[fileLength];
	//	memset(get_text, 0, sizeof(get_text));
	//	file.Read(get_text, fileLength);

	//	CStringA fileContent;
	//	fileContent.Format("%s", get_text);


	//	for (int hostIndex = 0; hostIndex < 2; hostIndex++) {
	//		int position = fileContent.Find(Hosts[hostIndex].host);
	//		if (fileContent.Find("#" + Hosts[hostIndex].host) == -1) {
	//			file.Seek(position, CFile::begin);
	//			CStringA hostOutput = "#" + Hosts[hostIndex].host + "\r";
	//			file.Write(hostOutput, hostOutput.GetLength());
	//			//file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
	//		}

	//		fileLength = file.GetLength();
	//		get_text = new char[fileLength];
	//		memset(get_text, 0, sizeof(get_text));
	//		file.Read(get_text, fileLength);

	//		CStringA fileContent;
	//		fileContent.Format("%s", get_text);
	//	}

	//	file.Close();
	//	free(get_text);
	//	get_text = NULL;
	//	WinExec("ipconfig /flushdns", SW_HIDE);
	//}


	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("KeyModifiers"), get_modifiers);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("KeyVk"), get_vk_code);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("Key"), get_hotkey);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("DeactivatingSoundState"), deactivatingSoundState);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ActivatingSoundState"), activatingSoundState);
	//AfxGetApp()->WriteProfileInt(_T("Settings"), _T("SwitchMode"), mode);
	UnregisterHotKey(GetSafeHwnd(), 1);
}


void CMatchmakingSwitchDlg::OnOptionsDeactivatingsound()
{
	CMenu* menu = GetMenu()->GetSubMenu(1);

	if (menu->GetMenuState(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND) == MF_CHECKED) {
		deactivatingSoundState = MF_UNCHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else if (menu->GetMenuState(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND) == MF_UNCHECKED) {
		deactivatingSoundState = MF_CHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNOFFSOUND, MF_BYCOMMAND | MF_CHECKED);
	}
}


void CMatchmakingSwitchDlg::OnOptionsActivatingsound()
{
	CMenu* menu = GetMenu()->GetSubMenu(1);
	
	if (menu->GetMenuState(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND) == MF_CHECKED) {
		activatingSoundState = MF_UNCHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else if (menu->GetMenuState(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND) == MF_UNCHECKED) {
		activatingSoundState = MF_CHECKED;
		menu->CheckMenuItem(ID_OPTIONS_TURNONSOUND, MF_BYCOMMAND | MF_CHECKED);
	}
}


void CMatchmakingSwitchDlg::OnOptionsFirewallsolosession()
{
	UpdateData(true);
	pLog->UpdateData(true);
	SYSTEMTIME systime;
	CString time;

	GetLocalTime(&systime);
	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);


	CMenu* menu = GetMenu()->GetSubMenu(1);

	if (menu->GetMenuState(ID_OPTIONS_FIREWALLSOLOSESSION, MF_BYCOMMAND) == MF_CHECKED) {
		WinExec(RulesSet[1].cmdline_disable[0], SW_HIDE);
		WinExec(RulesSet[1].cmdline_disable[1], SW_HIDE);

		firewallSoloSessionState = MF_UNCHECKED;

		pLog->m_LogContent += _T("\r\n") + time + _T("Firewall based solo session disabled");
		if (get_vk_code != 0) {
			m_Process = m_Process.Mid(32, m_Process.GetLength());
		}
		else {
			m_Process = m_Process.Mid(30, m_Process.GetLength());
		}
		

		menu->CheckMenuItem(ID_OPTIONS_FIREWALLSOLOSESSION, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else if (menu->GetMenuState(ID_OPTIONS_FIREWALLSOLOSESSION, MF_BYCOMMAND) == MF_UNCHECKED) {
		WinExec(RulesSet[1].cmdline_enable[0], SW_HIDE);
		WinExec(RulesSet[1].cmdline_enable[1], SW_HIDE);

		firewallSoloSessionState = MF_CHECKED;

		pLog->m_LogContent += _T("\r\n") + time + _T("Firewall based solo session enabled");
		if (get_vk_code != 0) {
			m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
		}
		else {
			m_Process = _T("Firewall Solo Session Enabled");
		}

		menu->CheckMenuItem(ID_OPTIONS_FIREWALLSOLOSESSION, MF_BYCOMMAND | MF_CHECKED);
	}


	//Check system firewall
	if (!CheckSystemFirewall()) {
		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
	}
	/*HRESULT hr = S_OK;
	INetFwMgr* fwMgr = NULL;
	INetFwPolicy* fwPolicy = NULL;
	INetFwProfile* fwProfile;
	hr = CoCreateInstance(
		__uuidof(NetFwMgr),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwMgr),
		(void**)&fwMgr
	);
	hr = fwMgr->get_LocalPolicy(&fwPolicy);
	if (FAILED(hr)) {
		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
	}
	hr = fwPolicy->get_CurrentProfile(&fwProfile);
	if (FAILED(hr)) {
		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
	}
	VARIANT_BOOL fwEnabled = NULL;
	hr = fwProfile->get_FirewallEnabled(&fwEnabled);
	if (FAILED(hr)) {
		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
	}
	if (fwEnabled != VARIANT_FALSE) {
		fwOn = TRUE;
	}
	else {
		fwOn = FALSE;
	}*/

	if (!fwOn) {
		pLog->m_LogContent += _T("\r\n") + time + _T("Firewall is off!!! Turn it on to make the tool work");
		pLog->m_LogContent += _T("\r\n") + time + _T("This tool can not work without firewall on");
		m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
	}
	else {
		if (get_vk_code != 0) {
			if (Hosts[hostsIndex].switchState[mode]) {
				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to deactivate");
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Deactivate");
			}
			else {
				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Activate");
			}
			if (firewallSoloSessionState) {
				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
			}
		}
	}
	//else {
	//	if (get_vk_code != 0) {
	//		if (Hosts[hostsIndex].switchState[mode]) {
	//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
	//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
	//		}
	//		else {
	//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
	//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
	//		}
	//		if (firewallSoloSessionState) {
	//			m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
	//		}
	//	}
	//	else {
	//		if (firewallSoloSessionState) {
	//			m_Process = _T("Firewall Solo Session Enabled");
	//		}
	//	}
	//}


	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
	pLog->UpdateData(false);
	UpdateData(false);
}


//void CMatchmakingSwitchDlg::OnOptionsFirewallrulesmode()
//{
//	CMenu* menu = GetMenu()->GetSubMenu(1);
//	menu->CheckMenuRadioItem(ID_OPTIONS_FIREWALLRULESMODE, ID_OPTIONS_HOSTSREDIRECTMODE, ID_OPTIONS_FIREWALLRULESMODE, MF_BYCOMMAND);
//	mode = 0;
//
//
//	UpdateData(true);
//	pLog->UpdateData(true);
//	SYSTEMTIME systime;
//	CString time;
//
//
//	GetLocalTime(&systime);
//	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//	pLog->m_LogContent += _T("\r\n") + time + _T("Now using Firewall Rules Mode, This mode require firewall enabled");
//
//
//	//Check system firewall
//	if (!CheckSystemFirewall()) {
//		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
//	}
//	/*HRESULT hr = S_OK;
//	INetFwMgr* fwMgr = NULL;
//	INetFwPolicy* fwPolicy = NULL;
//	INetFwProfile* fwProfile;
//	hr = CoCreateInstance(
//		__uuidof(NetFwMgr),
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		__uuidof(INetFwMgr),
//		(void**)&fwMgr
//	);
//	hr = fwMgr->get_LocalPolicy(&fwPolicy);
//	if (FAILED(hr)) {
//		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
//	}
//	hr = fwPolicy->get_CurrentProfile(&fwProfile);
//	if (FAILED(hr)) {
//		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
//	}
//	VARIANT_BOOL fwEnabled = NULL;
//	hr = fwProfile->get_FirewallEnabled(&fwEnabled);
//	if (FAILED(hr)) {
//		pLog->m_LogContent += _T("\r\n") + time + _T("Failed to check firewall");
//	}
//	if (fwEnabled != VARIANT_FALSE) {
//		fwOn = TRUE;
//	}
//	else {
//		fwOn = FALSE;
//	}*/
//
//	if (!fwOn) {
//		pLog->m_LogContent += _T("\r\n") + time + _T("Firewall is off!!! Turn it on to make the tool work");
//		pLog->m_LogContent += _T("\r\n") + time + _T("This tool can not work without firewall on");
//		m_Process = _T("Firewall is off!!! Turn it on to make the tool work");
//	}
//	else {
//		if (get_vk_code != 0) {
//			if (Hosts[hostsIndex].switchState[mode]) {
//				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to deactivate");
//				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Deactivate");
//			}
//			else {
//				pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
//				m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Activate");
//			}
//			if (firewallSoloSessionState) {
//				m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//			}
//		}
//		else {
//			if (firewallSoloSessionState) {
//				m_Process = _T("Firewall Solo Session Enabled");
//			}
//		}
//	}
//
//	GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState[mode]]);
//	m_Button.SetFaceColor(Hosts[hostsIndex].buttonColor[Hosts[hostsIndex].switchState[mode]], true);
//
//	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//	pLog->UpdateData(false);
//	UpdateData(false);
//}


//void CMatchmakingSwitchDlg::OnOptionsHostsredirectmode()
//{
//	CMenu* menu = GetMenu()->GetSubMenu(1);
//	menu->CheckMenuRadioItem(ID_OPTIONS_FIREWALLRULESMODE, ID_OPTIONS_HOSTSREDIRECTMODE, ID_OPTIONS_HOSTSREDIRECTMODE, MF_BYCOMMAND);
//	mode = 1;
//
//
//	UpdateData(true);
//	pLog->UpdateData(true);
//	SYSTEMTIME systime;
//	CString time;
//
//
//	GetLocalTime(&systime);
//	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//	pLog->m_LogContent += _T("\r\n") + time + _T("Now using Hosts Redirect Mode");
//
//
//	TCHAR path_temp[128];
//	_tcscpy_s(path_temp, path);
//
//	CFile file;
//	if (file.Open(path_temp, CFile::modeReadWrite) == 0) {
//		GetLocalTime(&systime);
//		time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//		MessageBox(_T("No Permission or File not Found, Please Run this Program as Administrator and Try Again"), NULL, MB_OK);
//		pLog->m_LogContent += time + _T("No Permission or File not Found, Please Run it as Administrator and Try Again");
//		return;
//	}
//	int fileLength = file.GetLength();
//	char* get_text = new char[fileLength];
//	memset(get_text, 0, sizeof(get_text));
//	file.Read(get_text, fileLength);
//
//
//	CStringA fileContent;
//	fileContent.Format("%s", get_text);
//	free(get_text);
//	get_text = NULL;
//
//
//	for (int hostIndex = 0; hostIndex < 2; hostIndex++) {
//		if (fileContent.Find(Hosts[hostIndex].host) == -1) { //If host not exist
//			file.SeekToEnd();
//			CStringA hostOutput = "\r\n#" + Hosts[hostIndex].host + "\r";
//			file.Write(hostOutput, hostOutput.GetLength());
//		}
//		else {
//			int position = fileContent.Find(Hosts[hostIndex].host);//TODO: Make it not replace text because the length is extended
//			if (fileContent.Find("#" + Hosts[hostIndex].host) == -1) { //If host is not comment
//				//position = fileContent.Find(Hosts[hostIndex].host);
//				//file.Seek(position - 1, CFile::begin);
//
//				//CStringA hostOutput = "#";
//				////CStringA hostOutput = "#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r";// + Hosts[0].host + "\r";
//				//file.Write(hostOutput, hostOutput.GetLength());
//				//file.Flush();
//				//WinExec("ipconfig /flushdns", SW_HIDE);
//
//
//				position = fileContent.Find(Hosts[hostIndex].host);
//				file.Seek(position, CFile::begin);
//				CStringA hostOutput = "#" + Hosts[hostIndex].host + "\r";
//				file.Write(hostOutput, hostOutput.GetLength());
//				//file.Write("#127.0.0.1 mm-gta5-prod.ros.rockstargames.com\r", 46);
//			}
//			if (fileContent.Find("##" + Hosts[hostIndex].host) != -1) { //If host is corrupted
//				int linestart = fileContent.Find("##" + Hosts[hostIndex].host);
//				CStringA fileContentCopy = fileContent;
//				fileContentCopy.Delete(linestart + 1, fileContentCopy.GetLength());
//				while (true) {
//					file.Seek(linestart, CFile::begin);
//					file.Write(" ", 1);
//					linestart--;
//					if (linestart < 0) {
//						break;
//					}
//					if (fileContentCopy.Find("\n", linestart) != -1) {
//						break;
//					}
//				}
//			}
//		}
//	}
//
//
//	file.Close();
//	WinExec("ipconfig /flushdns", SW_HIDE);
//
//
//	if (get_vk_code != 0) {
//		if (Hosts[hostsIndex].switchState[mode]) {
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to deactivate");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Deactivate");
//		}
//		else {
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to activate");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Activate");
//		}
//		if (firewallSoloSessionState) {
//			m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//		}
//	}
//	else {
//		if (firewallSoloSessionState) {
//			m_Process = _T("Firewall Solo Session Enabled");
//		}
//	}
//
//	GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState[mode]]);
//	m_Button.SetFaceColor(Hosts[hostsIndex].buttonColor[Hosts[hostsIndex].switchState[mode]], true);
//
//	pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//	pLog->UpdateData(false);
//	UpdateData(false);
//}


//void CMatchmakingSwitchDlg::OnOptionsMatchmakingSwitch()
//{
//	CMenu* menu = GetMenu()->GetSubMenu(1);
//	menu->CheckMenuRadioItem(ID_OPTIONS_MATCHMAKINGSWITCH, ID_OPTIONS_CLOUDSERVICESSWITCH, ID_OPTIONS_MATCHMAKINGSWITCH, MF_BYCOMMAND);
//	//menu->CheckMenuRadioItem(6, 7, 6, MF_BYPOSITION);
//	hostsIndex = 0;
//
//
//	UpdateData(true);
//	pLog->UpdateData(true);
//	SYSTEMTIME systime;
//	CString time;
//
//
//	GetLocalTime(&systime);
//	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//	pLog->m_LogContent += _T("\r\n") + time + _T("Program mode change to switching matchmaking services");
//	if (get_vk_code != 0) {
//		if (Hosts[hostsIndex].switchState) {
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
//		}
//		else {
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
//		}
//		if (firewallSoloSessionState) {
//			m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//		}
//		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//		pLog->UpdateData(false);
//	}
//	else {
//		if (firewallSoloSessionState) {
//			m_Process = _T("Firewall Solo Session Enabled");
//		}
//	}
//	UpdateData(false);
//
//
//	GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState]);
//	AfxGetMainWnd()->SetWindowTextW(_T("MatchingSwitch"));
//	if (!Hosts[hostsIndex].switchState) {
//		m_Button.SetFaceColor(RGB(129, 0, 24), true);
//	}
//	else {
//		m_Button.SetFaceColor(RGB(0, 129, 105), true);
//	}
//
//	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("Switch MatchingSwitch between on and off, when it's off randoms can join, when it's on randoms will not join."));
//}


//void CMatchmakingSwitchDlg::OnOptionsCloudservicesSwitch()
//{
//	CMenu* menu = GetMenu()->GetSubMenu(1);
//	menu->CheckMenuRadioItem(ID_OPTIONS_MATCHMAKINGSWITCH, ID_OPTIONS_CLOUDSERVICESSWITCH, ID_OPTIONS_CLOUDSERVICESSWITCH, MF_BYCOMMAND);
//	hostsIndex = 1;
//	advancedMode = true;
//
//
//	UpdateData(true);
//	pLog->UpdateData(true);
//	SYSTEMTIME systime;
//	CString time;
//
//
//	GetLocalTime(&systime);
//	time.Format(_T("%02d:%02d:%02d "), systime.wHour, systime.wMinute, systime.wSecond);
//
//	pLog->m_LogContent += _T("\r\n") + time + _T("Program mode change to switching cloud services");
//	if (get_vk_code != 0) {
//		if (Hosts[hostsIndex].switchState) {
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn off");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn off");
//		}
//		else {
//			pLog->m_LogContent += _T("\r\n") + time + _T("Press ") + get_hotkey.MakeUpper() + _T(" to turn on");
//			m_Process = _T("Press ") + get_hotkey.MakeUpper() + _T(" to Turn on");
//		}
//		if (advancedMode) {
//			m_Process = Hosts[1].advancedModeText + Hosts[1].state[Hosts[1].switchState] + _T(" | ") + m_Process;
//			m_Process = Hosts[0].advancedModeText + Hosts[0].state[Hosts[0].switchState] + _T(" | ") + m_Process;
//		}
//		if (firewallSoloSessionState) {
//			m_Process = _T("Firewall Solo Session Enabled | ") + m_Process;
//		}
//		pLog->m_LogCtrl.LineScroll(pLog->m_LogCtrl.GetLineCount(), 0);
//		pLog->UpdateData(false);
//	}
//	else {
//		if (advancedMode) {
//			m_Process = Hosts[0].advancedModeText + Hosts[0].state[Hosts[0].switchState] + _T(" | ");
//			m_Process += Hosts[1].advancedModeText + Hosts[1].state[Hosts[1].switchState];
//		}
//		if (firewallSoloSessionState) {
//			m_Process = _T("Firewall Solo Session Enabled");
//		}
//	}
//	UpdateData(false);
//
//
//	GetDlgItem(IDC_BUTTON1)->SetWindowTextW(buttonText[hostsIndex] + Hosts[hostsIndex].state[Hosts[hostsIndex].switchState]);
//	AfxGetMainWnd()->SetWindowTextW(_T("SavingfailedSwitch"));
//	if (!Hosts[hostsIndex].switchState) {
//		m_Button.SetFaceColor(RGB(129, 0, 24), true);
//	}
//	else {
//		m_Button.SetFaceColor(RGB(0, 129, 105), true);
//	}
//
//	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUTTON1), _T("Switch SavingfailedSwitch between on and off, when it's off game is able to save, when it's on game can not save."));
//}


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

BOOL CMatchmakingSwitchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTipCtrl.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}

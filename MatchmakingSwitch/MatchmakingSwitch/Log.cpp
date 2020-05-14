// Log.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MatchmakingSwitch.h"
#include "Log.h"
#include "afxdialogex.h"


// Log �Ի���

IMPLEMENT_DYNAMIC(Log, CDialogEx)

Log::Log(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOG_DIALOG, pParent)
	, m_LogContent(_T(""))
{

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


// Log ��Ϣ�������


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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void Log::OnEnSetfocusLog()
{
	m_LogCtrl.SetSel(-1, -1);
	::HideCaret(GetDlgItem(IDC_LOG)->GetSafeHwnd());
}


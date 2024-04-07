
// springsDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "springs.h"
#include "springsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CspringsDlg dialog



CspringsDlg::CspringsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPRINGS_DIALOG, pParent)
	, m_mu(0)
	, m_visc(0)
	, m_N(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CspringsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLOTAREA, m_PlotArea);
	//  DDX_Control(pDX, IDC_N, m_N);
	DDX_Text(pDX, IDC_MU, m_mu);
	DDX_Text(pDX, IDC_VISC, m_visc);
	DDX_Control(pDX, IDC_PLOTAREA2, m_PlotArea2);
	DDX_Text(pDX, IDC_N, m_N);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_SETBALLS, m_Settings);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	DDX_Control(pDX, IDC_N, m_nedit);
	DDX_Control(pDX, IDC_MU, m_muedit);
	DDX_Control(pDX, IDC_VISC, m_viscedit);
}

BEGIN_MESSAGE_MAP(CspringsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, &CspringsDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CspringsDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SETBALLS, &CspringsDlg::OnBnClickedSetballs)
	ON_BN_CLICKED(IDC_APPLY, &CspringsDlg::OnBnClickedApply)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CspringsDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CspringsDlg message handlers
extern DWORD WINAPI ThreadFunc(PVOID);
extern int N;
extern double u;
extern double visc;
BOOL CspringsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	thrParams.hCalculateEvent = CreateEvent(NULL, TRUE, FALSE, L"");
	thrParams.hDrawEvent = CreateEvent(NULL, TRUE, FALSE, L"");
	thrParams.pPlotarea = &m_PlotArea;
	thrParams.pPlotarea2 = &m_PlotArea2;
	thrParams.hwnd = GetSafeHwnd();

	m_Start.EnableWindow(FALSE);
	m_Settings.EnableWindow(FALSE);
	m_Apply.EnableWindow(TRUE);
	m_Stop.EnableWindow(FALSE);

	m_PlotArea2.Initialize(0,0,0,0,0);

	hThread = CreateThread(NULL, 0, ThreadFunc, &thrParams, CREATE_SUSPENDED, NULL);
	suspendCount = 1;

	N = m_N;
	u = m_mu;
	visc = m_visc;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CspringsDlg::OnPaint()
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
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CspringsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


extern double dt;
DWORD timeOut;
extern std::vector<Ball*> allBalls;
extern std::vector<Spring*> allSprings;
void CspringsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	//double tElapsed=0;
	//while (tElapsed < 0.02)
	//{
	//	m_PlotArea.RKIter();
	//	tElapsed += dt;
	//}
	//m_PlotArea.Invalidate(0);

	timeOut = WaitForSingleObject(thrParams.hDrawEvent, 0);
//	if (timeOut == WAIT_TIMEOUT) { KillTimer(1); CDialogEx::OnTimer(nIDEvent);}
	if (timeOut == WAIT_TIMEOUT) { CDialogEx::OnTimer(nIDEvent); return; }
	ResetEvent(thrParams.hDrawEvent);
	timeElapsed += 0.2;
	m_PlotArea2.offsetX -= 0.2*m_PlotArea2.scaleX;
	m_PlotArea2.AddPoint(timeElapsed, allBalls[curBall+1]->x0);
	m_PlotArea2.Invalidate(0);
	m_PlotArea.Invalidate(0);
	SetEvent(thrParams.hCalculateEvent);

	CDialogEx::OnTimer(nIDEvent);
}



void CspringsDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	if (suspendCount == 0) return;
	suspendCount--;
	ResumeThread(hThread);
	SetEvent(thrParams.hCalculateEvent);
	SetTimer(1,20, NULL);

	m_Start.EnableWindow(FALSE);
	m_Settings.EnableWindow(FALSE);
	m_Apply.EnableWindow(FALSE);
	m_Stop.EnableWindow(TRUE);

	m_nedit.SetReadOnly(1);
	m_muedit.SetReadOnly(1);
	m_viscedit.SetReadOnly(1);
	m_combo.EnableWindow(FALSE);
}


void CspringsDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	if (suspendCount != 0)return;
	suspendCount++;
	KillTimer(1);
	WaitForSingleObject(thrParams.hDrawEvent, INFINITE); 
	SuspendThread(hThread);

	m_Start.EnableWindow(TRUE);
	m_Settings.EnableWindow(TRUE);
	m_Apply.EnableWindow(TRUE);
	m_Stop.EnableWindow(FALSE);

	m_nedit.SetReadOnly(0);
	m_muedit.SetReadOnly(0);
	m_viscedit.SetReadOnly(0);
	m_combo.EnableWindow(TRUE);
}

#include "settingsStruct.h"
DWORD WINAPI ThreadFunc(PVOID param)
{

	while (true)
	{
		WaitForSingleObject(((ThreadParam*)param)->hCalculateEvent, INFINITE);
		ResetEvent(((ThreadParam*)param)->hCalculateEvent);
		double tElapsed = 0;
		while (tElapsed < 0.02)
		{
			((ThreadParam*)param)->pPlotarea->RKIter();
			tElapsed += dt;
		}
		//if (timeOut == WAIT_TIMEOUT) SetTimer(((ThreadParam*)param)->hwnd,1, 0, NULL);
		
		SetEvent(((ThreadParam*)param)->hDrawEvent);
	}
	return 0;
}

void CspringsDlg::OnBnClickedSetballs()
{
	// TODO: Add your control notification handler code here
	if (N == 0) { MessageBox(L"”кажите параметры системы (N != 0)", L"опа"); return; }

	Settings settings;
	settings.balls = allBalls;
	settings.springs = allSprings;
	settings.pplotarea = &m_PlotArea;


	CSettingsDlg dialog(CWnd::FromHandle(GetSafeHwnd()), settings);
	dialog.DoModal();
	//m_PlotArea.Invalidate(0);

}


void CspringsDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	N = m_N + 2;
	u = m_mu;
	visc = m_visc;

	m_PlotArea.Reset();
	m_PlotArea.Invalidate(0);
	m_PlotArea2.Reset();
	m_PlotArea2.Invalidate(0);
	for (int i = m_combo.GetCount() - 1; i >= 0; i--)
	{
		m_combo.DeleteString(i);
	}
	for (int i = 1; i < allBalls.size()-1; i++) 
	{
		wchar_t str[10];
		swprintf_s(str, L"%d", i);
		m_combo.AddString(str);
	}
	m_combo.SetCurSel(0);
	curBall = 0;
	timeElapsed = 0;
	m_PlotArea2.ToOrigin();

	m_Start.EnableWindow(TRUE);
	m_Settings.EnableWindow(TRUE);
	m_Apply.EnableWindow(TRUE);
	m_Stop.EnableWindow(FALSE);

}


void CspringsDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	int i = m_combo.GetCurSel();
	curBall = i;


}

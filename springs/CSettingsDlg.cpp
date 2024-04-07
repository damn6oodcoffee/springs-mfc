// CSettingsDlg.cpp : implementation file
//
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "springs.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"


// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CSettingsDlg, pParent)
	, m_x0_val(0)
	, m_vx0_val(0)
	, m_m(0)
	, m_k0(0)
	
{

}

CSettingsDlg::CSettingsDlg(CWnd* pParent,Settings &settings)
	: CDialog(IDD_CSettingsDlg, pParent)
{
	
	balls = settings.balls;
	springs = settings.springs;
	pplotarea = settings.pplotarea;
	
}


CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBALLS, m_combo);
	DDX_Control(pDX, IDC_X0, m_x0);
	DDX_Control(pDX, IDC_VX0, m_vx0);
	DDX_Text(pDX, IDC_X0, m_x0_val);
	DDX_Text(pDX, IDC_VX0, m_vx0_val);
	DDX_Text(pDX, IDC_M, m_m);
	//  DDX_Text(pDX, IDC_N, m_N);
	//  DDX_Text(pDX, IDC_MU, m_mu);
	DDX_Text(pDX, IDC_K0, m_k0);
	DDX_Control(pDX, IDC_COMBOSPRINGS, m_combo2);
	DDX_Control(pDX, IDC_RADIO2, m_radio1);
	DDX_Control(pDX, IDC_RADIO1, m_radio2);
	//  DDX_Text(pDX, IDC_VISC, m_visc);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &CSettingsDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_EXIT, &CSettingsDlg::OnBnClickedExit)
	ON_CBN_SELCHANGE(IDC_COMBOBALLS, &CSettingsDlg::OnCbnSelchangeComboballs)
	ON_CBN_SELCHANGE(IDC_COMBOSPRINGS, &CSettingsDlg::OnCbnSelchangeCombosprings)
	ON_BN_CLICKED(IDC_BUTTON1, &CSettingsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSettingsDlg message handlers


void CSettingsDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int i = m_combo.GetCurSel() + 1;
	balls[i]->Set(m_x0_val);
	balls[i]->vx0 = m_vx0_val;
	balls[i]->m = m_m;

	i = m_combo2.GetCurSel();
	int type = m_radio2.GetState();
	springs[i]->type = type;
	springs[i]->k0 = m_k0;

	pplotarea->Invalidate(0);

	
	//OnOK();
}


void CSettingsDlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


BOOL CSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	for (int i = 1; i < balls.size() - 1; i++)
	{
		wchar_t str[] = L"%d";
		swprintf_s(str, L"%d", i);
		m_combo.AddString(str);
	}

	for (int i = 0; i < springs.size(); i++)
	{
		wchar_t str[] = L"%d";
		swprintf_s(str, L"%d", i+1);
		m_combo2.AddString(str);
	}

	m_combo.SetCurSel(0);
	m_combo2.SetCurSel(0);
	OnCbnSelchangeComboballs();
	OnCbnSelchangeCombosprings();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSettingsDlg::OnCbnSelchangeComboballs()
{
	// TODO: Add your control notification handler code here
	int i = m_combo.GetCurSel()+1;
	m_x0_val = balls[i]->x0;
	m_vx0_val = balls[i]->vx0;
	m_m = balls[i]->m;
	UpdateData(FALSE);
}


void CSettingsDlg::OnCbnSelchangeCombosprings()
{
	// TODO: Add your control notification handler code here
	int i = m_combo2.GetCurSel();
	m_k0 = springs[i]->k0;
	bool state = springs[i]->type;
	m_radio1.SetCheck(!state);
	m_radio2.SetCheck(state);


	UpdateData(FALSE);
}


void CSettingsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	int type = m_radio2.GetState();
	for (int i = 0; i < springs.size(); i++)
	{
		springs[i]->type = type;
		springs[i]->k0 = m_k0;
	}
}

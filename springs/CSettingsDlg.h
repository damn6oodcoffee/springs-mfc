#pragma once
#include "afxdialogex.h"
#include "PlotArea.h"
#include "settingsStruct.h"

// CSettingsDlg dialog

class CSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = nullptr);   // standard constructor
	CSettingsDlg(CWnd* pParent, Settings& settings);   // my constructor
	virtual ~CSettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSettingsDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedExit();
	CComboBox m_combo;
	CEdit m_x0;
	CEdit m_vx0;
	virtual BOOL OnInitDialog();

private:
	std::vector<Ball*> balls;
	std::vector<Spring*> springs;
	PlotArea* pplotarea;
public:
	afx_msg void OnCbnSelchangeComboballs();
	double m_x0_val;
	double m_vx0_val;
	double m_m;
//	int m_N;
//	double m_mu;
	double m_k0;
	CComboBox m_combo2;
	CButton m_radio1;
	CButton m_radio2;
	afx_msg void OnCbnSelchangeCombosprings();
//	double m_visc;
	afx_msg void OnBnClickedButton1();
};

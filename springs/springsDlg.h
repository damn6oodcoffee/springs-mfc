
// springsDlg.h : header file
//

#pragma once
#include "PlotArea.h"
#include "PlotArea2.h"
#include "CSettingsDlg.h"

struct ThreadParam
{
	PlotArea* pPlotarea;
	PlotArea2* pPlotarea2;
	HWND hwnd;
	HANDLE hCalculateEvent;
	HANDLE hDrawEvent;
};

// CspringsDlg dialog
class CspringsDlg : public CDialogEx
{
// Construction
public:
	CspringsDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRINGS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	PlotArea m_PlotArea;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
private:
	HANDLE hThread;
	ThreadParam thrParams;
	DWORD suspendCount;
	
public:
	afx_msg void OnBnClickedSetballs();
//	int m_N;
	double m_mu;
	double m_visc;
	PlotArea2 m_PlotArea2;
	afx_msg void OnBnClickedApply();
	int m_N;
	int curBall;
	double timeElapsed;
	CComboBox m_combo;
	afx_msg void OnCbnSelchangeCombo1();
	CButton m_Start;
	CButton m_Stop;
	CButton m_Settings;
	CButton m_Apply;
	CEdit m_nedit;
	CEdit m_muedit;
	CEdit m_viscedit;
};


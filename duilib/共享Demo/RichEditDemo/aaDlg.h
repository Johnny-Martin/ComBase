// aaDlg.h : header file
//

#if !defined(AFX_AADLG_H__23BBED8F_3F84_403F_82F9_6B877E00EF30__INCLUDED_)
#define AFX_AADLG_H__23BBED8F_3F84_403F_82F9_6B877E00EF30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bb.h"
/////////////////////////////////////////////////////////////////////////////
// CAaDlg dialog

class CAaDlg : public CDialog
{
// Construction
public:
	CAaDlg(CWnd* pParent = NULL);	// standard constructor
	
IExRichEditOleCallback roc;
IRichEditOle	*m_pRichEditOle;
// Dialog Data
	//{{AFX_DATA(CAaDlg)
	enum { IDD = IDD_AA_DIALOG };
	CRichEditCtrl	m_re;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AADLG_H__23BBED8F_3F84_403F_82F9_6B877E00EF30__INCLUDED_)

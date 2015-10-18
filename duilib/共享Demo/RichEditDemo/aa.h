// aa.h : main header file for the AA application
//

#if !defined(AFX_AA_H__2A1C364D_797A_4846_AB41_E5A60AD85C66__INCLUDED_)
#define AFX_AA_H__2A1C364D_797A_4846_AB41_E5A60AD85C66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAaApp:
// See aa.cpp for the implementation of this class
//

class CAaApp : public CWinApp
{
public:
	CAaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAaApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AA_H__2A1C364D_797A_4846_AB41_E5A60AD85C66__INCLUDED_)

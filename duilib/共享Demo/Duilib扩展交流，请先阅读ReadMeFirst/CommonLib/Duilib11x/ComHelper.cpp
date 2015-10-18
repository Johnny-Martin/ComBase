#include "StdAfx.h"


namespace DuiLib {


	CRect ParseRect(LPCTSTR lpszText)
	{
		LPTSTR lpszValue = nullptr;
		CRect rc;
		rc.left   = _tcstol(lpszText,      &lpszValue, 10);  ASSERT(lpszValue);
		rc.top    = _tcstol(lpszValue + 1, &lpszValue, 10);  ASSERT(lpszValue);
		rc.right  = _tcstol(lpszValue + 1, &lpszValue, 10);  ASSERT(lpszValue);
		rc.bottom = _tcstol(lpszValue + 1, &lpszValue, 10);  ASSERT(lpszValue);
		return rc;
	}


} //namespace DuiLib
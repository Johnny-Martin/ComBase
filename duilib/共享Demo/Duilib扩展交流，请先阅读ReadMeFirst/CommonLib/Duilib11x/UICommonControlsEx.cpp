#include "StdAfx.h"
#include "ComHelper.h"


namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	LPCTSTR CCheckBoxUI::GetClass() const
	{
		return _T("CheckBoxUI");
	}

	void CCheckBoxUI::SetCheck(bool bCheck)
	{
		Selected(bCheck);
	}

	bool  CCheckBoxUI::GetCheck() const
	{
		return IsSelected();
	}

	//////////////////////////////////////////////////////////////////////////
	//
	CComboBoxUI::CComboBoxUI()
	{
		m_nArrowWidth = 0;
	}

	LPCTSTR CComboBoxUI::GetClass() const
	{
		return _T("ComboBoxUI");
	}
	
	void CComboBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("arrowimage")) == 0)
			m_sArrowImage = pstrValue;
		else
			CComboUI::SetAttribute(pstrName, pstrValue);
	}
	
	bool CComboBoxUI::SelectString(LPCTSTR lpszText)
	{
		int nCount = GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			CControlUI* pItem = (CControlUI*)GetItemAt(i);
			if (pItem && pItem->GetText() == lpszText)
				return SelectItem(i, false);
		}
		return false;
	}

	void CComboBoxUI::PaintStatusImage(HDC hDC)
	{
		if (m_sArrowImage.IsEmpty())
			CComboUI::PaintStatusImage(hDC);
		else
		{
			// get index
			if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
			else m_uButtonState &= ~ UISTATE_FOCUSED;
			if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
			else m_uButtonState &= ~ UISTATE_DISABLED;

			int nIndex = 0;
			if ((m_uButtonState & UISTATE_DISABLED) != 0)
				nIndex = 4;
			else if ((m_uButtonState & UISTATE_PUSHED) != 0)
				nIndex = 2;
			else if ((m_uButtonState & UISTATE_HOT) != 0)
				nIndex = 1;
			else if ((m_uButtonState & UISTATE_FOCUSED) != 0)
				nIndex = 3;

			// make modify string
			CStdString sModify = m_sArrowImage;

			int nPos1 = sModify.Find(_T("source"));
			int nPos2 = sModify.Find(_T("'"), nPos1 + 7);
			if (nPos2 == -1) return; //first
			int nPos3 = sModify.Find(_T("'"), nPos2 + 1);
			if (nPos3 == -1) return; //second

			CRect rcBmpPart = ParseRect(sModify.GetData() + nPos2 + 1);
			m_nArrowWidth = rcBmpPart.GetWidth() / 5;
			rcBmpPart.left += nIndex * m_nArrowWidth;
			rcBmpPart.right = rcBmpPart.left + m_nArrowWidth;

			CRect rcDest(0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
			rcDest.Deflate(GetBorderSize(), GetBorderSize());
			rcDest.left = rcDest.right - m_nArrowWidth;

			CStdString sSource = sModify.Mid(nPos1, nPos3 + 1 - nPos1);
			CStdString sReplace;
			sReplace.SmallFormat(_T("source='%d,%d,%d,%d' dest='%d,%d,%d,%d'"),
				rcBmpPart.left, rcBmpPart.top, rcBmpPart.right, rcBmpPart.bottom,
				rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);

			sModify.Replace(sSource, sReplace);

			// draw image
			if (!DrawImage(hDC, m_sArrowImage, sModify))
				m_sArrowImage.Empty();
		}
	}

	void CComboBoxUI::PaintText(HDC hDC)
	{
		RECT rcText = m_rcItem;
		rcText.right -= m_nArrowWidth; // add this line than CComboUI::PaintText(HDC hDC)
		rcText.left += m_rcTextPadding.left;
		rcText.right -= m_rcTextPadding.right;
		rcText.top += m_rcTextPadding.top;
		rcText.bottom -= m_rcTextPadding.bottom;

		if( m_iCurSel >= 0 ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
			IListItemUI* pElement = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if( pElement != NULL ) {
				pElement->DrawItemText(hDC, rcText);
			}
			else {
				RECT rcOldPos = pControl->GetPos();
				pControl->SetPos(rcText);
				pControl->DoPaint(hDC, rcText);
				pControl->SetPos(rcOldPos);
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//

	class CDateTimeWnd : public CWindowWnd
	{
	public:
		CDateTimeWnd();

		void Init(CDateTimeUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;

		void OnFinalMessage(HWND hWnd);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		CDateTimeUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
	};

	CDateTimeWnd::CDateTimeWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
	{
	}

	void CDateTimeWnd::Init(CDateTimeUI* pOwner)
	{
		m_pOwner = pOwner;

		if (m_hWnd == NULL)
		{
			RECT rcPos = CalPos();
			UINT uStyle = WS_CHILD;
			Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
			SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
		}

		if (m_pOwner->GetText().IsEmpty())
			::GetLocalTime(&m_pOwner->m_sysTime);

		::SendMessage(m_hWnd, DTM_SETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);

		m_bInit = true;    
	}

	RECT CDateTimeWnd::CalPos()
	{
		CRect rcPos = m_pOwner->GetPos();
		if (m_pOwner->m_nClearWidth > 0)
			rcPos.right -= m_pOwner->m_nClearWidth + 1;
		//时间控件的边框貌似无法去掉，只能覆盖整个窗口
		//rcPos.Deflate(m_pOwner->GetBorderSize(), m_pOwner->GetBorderSize());
		return rcPos;
	}

	LPCTSTR CDateTimeWnd::GetWindowClassName() const
	{
		return _T("DateTimeWnd");
	}

	LPCTSTR CDateTimeWnd::GetSuperClassName() const
	{
		return DATETIMEPICK_CLASS;
	}

	void CDateTimeWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		// Clear reference and die
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CDateTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if( uMsg == WM_KILLFOCUS )
		{
			LRESULT lResult = CWindowWnd::HandleMessage(uMsg, wParam, lParam);

			if (NULL == (HWND)::SendMessage(m_hWnd, DTM_GETMONTHCAL, 0, 0))
				PostMessage(WM_CLOSE); //失去焦点就关闭(非下拉状态)

			return lResult;
		}
		else if (uMsg == OCM_NOTIFY)
		{
			if (lParam && DTN_DATETIMECHANGE == ((NMHDR*)(lParam))->code) //时间变化则更新
			{
				::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
				m_pOwner->UpdateText();
				return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			}
			if (lParam && DTN_CLOSEUP == ((NMHDR*)(lParam))->code) //选择后直接关闭
			{
				LRESULT lResult = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
				PostMessage(WM_CLOSE); //失去焦点就关闭
				return lResult;
			}
		}

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	CDateTimeUI::CDateTimeUI()
	{
		::GetLocalTime(&m_sysTime);
		m_bReadOnly = false;
		m_pWindow = nullptr;
		m_nArrowWidth = 0;
		m_nClearWidth = 0;
	}

	LPCTSTR CDateTimeUI::GetClass() const
	{
		return _T("DateTimeUI");
	}

	LPVOID CDateTimeUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("DateTime")) == 0 ) return static_cast<CDateTimeUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	void CDateTimeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("arrowimage")) == 0)
			m_sArrowImage = pstrValue;
		if (_tcscmp(pstrName, _T("clearimage")) == 0)
			m_sClearImage = pstrValue;
		else
			CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	SYSTEMTIME& CDateTimeUI::GetTime()
	{
		return m_sysTime;
	}

	void CDateTimeUI::SetTime(SYSTEMTIME* pst)
	{
		m_sysTime = *pst;
		Invalidate();
	}

	void CDateTimeUI::SetReadOnly(bool bReadOnly)
	{
		m_bReadOnly = bReadOnly;
		Invalidate();
	}

	bool CDateTimeUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CDateTimeUI::UpdateText()
	{
		CStdString sText;
		sText.SmallFormat(_T("%4d-%02d-%02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
		SetText(sText);
	}

	CRect CDateTimeUI::GetClearRect()
	{
		CRect rc(0,0,0,0);
		if (m_nClearWidth > 0)
		{
			rc = m_rcItem;
			rc.Deflate(0, GetBorderSize());
			rc.left = rc.right - m_nClearWidth;
		}
		return rc;
	}

	CRect CDateTimeUI::GetArrowRect()
	{
		CRect rc(0,0,0,0);
		if (m_nArrowWidth > 0)
		{
			rc = m_rcItem;
			if (m_nClearWidth > 0)
				rc.right -= m_nClearWidth + 1;
			rc.Deflate(GetBorderSize(), GetBorderSize());
			rc.left = rc.right - m_nArrowWidth;
		}
		return rc;
	}

	CRect CDateTimeUI::GetTextRect()
	{
		CRect rc = m_rcItem;
		if (m_nClearWidth > 0)
			rc.right -= m_nClearWidth + 1;//清除按钮，并留1个像素的间隔
		rc.Deflate(GetBorderSize(), GetBorderSize());
		if (m_nArrowWidth > 0)
			rc.right -= m_nArrowWidth;//下拉按钮
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		return rc;
	}

	void CDateTimeUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindow != NULL ) return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			//if( m_pWindow ) return;
			//m_pWindow = new CDateTimeWnd();
			//ASSERT(m_pWindow);
			//m_pWindow->Init(this);
			//m_pWindow->ShowWindow();
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if (m_pWindow == NULL)
				{
					m_pWindow = new CDateTimeWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);
				}

				CRect rcClear = GetClearRect();
				if (::PtInRect(&rcClear, event.ptMouse))//在清空框
				{
					if( m_pWindow != NULL )
						m_pWindow->ShowWindow(false);
				}
				else //不在清空框
				{
					CRect rcArrow = GetArrowRect();
					if (::PtInRect(&rcArrow, event.ptMouse))
					{
						if( m_pWindow != NULL )
						{
							m_pWindow->ShowWindow();
							//自动下拉
							CPoint pt = event.ptMouse;
							::ClientToScreen(m_pManager->GetPaintWindow(), &pt);
							::ScreenToClient(m_pWindow->GetHWND(), &pt);
							::SendMessage(m_pWindow->GetHWND(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
							if (m_pWindow && m_pWindow->GetHWND()) //要判断，xp下已经关闭了，有点奇怪
								::SendMessage(m_pWindow->GetHWND(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
						}
					}
					else
					{
						if( m_pWindow != NULL )
						{
							m_pWindow->ShowWindow();
							::SetFocus(m_pWindow->GetHWND());
						}
					}
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			CRect rcClear = GetClearRect();
			if (::PtInRect(&rcClear, event.ptMouse))
				SetText(_T(""));
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			return;
		}

		CLabelUI::DoEvent(event);
	}

	void CDateTimeUI::PaintStatusImage(HDC hDC)
	{
		if (!m_sClearImage.IsEmpty())
		{
			int nIndex = 0; //以后增加状态用

			// make modify string
			CStdString sModify = m_sClearImage;
			bool bSource = true;
			int nPos1 = sModify.Find(_T("source"));
			int nPos2 = sModify.Find(_T("'"), nPos1 + 7);
			if (nPos2 == -1) bSource = false; //first
			int nPos3 = sModify.Find(_T("'"), nPos2 + 1);
			if (nPos3 == -1) bSource = false; //second

			if (bSource)
			{
				CRect rcBmpPart = ParseRect(sModify.GetData() + nPos2 + 1);
				m_nClearWidth = rcBmpPart.GetWidth() / 5;
				rcBmpPart.left += nIndex * m_nClearWidth;
				rcBmpPart.right = rcBmpPart.left + m_nClearWidth;

				CRect rcDest(0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
				rcDest.Deflate(0, GetBorderSize());
				rcDest.left = rcDest.right - m_nClearWidth;

				CStdString sSource = sModify.Mid(nPos1, nPos3 + 1 - nPos1);
				CStdString sReplace;
				sReplace.SmallFormat(_T("source='%d,%d,%d,%d' dest='%d,%d,%d,%d'"),
					rcBmpPart.left, rcBmpPart.top, rcBmpPart.right, rcBmpPart.bottom,
					rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);

				sModify.Replace(sSource, sReplace);

				// draw image
				if (!DrawImage(hDC, m_sClearImage, sModify))
					m_sClearImage.Empty();
			}
		}

		if (!m_sArrowImage.IsEmpty())
		{
			int nIndex = 0; //以后增加状态用

			// make modify string
			CStdString sModify = m_sArrowImage;
			bool bSource = true;
			int nPos1 = sModify.Find(_T("source"));
			int nPos2 = sModify.Find(_T("'"), nPos1 + 7);
			if (nPos2 == -1) bSource = false; //first
			int nPos3 = sModify.Find(_T("'"), nPos2 + 1);
			if (nPos3 == -1) bSource = false; //second

			if (bSource)
			{
				CRect rcBmpPart = ParseRect(sModify.GetData() + nPos2 + 1);
				m_nArrowWidth = rcBmpPart.GetWidth() / 5;
				rcBmpPart.left += nIndex * m_nArrowWidth;
				rcBmpPart.right = rcBmpPart.left + m_nArrowWidth;

				CRect rcDest(0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
				rcDest.Deflate(GetBorderSize(), GetBorderSize());
				if (m_nClearWidth > 0)
					rcDest.right -= m_nClearWidth + 1;
				rcDest.left = rcDest.right - m_nArrowWidth;

				CStdString sSource = sModify.Mid(nPos1, nPos3 + 1 - nPos1);
				CStdString sReplace;
				sReplace.SmallFormat(_T("source='%d,%d,%d,%d' dest='%d,%d,%d,%d'"),
					rcBmpPart.left, rcBmpPart.top, rcBmpPart.right, rcBmpPart.bottom,
					rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);

				sModify.Replace(sSource, sReplace);

				// draw image
				if (!DrawImage(hDC, m_sArrowImage, sModify))
					m_sArrowImage.Empty();
			}
		}
	}

	void CDateTimeUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		RECT rc = GetTextRect();

		if( IsEnabled() ) {
			if( m_bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
				NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else {
			if( m_bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
				NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
	}

	void CDateTimeUI::PaintBorder(HDC hDC)
	{
		if (m_nClearWidth == 0)
		{
			CLabelUI::PaintBorder(hDC);
		}
		else
		{
			CRect rcItem = m_rcItem;
			rcItem.right -= m_nClearWidth + 1; //清除按钮，并留1个像素的间隔

			if( m_nBorderSize > 0)
			{
				if( m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0 )//画圆角边框
				{
					if (IsFocused() && m_dwFocusBorderColor != 0)
						CRenderEngine::DrawRoundRect(hDC, rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwFocusBorderColor));
					else
						CRenderEngine::DrawRoundRect(hDC, rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor));
				}
				else
				{
					if (IsFocused() && m_dwFocusBorderColor != 0)
						CRenderEngine::DrawRect(hDC, rcItem, m_nBorderSize, GetAdjustColor(m_dwFocusBorderColor));
					else
						CRenderEngine::DrawRect(hDC, rcItem, m_nBorderSize, GetAdjustColor(m_dwBorderColor));
				}
			}
		}
	}


} // namespace DuiLib

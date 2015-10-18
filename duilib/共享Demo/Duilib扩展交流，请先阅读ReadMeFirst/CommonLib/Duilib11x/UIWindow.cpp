#include "StdAfx.h"


namespace DuiLib {


CWindowUI::CWindowUI(void)
{
}


CWindowUI::~CWindowUI(void)
{
}

HWND CWindowUI::CreateX(HWND hwndParent, DWORD dwStyle, LPCTSTR lpszXml)
{
	m_xmlCreate = lpszXml;
	return CWindowWnd::Create(hwndParent, _T(""), dwStyle, 0);
}

UINT CWindowUI::DoModal(HWND hwndParent, LPCTSTR lpszXml)
{
	HWND hWnd = CreateX(hwndParent, UI_WNDSTYLE_DIALOG, lpszXml);
	if (hWnd == NULL) return -1;

	CenterWindow();
	return ShowModal();;
}

LRESULT CWindowUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	BOOL    bHandled = FALSE;

	//���ɱ��ദ����Ϣ
	switch (uMsg)
	{
	case WM_CREATE:        lResult = OnCreate(uMsg, wParam, lParam, bHandled);        break;
	case WM_CLOSE:         lResult = OnClose(uMsg, wParam, lParam, bHandled);         break;
	case WM_DESTROY:       lResult = OnDestroy(uMsg, wParam, lParam, bHandled);       break;
	case WM_NCDESTROY:     lResult = OnNcDestroy(uMsg, wParam, lParam, bHandled);     break;
	case WM_NCACTIVATE:    lResult = OnNcActivate(uMsg, wParam, lParam, bHandled);    break;
	case WM_NCHITTEST:     lResult = OnNcHitTest(uMsg, wParam, lParam, bHandled);     break;
	case WM_SIZE:          lResult = OnSize(uMsg, wParam, lParam, bHandled);          break;
	case WM_GETMINMAXINFO: lResult = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_COMMAND:       lResult = OnCommand(uMsg, wParam, lParam, bHandled);       break;
	case WM_SYSCOMMAND:    lResult = OnSysCommand(uMsg, wParam, lParam, bHandled);    break;
	default:
		bHandled = FALSE;
	}
	if (bHandled)
		return lResult;

	//���ƹ���������
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lResult))
		return lResult;

	//���ദ��
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CWindowUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CStdString sName = msg.pSender->GetName();
		if (sName == _T("sysbtn_min"))
		{
			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (sName == _T("sysbtn_max"))
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if (sName == _T("sysbtn_cls"))
		{
			PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		else if (sName == _T("sysbtn_rst"))
		{
			PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else if (sName == _T("sysbtn_ok"))
		{
			PostMessage(WM_CLOSE, IDOK, 0);
		}
		else if (sName == _T("sysbtn_cancel"))
		{
			PostMessage(WM_CLOSE, IDCANCEL, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

LRESULT CWindowUI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//������Ϣ��
	m_PaintManager.Init(m_hWnd);

	//����xml����
	if(!m_xmlCreate.IsEmpty())
	{
		//ȥ������������xml������
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~(WS_CAPTION | WS_THICKFRAME);
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		//�������ã�����
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create((LPCTSTR)m_xmlCreate, (UINT)0, NULL, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
	}

	return 0;
}

LRESULT CWindowUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::DestroyWindow(m_hWnd);//���ٴ���
	m_hWnd = NULL;
	bHandled = TRUE;
	return 0;
}

LRESULT CWindowUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CWindowUI::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);//�˳��߳�
	bHandled = TRUE;
	return 0;
}

LRESULT CWindowUI::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this))
		bHandled = FALSE;

	return (wParam == 0);
}

LRESULT CWindowUI::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this))//������󻯵������������
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	//�ж��Ƿ������
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left &&
		pt.x < rcClient.right - rcCaption.right &&
		pt.y >= rcCaption.top &&
		pt.y < rcCaption.bottom )
	{
		CControlUI* pControl = m_PaintManager.FindControl(pt);
		if (pControl &&
			_tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI"))   != 0 )
		return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CWindowUI::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();

	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0))
	{
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE; //������ϵͳĬ�ϴ���
	return 0;
}

LRESULT CWindowUI::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {0};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;//������ϵͳ����
	return 0;
}

LRESULT CWindowUI::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowUI::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
	if (GET_SC_WPARAM(wParam) == SC_CLOSE)
	{
		PostMessage(WM_CLOSE);
		bHandled = TRUE;
		return 0L;
	}

	BOOL bZoomed = ::IsZoomed(*this);//�˿��Ƿ����

	LRESULT lResult = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	bHandled = TRUE;

	if (::IsZoomed(*this) != bZoomed)//�л���󻯺ͻָ���ť
	{
		CControlUI* pControl;
		if (!bZoomed)
		{
			pControl = m_PaintManager.FindControl(_T("sysbtn_max"));
			if (pControl)
				pControl->SetVisible(false);

			pControl = m_PaintManager.FindControl(_T("sysbtn_rst"));
			if (pControl)
				pControl->SetVisible(true);
		}
		else
		{
			pControl = m_PaintManager.FindControl(_T("sysbtn_max"));
			if (pControl)
				pControl->SetVisible(true);

			pControl = m_PaintManager.FindControl(_T("sysbtn_rst"));
			if (pControl)
				pControl->SetVisible(false);
		}
	}

	return lResult;
}


} // namespace DuiLib

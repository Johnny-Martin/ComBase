#include "StdAfx.h"
using namespace DuiLib;
#include "MainFrameWnd.h"
#include "Resource.h"


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////

CMainFrameWnd::CMainFrameWnd(void)
{
}

CMainFrameWnd::~CMainFrameWnd(void)
{
}

void CMainFrameWnd::Init()
{
}

void CMainFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("selectchanged"))
	{
		CStdString sName = msg.pSender->GetName();
	}
	else if( msg.sType == _T("click") ) 
	{
		CStdString sName = msg.pSender->GetName();

		if (sName == _T("sel"))
		{
			CWindowUI wnd;
			wnd.DoModal(GetHWND(), _T("selectwnd.xml"));
		}

		CWindowUI::Notify(msg);//标题栏消息
	}
	else if(msg.sType==_T("setfocus"))
	{
	}
}

LRESULT CMainFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	switch( uMsg ) {
	case WM_CREATE:				lRes = OnCreate(uMsg, wParam, lParam, bHandled);			break;
	case WM_TRAY_NOTIFICATION:	lRes = OnTrayIconNotify(wParam, lParam); bHandled = TRUE;	break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;

	return CWindowUI::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnCreate(uMsg, wParam, lParam, bHandled);

	SetIcon(IDI_DUILIBTEST);

	m_TryIcon.Init(GetHWND(), WM_TRAY_NOTIFICATION);
	m_TryIcon.SetIcon((HICON)::LoadImage(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDI_DUILIBTEST), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR), _T("Duilib测试程序"));

	Init();
	bHandled = TRUE;

	return 0;
}

LRESULT CMainFrameWnd::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uMouseMsg = (UINT)lParam; // mouse message that was sent.

	switch(uMouseMsg)
	{
	case WM_LBUTTONUP:
		if (::IsIconic(m_hWnd))
		{
			::ShowWindow(m_hWnd, SW_RESTORE);
		}
		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
		::SetForegroundWindow(m_hWnd);

		break;

	case WM_RBUTTONUP: // 弹出菜单
		{
// 			DuiLib::CPoint pos;
// 			::GetCursorPos(&pos);
// 
// 			HMENU hMainMenu = ::LoadMenu(m_PaintManager.GetInstance(), MAKEINTRESOURCE(IDC_MEDTOOLS));
// 			HMENU hTrayMenu = GetSubMenu(hMainMenu, 2);
// 			if (hTrayMenu)
// 			{
// 				::SetForegroundWindow(m_hWnd);
// 				::TrackPopupMenu(hTrayMenu, TPM_RIGHTBUTTON|TPM_RIGHTALIGN, pos.x, pos.y, 0, m_hWnd, NULL);
// 			}
// 
// 			::PostMessage(m_hWnd,WM_NULL, 0, 0);
		}
		break;
	}

	return 0;
}

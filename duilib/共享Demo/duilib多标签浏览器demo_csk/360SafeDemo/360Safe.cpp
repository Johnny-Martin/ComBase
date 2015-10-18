#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "ControlEx.h"

class C360SafeFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	C360SafeFrameWnd() { };
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init() {
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));


		CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_pm.FindControl(_T("ie")));
		if( pActiveXUI ) {
			IWebBrowser2* pWebBrowser = NULL;
			pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
			if( pWebBrowser != NULL ) {
				pWebBrowser->Navigate(L"http://www.baidu.com",NULL,NULL,NULL,NULL);  
				//pWebBrowser->Navigate(L"about:blank",NULL,NULL,NULL,NULL); 
				pWebBrowser->Release();
			}
		}

	}

	void OnPrepare() {
	}

	void Notify(TNotifyUI& msg)
	{


		if( msg.sType == _T("windowinit") ) OnPrepare();
		else if( msg.sType == _T("click") ) {
			if( msg.pSender == m_pCloseBtn ) {
				PostQuitMessage(0);
				return; 
			}
			else if( msg.pSender == m_pMinBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
			else if( msg.pSender == m_pMaxBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
			else if( msg.pSender == m_pRestoreBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
		}
		else if(msg.sType==_T("selectchanged"))
		{
			CStdString name = msg.pSender->GetName();
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
			if(name==_T("examine"))
				 pControl->SelectItem(0);
			else if(name==_T("trojan"))
				 pControl->SelectItem(1);
			else if(name==_T("plugins"))
				pControl->SelectItem(2);
			else if(name==_T("vulnerability"))
				pControl->SelectItem(3);
			else if(name==_T("rubbish"))
				pControl->SelectItem(4);
			else if(name==_T("cleanup"))
				pControl->SelectItem(5);
			else if(name==_T("fix"))
				pControl->SelectItem(6);
			else if(name==_T("tool"))
				pControl->SelectItem(7);
		}




		CStdString name=msg.pSender->GetName();
		if (msg.sType==_T("click"))
		{
			if (name==_T("NewItemBtn"))
			{
				//by csk

				//MessageBox(NULL,"kj","jj",MB_OK);

				//标签栏区域
				pContainer=static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("IContainer")));
				//OptionUI
				COptionUI* pNewOption=new COptionUI;
				pNewOption->ApplyAttributeList(_T(" float=\"false\" ")\
					_T("group=\"group1\" height=\"30\" maxwidth=\"174\" normalimage=\"file=\'tabitem.png\' source=\'0,0,174,32\'\" hotimage=\"file=\'tabitem.png\' source=\'174,0,348,32\'\" pushedimage=\"file=\'tabitem.png\' source=\'348,0,522,32\'\" selectedimage=\"file=\'tabitem.png\' source=\'348,0,522,32\'\" textalign=\"center\" "));
				//关闭按钮
				CButtonUI* pCloseBtn=new CButtonUI;
				pCloseBtn->ApplyAttributeList(_T("name=\"CloseTabBtn\" float=\"true\" normalimage=\"tab_close_Crop1.png\" hotimage=\"tab_close_Crop1.png\" pushedimage=\"tab_close_Crop3.png\" bordersize=\"0\" pos=\"-24,12,-12,24\" bordercolor=\"#FF000000\" width=\"12\" height=\"12\""));

				//标签HorLayout  ==  OptionUI + 关闭按钮
				CHorizontalLayoutUI* pNewHorLayout=new CHorizontalLayoutUI;
				pNewHorLayout->ApplyAttributeList(_T("maxwidth=\"174\" "));

				//在标签HorLayout上增加 OptionUI 和 关闭按钮
				pNewHorLayout->Add(pNewOption);
				pNewHorLayout->Add(pCloseBtn);

				//CButtonUI* pAddItemBtn=new CButtonUI;
				//	pAddItemBtn->ApplyAttributeList(_T("name=\"NewItemBtn\" float=\"false\" width=\"30\" normalimage=\"file=\'tab_new.png\' source=\'0,0,21,28\' dest=\'0,0,21,28\' \" hotimage=\"file=\'tab_new.png\' source=\'21,0,42,28\' dest=\'0,0,21,28\' \" pushedimage=\"file=\'tab_new.png\' source=\'42,0,63,28\'dest=\'0,0,21,28\' \" "));

				UINT nNewItemBtn=m_pm.FindControl(_T("NewItemBtn"))->GetTag();
				//UINT nNewItemBtn=m_pm.FindControl(_T("Item0"))->GetTag();


				//在标签栏区域增加 标签HorLayout（带关闭按钮的ｔａｂ布局）；


				//pContainer->AddAt(pNewHorLayout,nNewItemBtn);

				//pNewOption->Activate();
				//pNewHorLayout->Activate();
				//pNewOption->SetText(_T("p_"));
				//pNewOption->SetTextAlign(4);



				pContainer->AddAt(pNewHorLayout,nNewItemBtn);
				//  pContainer->Add(pNewHorLayout);


				//每增加一个tab并激活它
				//pNewOption->Activate();


				//by csk
			}
			if (name==_T("CloseTabBtn"))
			{
				//by csk

				//MessageBox(NULL,"kj","jj",MB_OK);
				//pContainer->
				UINT nDelItemHorLayout=m_pm.FindControl(_T("CloseTabBtn"))->GetParent()->GetTag();
				CControlUI* pDelCtl=static_cast<CControlUI*>(m_pm.FindControl(_T("CloseTabBtn"))->GetParent());
				//pContainer->Remove(pDelCtl);
				pContainer->RemoveAt(nDelItemHorLayout);
				//pDelCtl->SetVisible(false);

				//pContainer->NeedUpdate();
				//pContainer->
				//	by csk
			}
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		Init();
		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

// 		if( !::IsZoomed(*this) ) {
// 			RECT rcSizeBox = m_pm.GetSizeBox();
// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
// 				return HTTOP;
// 			}
// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
// 				return HTBOTTOM;
// 			}
// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
// 		}

		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-rcWork.left, -rcWork.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		default:
		bHandled = FALSE;
		}
		if( bHandled ) return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CHorizontalLayoutUI* pContainer;
	CButtonUI* pAddItemBtn;
	//...
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin/MulTab"));
	//CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	C360SafeFrameWnd* pFrame = new C360SafeFrameWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("360安全卫士"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
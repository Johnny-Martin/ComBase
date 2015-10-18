// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "WinImplBase.hpp"
#include "MainFrame.h" 
#include "SysConfig.h" 
#include "EmotionButton.h"


MainFrame::MainFrame()
{   
}


MainFrame::~MainFrame()
{ 
	PostQuitMessage(0);
}


LPCTSTR MainFrame::GetWindowClassName() const 
{ 
	return _T("QSBFaceMain"); 
}


tString MainFrame::GetSkinFile()
{
	return _T("QSBFace.xml");
}
 

UINT MainFrame::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; 
} 


DuiLib::tString MainFrame::GetSkinFolder()
{
	return SysConfig::GetSkinFolder();

}


UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return SysConfig::GetResourceType();
}


LPCTSTR MainFrame::GetResourceID() const
{
	return SysConfig::GetResourceID();
}

void MainFrame::OnFinalMessage(HWND hWnd) 
{  
	__super::OnFinalMessage(hWnd);
	delete this; 
}

void MainFrame::Init() 
{  
} 

 
    
void MainFrame::OnPrepare() 
{
        
 
}  
 
void MainFrame::Notify(TNotifyUI& msg)
{
		switch(msg.iType)
		{
			case NOTIFY_WINDOWINIT_VALUE:
				{
					OnPrepare();
				}
				break;
			case NOTIFY_CLICK_VALUE:
				{ 
					if( _tcsicmp(msg.pSender->GetName(),_T("closebtn")) == 0) 
					{
						PostQuitMessage(0);
						return; 
					}
					else if( _tcsicmp( msg.pSender->GetName() , _T("minbtn")) == 0 ) 
					{ 
						SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
						return; 
					}
					else if( _tcsicmp(msg.pSender->GetName() ,_T("maxbtn")) ==0 ) 
					{ 
						SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
					}
					else if( _tcsicmp(msg.pSender->GetName() , _T("restorebtn")) ==0 ) 
					{ 
						SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
					} 
					else if( _tcsicmp(msg.pSender->GetName() , _T("prev_page")) ==0 )  //上一页
					{
						CEmotionButton *pEmotionButton = static_cast<CEmotionButton*>(m_PaintManager.FindControl(_T("emotion_button")));
						if(pEmotionButton!=NULL && pEmotionButton->GetCurrentPage()> 0)
						{							
							pEmotionButton->SetPage(pEmotionButton->GetCurrentPage()-1); 
						}

					}
					else if( _tcsicmp(msg.pSender->GetName() , _T("next_page")) ==0 )  //下一页
					{
						CEmotionButton *pEmotionButton = static_cast<CEmotionButton*>(m_PaintManager.FindControl(_T("emotion_button")));

						if(pEmotionButton!=NULL && pEmotionButton->GetCurrentPage() < (pEmotionButton->GetPageCount()-1))
						{ 
							pEmotionButton->SetPage(pEmotionButton->GetCurrentPage()+1); 
						}
					} 
				}
				break;
		}
}
 

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        /*
		LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) { 
            default:
                bHandled = FALSE;
        }
        if( bHandled ) return lRes;
		*/

	    return __super::HandleMessage(uMsg, wParam, lParam); 
}
	 
LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if( wParam == SC_CLOSE ) 
		{
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }

        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if( ::IsZoomed(*this) != bZoomed ) {
            if( !bZoomed ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
                if( pControl ) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
                if( pControl ) pControl->SetVisible(true);
            }
            else {
                CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
                if( pControl ) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
                if( pControl ) pControl->SetVisible(false);
            }
        }
        return lRes;
}
 
LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}


CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("EmotionButton")) == 0 ) return new CEmotionButton;
	
	return NULL;
}
 
LONG MainFrame::GetStyle() const
{ 
	LONG styleValue = __super::GetStyle();
	  
	styleValue &= ~WS_MAXIMIZEBOX;
	styleValue &= ~WS_SIZEBOX; 
	styleValue &= ~WS_CAPTION;

	return styleValue|WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 
} 

LRESULT MainFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return HTCLIENT; //这个窗体标题栏
}
#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>
#include "resource.h"
#include "SysConfig.h"
#include "..\DuiLib\UIRichEdit.h"

#pragma  comment (lib,"oledlg.lib")

#include "WinImplBase.hpp"
#include "chat_dialog.hpp"
#include "ColorPicker.hpp"

 
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kBackgroundControlName = _T("bg");

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kDescriptionControlName = _T("description");

const TCHAR* const kFontButtonControlName = _T("fontbtn");
const TCHAR* const kFontbarControlName = _T("fontbar");
const TCHAR* const kFontTypeControlName = _T("font_type");
const TCHAR* const kFontSizeControlName = _T("font_size");
const TCHAR* const kBoldButtonControlName = _T("boldbtn");
const TCHAR* const kItalicButtonControlName = _T("italicbtn");
const TCHAR* const KUnderlineButtonControlName = _T("underlinebtn");
const TCHAR* const kColorButtonControlName = _T("colorbtn");

const TCHAR* const kInputRichEditControlName = _T("input_richedit");
const TCHAR* const kViewRichEditControlName = _T("view_richedit");

const TCHAR* const kEmotionButtonControlName = _T("emotionbtn");

const TCHAR* const kSendButtonControlName = _T("sendbtn");

const int kEmotionRefreshTimerId = 1001;
const int kEmotionRefreshInterval = 150;

ChatDialog::ChatDialog(const tString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info)
: bgimage_(bgimage) 
, bkcolor_(bkcolor)
, myselft_(myselft_info)
, friend_(friend_info)
, emotion_timer_start_(false)
, text_color_(0xFF000000)
, bold_(false)
, italic_(false)
, underline_(false)
, font_size_(12)
, font_face_name_(_T("微软雅黑"))
{ 
	
}

ChatDialog::~ChatDialog()
{
	return;
}

CControlUI* ChatDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void ChatDialog::OnFinalMessage(HWND hWnd)
{
	ReleaseAllGif();
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

BOOL ChatDialog::Receive(SkinChangedParam param)
{
	bgimage_ = param.bgimage;
	bkcolor_ = param.bkcolor;
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		if (!param.bgimage.empty())
		{
			TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
			_stprintf(szBuf, _T("file='%s' corner='600,200,1,1'"), param.bgimage.c_str());
#else
			_sntprintf(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), param.bgimage.c_str());
#endif
			background->SetBkImage(szBuf);
		}
		else
			background->SetBkImage(_T(""));

		background->SetBkColor(param.bkcolor);
	}

	return TRUE;
}

LPCTSTR ChatDialog::GetWindowClassName() const
{
	return _T("ChatDialog");
}

tString ChatDialog::GetSkinFile()
{
	return _T("chatbox.xml");
}

tString ChatDialog::GetSkinFolder()
{ 
	return SysConfig::GetSkinFolder();
}


UILIB_RESOURCETYPE ChatDialog::GetResourceType() const
{
	return SysConfig::GetResourceType();
}


LPCTSTR ChatDialog::GetResourceID() const
{
	return SysConfig::GetResourceID();
} 


UINT ChatDialog::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; 
} 

LRESULT ChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ChatDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ChatDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT ChatDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ChatDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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

void ChatDialog::OnTimer(TNotifyUI& msg)
{}

void ChatDialog::OnExit(TNotifyUI& msg)
{
	Close();
}

void ChatDialog::Init()
{
    
}

void ChatDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("file='%s' corner='600,200,1,1'"), bgimage_.c_str());
#else
		_sntprintf(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_.c_str());
#endif
		background->SetBkImage(szBuf);
		background->SetBkColor(bkcolor_);
	}

	CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
	if (log_button != NULL)
	{
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("%s"), friend_.logo.c_str());
#else
		_sntprintf(szBuf, MAX_PATH - 1, _T("%s"), friend_.logo.c_str());
#endif
		log_button->SetNormalImage(szBuf);
	}

	CControlUI* nick_name = m_PaintManager.FindControl(kNickNameControlName);
	if (nick_name != NULL)
		nick_name->SetText(friend_.nick_name.c_str());

	CControlUI* desciption = m_PaintManager.FindControl(kDescriptionControlName);
	if (desciption != NULL)
		desciption->SetText(friend_.description.c_str());

	CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());
}

tString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );
#if defined(UNDER_CE)
	_stprintf( szTime, _T("%04d年%02d月%02d日 %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#else
	_sntprintf( szTime, MAX_PATH, _T("%04d年%02d月%02d日 %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#endif

	return szTime;
}

void ChatDialog::SendMsg()
{
    CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
    if( pRichEdit == NULL ) return;
    pRichEdit->SetFocus();
    CStdString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
    if( sText.IsEmpty() ) return;
    pRichEdit->SetText(_T(""));

    pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kViewRichEditControlName));
    if( pRichEdit == NULL ) return;
    long lSelBegin = 0, lSelEnd = 0;
    CHARFORMAT2 cf;
    ZeroMemory(&cf, sizeof(CHARFORMAT2));
    cf.cbSize = sizeof(cf);
    cf.dwReserved = 0;
    cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE | CFM_UNDERLINETYPE;
    cf.dwEffects = CFE_LINK;
    cf.bUnderlineType = CFU_UNDERLINE;
    cf.crTextColor = RGB(220, 0, 0);

    lSelEnd = lSelBegin = pRichEdit->GetTextLength();
    pRichEdit->SetSel(lSelEnd, lSelEnd);
    pRichEdit->ReplaceSel(_T("某人"), false);

    lSelEnd = pRichEdit->GetTextLength();
    pRichEdit->SetSel(lSelBegin, lSelEnd);
    pRichEdit->SetSelectionCharFormat(cf);

    lSelEnd = lSelBegin = pRichEdit->GetTextLength();
    pRichEdit->SetSel(lSelEnd, lSelEnd);
    pRichEdit->ReplaceSel(_T("说:\t2011-07-30 08:30:12\n"), false);
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = RGB(0, 0, 0);
    cf.dwEffects = 0;
    lSelEnd = pRichEdit->GetTextLength();
    pRichEdit->SetSel(lSelBegin, lSelEnd);
    pRichEdit->SetSelectionCharFormat(cf);

    PARAFORMAT2 pf;
    ZeroMemory(&pf, sizeof(PARAFORMAT2));
    pf.cbSize = sizeof(pf);
    pf.dwMask = PFM_STARTINDENT;
    pf.dxStartIndent = 0;
    pRichEdit->SetParaFormat(pf);

    lSelEnd = lSelBegin = pRichEdit->GetTextLength();

    pRichEdit->SetSel(-1, -1);
    pRichEdit->ReplaceSel(sText.GetData(), false);

    pRichEdit->SetSel(-1, -1);
    pRichEdit->ReplaceSel(_T("\n"), false);

    cf.crTextColor = RGB(0, 0, 0);
    lSelEnd = pRichEdit->GetTextLength();
    pRichEdit->SetSel(lSelBegin, lSelEnd);
    pRichEdit->SetSelectionCharFormat(cf);

    ZeroMemory(&pf, sizeof(PARAFORMAT2));
    pf.cbSize = sizeof(pf);
    pf.dwMask = PFM_STARTINDENT;
    pf.dxStartIndent = 220;
    pRichEdit->SetParaFormat(pf);

    pRichEdit->EndDown(); 
 
	InsertGif(pRichEdit);

	//InsertObject(pRichEdit);
}

void ChatDialog::InsertGif(CRichEditUI* pRichEdit)
{ 
	_bstr_t bstrFileName1="c:\\Pic\\1.gif"; 


	InsertAniSmiley(pRichEdit,(long)GetHWND(), bstrFileName1, ::GetSysColor(COLOR_WINDOW), 0);

	_bstr_t bstrFileName2="c:\\Pic\\2.gif"; 


	InsertAniSmiley(pRichEdit,(long)GetHWND(), bstrFileName2, ::GetSysColor(COLOR_WINDOW), 0);

	_bstr_t bstrFileName3="c:\\Pic\\3.gif"; 


	InsertAniSmiley(pRichEdit,(long)GetHWND(), bstrFileName3, ::GetSysColor(COLOR_WINDOW), 0);

	_bstr_t bstrFileName4="c:\\Pic\\4.gif"; 


	InsertAniSmiley(pRichEdit,(long)GetHWND(), bstrFileName4, ::GetSysColor(COLOR_WINDOW), 0);
	
	//_bstr_t bstrFileName5="c:\\Pic\\5.gif"; 


	//InsertAniSmiley(pRichEdit,(long)GetHWND(), bstrFileName5, ::GetSysColor(COLOR_WINDOW), 0);
}

void ChatDialog::ReleaseAllGif()
{ 
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
	IRichEditOle *pRichEditOle = pRichEdit->GetRichEditOle();
	if (NULL==pRichEditOle)
	{
		return ;
	} 

	for(int i=0; i<pRichEditOle->GetObjectCount(); i++)
	{
		REOBJECT reobj={0};
		reobj.cbStruct=sizeof(REOBJECT);
		pRichEditOle->GetObject(i,&reobj,REO_GETOBJ_POLEOBJ);
		GifSmiley::IGifSmileyCtrl* lpAnimator;
		HRESULT hr = reobj.poleobj->QueryInterface(GifSmiley::IID_IGifSmileyCtrl, (void**)&lpAnimator);
		if(SUCCEEDED(hr))
		{
			lpAnimator->FreeImage();
			lpAnimator->Release();
		}
		reobj.poleobj->Release(); 
	}


}

bool ChatDialog::InsertAniSmiley(CRichEditUI* pRichEdit, long hwndHostWindow, BSTR bstrFileName, OLE_COLOR clr, INT cy)
{  
	GifSmiley::IGifSmileyCtrl* lpAnimator;
	::CoCreateInstance(GifSmiley::CLSID_CGifSmileyCtrl,NULL,CLSCTX_INPROC,GifSmiley::IID_IGifSmileyCtrl,(LPVOID*)&lpAnimator); 

	if (lpAnimator==NULL)
	{
		return false;
	}

	COLORREF backColor = (COLORREF)(clr);
	HWND hwnd = (HWND)(hwndHostWindow);
	HRESULT hr;
 

	IRichEditOle *pRichEditOle = pRichEdit->GetRichEditOle();
	if (NULL==pRichEditOle)
	{
		return false;
	} 


	/*
	ATL::CString strPicPath(bstrFileName);
	ATL::CString strExt=strPicPath.Right(4);
	if (strExt.CompareNoCase(_T(".gif")) && strExt.CompareNoCase(_T(".jpg")) && strExt.CompareNoCase(_T(".png")) && strExt.CompareNoCase(_T(".bmp"))) 
		return FALSE;

		*/

	LPSTORAGE lpStorage=NULL;
	LPOLEOBJECT	lpObject=NULL;
	LPLOCKBYTES lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite = NULL;   
	BSTR path = NULL;

	//Create lockbytes
	hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (FAILED(hr))
	{
		return	 false;
	}
	//use lockbytes to create storage
	SCODE sc = ::StgCreateDocfileOnILockBytes(lpLockBytes, STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		lpLockBytes->Release();
		return false;
	}

	// retrieve OLE interface for richedit   and  Get site
	pRichEditOle->GetClientSite(&lpClientSite);
	
	try
	{ 
		//COM operation need BSTR, so get a BSTR
		path = bstrFileName;

		//Load the gif
		lpAnimator->LoadFromFileSized(path, cy);

		//Set back color
		OLE_COLOR oleBackColor=(OLE_COLOR)backColor;
		lpAnimator->put_BackColor(oleBackColor); 


		//get the IOleObject
		hr = lpAnimator->QueryInterface(IID_IOleObject, (void**)&lpObject);
		if (FAILED(hr))
		{
			return	 false;
		}

		//Set it to be inserted
		OleSetContainedObject(lpObject, TRUE);

		//to insert into richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);	

		CLSID clsid;
		hr =  lpObject->GetUserClassID(&clsid);


		//set clsid
		reobject.clsid = clsid;
		//can be selected
		reobject.cp = REO_CP_SELECTION;
		//content, but not static
		reobject.dvaspect = DVASPECT_CONTENT;
		//goes in the same line of text line
		reobject.dwFlags = REO_BELOWBASELINE;
		//reobject.dwUser = (DWORD)myObject;
		
		//the very object
		reobject.poleobj = lpObject;
		//client site contain the object
		reobject.polesite = lpClientSite;
		//the storage 
		reobject.pstg = lpStorage;

		SIZEL sizel={0};
		reobject.sizel = sizel;


		LPOLECLIENTSITE lpObjectClientSite = NULL;   
		hr =lpObject->GetClientSite(&lpObjectClientSite);
		if(FAILED(hr) || lpObjectClientSite==NULL)
		  lpObject->SetClientSite(lpClientSite); 

		pRichEditOle->InsertObject(&reobject); 
		//redraw the window to show animation
		::RedrawWindow(hwnd,NULL, NULL, RDW_INVALIDATE);

		if (lpClientSite)
		{
			lpClientSite->Release();
			lpClientSite = NULL;
		}
		if (lpObject)
		{
			lpObject->Release();
			lpObject = NULL;
		}
		if (lpStorage)
		{
			lpStorage->Release();
			lpStorage = NULL;
		}

		if(lpAnimator)
		{
			 lpAnimator->Release();	
			 lpAnimator=NULL;
		}
  
	}
	catch( ... )
	{
		if (lpClientSite)
		{
			lpClientSite->Release();
			lpClientSite = NULL;
		}
		if (lpObject)
		{
			lpObject->Release();
			lpObject = NULL;
		}
		if (lpStorage)
		{
			lpStorage->Release();
			lpStorage = NULL;
		}
 
		if(lpAnimator)
		{
			lpAnimator->Release();	
			lpAnimator=NULL;
		}

		return false;
	}
	return true;
}


int ChatDialog::GetObjectPos(CRichEditUI* pRichEdit)
{
	bool findObject = false; 


	IRichEditOle *pRichEditOle = pRichEdit->GetRichEditOle();
	if (NULL==pRichEditOle)
	{
		return 0;
	}

	int nCount=pRichEditOle->GetObjectCount();
	for (int i=nCount-1; i>=0; i--)
	{
		REOBJECT reobj={0};
		reobj.cbStruct=sizeof(REOBJECT);
		pRichEditOle->GetObject(i,&reobj,REO_GETOBJ_POLEOBJ);
		reobj.poleobj->Release();
		
		/*
		if (reobj.clsid==__uuidof(GifSmiley::IGifSmileyCtrl))// && ((CGifSmileyCtrl*)reobj.dwUser)==this)
		{
			findObject = true;
			long left, bottom;
			HRESULT res;
			ITextDocument * iDoc=NULL;
			ITextRange *iRange=NULL;

			pRichEditOle->QueryInterface(IID_ITextDocument,(void**)&iDoc);
			if (!iDoc) break;

			iDoc->Range(reobj.cp, reobj.cp, &iRange);
			if (reobj.dwFlags&REO_BELOWBASELINE)
				res=iRange->GetPoint(TA_BOTTOM|TA_LEFT, &left, &bottom);
			else
				res=iRange->GetPoint(TA_BASELINE|TA_LEFT, &left, &bottom);

			iRange->Release();
			iDoc->Release();

			if (res!=S_OK) //object is out of screen let do normal fireview change
			{
				//UpdateSmileyPos(-100, -100, 1, 1);
				break;
			}

			DWORD nom=1, den=1;
			int zoom=MAKELONG(den,nom);
			//2012-03-10 wuding ,后续需要加入CRichEditUI来发消息 if (::SendMessage(hWnd, EM_GETZOOM, (WPARAM)&nom, (LPARAM)&den)) zoom=(den && nom) ? MAKELONG(den,nom) : zoom;

			RECT windowOffset={0};
			//2012-03-10 wuding ,后续需要加入CRichEditUI来发消息得到窗口大小 ::GetWindowRect(hWnd,&windowOffset);
			CHARRANGE chr={0};
			//2012-03-10 wuding ,后续需要加入CRichEditUI来发消息 ::SendMessage(hWnd,EM_EXGETSEL, 0, (LPARAM)&chr);
			DWORD flag=0;//(reobj.dwFlags & (REO_INVERTEDSELECT|REO_SELECTED));
			if  ( chr.cpMin!=chr.cpMax &&
				(  (reobj.cp<=chr.cpMax && reobj.cp>=chr.cpMin) ||
				(reobj.cp>=chr.cpMax && reobj.cp<=chr.cpMin)  )  )
			{
				flag|=REO_SELECTED;
				if((chr.cpMax-chr.cpMin)!= 1)
					flag|=REO_INVERTEDSELECT;
			}
			//UpdateSmileyPos(left-windowOffset.left, bottom-windowOffset.top, zoom, flag);

			break;            
		}
		*/
	}
	return 0;
}

void ChatDialog::GetObjectInfo(IRichEditOle *pIRichEditOle)
{
	long count =  pIRichEditOle->GetObjectCount();
	if(count)
	{
		REOBJECT reobj={0};
		reobj.cbStruct=sizeof(REOBJECT);
		pIRichEditOle->GetObject(0,&reobj,REO_GETOBJ_POLEOBJ);
		GifSmiley::IGifSmileyCtrl* lpAnimator;
		HRESULT hr = reobj.poleobj->QueryInterface(GifSmiley::IID_IGifSmileyCtrl, (void**)&lpAnimator);
		if(SUCCEEDED(hr))
		{
			_bstr_t fileName = lpAnimator->GetFileName(); 
		}
		reobj.poleobj->Release(); 
	}
}


HRESULT ChatDialog::GetNewStorage(LPSTORAGE* ppStg)
{ 
	
    if (!ppStg)
        return E_INVALIDARG;
	
    *ppStg = NULL;
	
    //
    // We need to create a new storage for an object to occupy.  We're going
    // to do this the easy way and just create a storage on an HGLOBAL and let
    // OLE do the management.  When it comes to saving things we'll just let
    // the RichEdit control do the work.  Keep in mind this is not efficient, 
    // but this program is just for demonstration.
    //
	
    LPLOCKBYTES pLockBytes;
    HRESULT hr = CreateILockBytesOnHGlobal(NULL, TRUE, &pLockBytes);
    if (FAILED(hr))
        return hr;
	
    hr = StgCreateDocfileOnILockBytes(pLockBytes, 
		STGM_SHARE_EXCLUSIVE | STGM_CREATE |
		STGM_READWRITE,
		0,
		ppStg);
    pLockBytes->Release();
    return (hr);
}



STDAPI_(void) OleUIMetafilePictIconFree(HGLOBAL hMetaPict)
{
    LPMETAFILEPICT pMF;
    
    if (NULL == hMetaPict)
        return;
    
    pMF = (LPMETAFILEPICT)GlobalLock(hMetaPict);
    
    if (NULL != pMF)
    {
        if (NULL != pMF->hMF)
            DeleteMetaFile(pMF->hMF);
    }
    
    GlobalUnlock(hMetaPict);
    GlobalFree(hMetaPict);
    return;
}


BOOL ChatDialog::InsertObject(CRichEditUI* pRichEdit)
{
	 
    return TRUE;
}


void ChatDialog::Notify(TNotifyUI& msg)
{
	switch(msg.iType)
	{
		case NOTIFY_WINDOWINIT_VALUE:
			{
				OnPrepare(msg);
			}
			break;
		case NOTIFY_KILLFOCUS_VALUE:
			{
			}
			break;
		case NOTIFY_CLICK_VALUE:
			{
				if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
				{
					OnExit(msg);
				}
				else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
				{
		#if defined(UNDER_CE)
					::ShowWindow(m_hWnd, SW_MINIMIZE);
		#else
					SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		#endif
				}
				else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
				{
		#if defined(UNDER_CE)
					::ShowWindow(m_hWnd, SW_MAXIMIZE);
					CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
					if( pControl ) pControl->SetVisible(false);
					pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
					if( pControl ) pControl->SetVisible(true);
		#else
					SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		#endif
				}
				else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
				{
		#if defined(UNDER_CE)
					::ShowWindow(m_hWnd, SW_RESTORE);
					CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
					if( pControl ) pControl->SetVisible(true);
					pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
					if( pControl ) pControl->SetVisible(false);
		#else
					SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		#endif
				}
				else if (_tcsicmp(msg.pSender->GetName(), kFontButtonControlName) == 0)
				{
					CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
					if (pFontbar != NULL)
						pFontbar->SetVisible(!pFontbar->IsVisible());
				}
				else if (_tcsicmp(msg.pSender->GetName(), kEmotionButtonControlName) == 0)
				{
					POINT pt = {0};
					CRect rcEmotionBtn = msg.pSender->GetPos();
					CRect rcWindow;
					GetWindowRect(m_hWnd, &rcWindow);

					pt.y = rcWindow.top + rcEmotionBtn.top;
					pt.x = rcWindow.left + rcEmotionBtn.left;
					//emotion_list_window_.SelectEmotion(pt);
				}
				else if (_tcsicmp(msg.pSender->GetName(), kSendButtonControlName) == 0)
				{
					SendMsg();
				}
			}
			break;
		case NOTIFY_RETURN_VALUE:
			{
				if (_tcsicmp(msg.pSender->GetName(), kInputRichEditControlName) == 0)
				{
					SendMsg();
				}
			}
			break;
		case NOTIFY_TIMER_VALUE:
			{
				OnTimer(msg);
			}
			break;
		case NOTIFY_SELECTCHANGED_VALUE:
			{
				if (_tcsicmp(msg.pSender->GetName(), kColorButtonControlName) == 0)
				{
					CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
					if (pFontbar != NULL)
					{
						POINT pt = {0};
						CRect rcFontbar = pFontbar->GetPos();
						CRect rcColorBtn = msg.pSender->GetPos();
						CRect rcWindow;
						GetWindowRect(m_hWnd, &rcWindow);

						pt.y = rcWindow.top + rcFontbar.top;
						pt.x = rcWindow.left + rcColorBtn.left + static_cast<LONG>(rcColorBtn.right - rcColorBtn.left / 2);
						new CColorPicker(this, pt);
					}
				}
				else if (_tcsicmp(msg.pSender->GetName(), kBoldButtonControlName) == 0)
				{
					COptionUI* bold_button = static_cast<COptionUI*>(msg.pSender);
					if (bold_button != NULL)
					{
						bold_ = bold_button->IsSelected();
						FontStyleChanged();
					}
				}
				else if (_tcsicmp(msg.pSender->GetName(), kItalicButtonControlName) == 0)
				{
					COptionUI* italic_button = static_cast<COptionUI*>(msg.pSender);
					if (italic_button != NULL)
					{
						italic_ = italic_button->IsSelected();
						FontStyleChanged();
					}
				}
				else if (_tcsicmp(msg.pSender->GetName(), KUnderlineButtonControlName) == 0)
				{
					COptionUI* underline_button = static_cast<COptionUI*>(msg.pSender);
					if (underline_button != NULL)
					{
						underline_ = underline_button->IsSelected();
						FontStyleChanged();
					}
				}
			}
			break;
		case NOTIFY_ITEMSELECT_VALUE:
			{
				if (_tcsicmp(msg.pSender->GetName(), kFontTypeControlName) == 0)
				{
					CComboUI* font_type = static_cast<CComboUI*>(msg.pSender);
					if (font_type != NULL)
					{
						font_face_name_ = font_type->GetText();
						FontStyleChanged();
					}
				}
				else if (_tcsicmp(msg.pSender->GetName(), kFontSizeControlName) == 0)
				{
					CComboUI* font_size = static_cast<CComboUI*>(msg.pSender);
					if (font_size != NULL)
					{
						font_size_ = _ttoi(font_size->GetText());
						FontStyleChanged();
					}
				}
			}
			break;

	}

}

void ChatDialog::SetTextColor(DWORD dwColor)
{
	COptionUI* color_button = static_cast<COptionUI*>(m_PaintManager.FindControl(kColorButtonControlName));
	if (color_button != NULL)
	{
		color_button->Selected(false);
		if (dwColor != 0)
		{
			text_color_ = dwColor;
			FontStyleChanged();
		}
	}
}

LRESULT ChatDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void ChatDialog::FontStyleChanged()
{}
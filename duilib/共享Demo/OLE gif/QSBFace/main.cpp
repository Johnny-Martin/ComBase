// QQFaceVC.cpp : 定义应用程序的入口点。
//


#include "stdafx.h"
#include "WinImplBase.hpp" 
#include "ximage.h"
#include "MainFrame.h"
#include "resource.h" 

#include <atlbase.h>
CComModule _Module;
#undef   SubclassWindow 
#include <atlwin.h>


#if defined(WIN32) && !defined(UNDER_CE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nCmdShow)
#endif
{  
	CPaintManagerUI::SetInstance(hInstance); 
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
 


	::CoInitialize(NULL);
	::OleInitialize(NULL);

	_Module.Init( 0, hInstance );

#if defined(WIN32) && !defined(UNDER_CE)
	HRESULT Hr = ::CoInitialize(NULL);
#else
	HRESULT Hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
	if( FAILED(Hr) ) return 0;

	MainFrame* pFrame = new MainFrame();
	if( pFrame == NULL ) return 0;
#if defined(WIN32) && !defined(UNDER_CE)
	pFrame->Create(NULL, _T("QQ2011"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
#else
	pFrame->Create(NULL, _T("QQ2011"), UI_WNDSTYLE_FRAME, WS_EX_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#endif
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();

	_Module.Term();

	WindowImplBase::Cleanup();

	::OleUninitialize();
	::CoUninitialize(); 

	return 0;
} 

 
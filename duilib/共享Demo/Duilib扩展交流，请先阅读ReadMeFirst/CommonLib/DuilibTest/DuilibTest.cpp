// DuilibTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DuilibTest.h"
#include "MainFrameWnd.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (GlobalSet::ForegroundPrevInstance())
		return FALSE;

	if(FAILED(::OleInitialize(NULL)))
		return FALSE;

	if (!GlobalSet::InitGlobal())
		return FALSE;


	{
		DuiLib::CPaintManagerUI::SetInstance(hInstance);
		DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath() + _T("skins\\skin1"));
		//CPaintManagerUI::SetResourceZip(CPaintManagerUI::GetInstancePath() + _T("skin\\skin1.zip"));

		CMainFrameWnd mainFrame;
		mainFrame.CreateX(NULL, UI_WNDSTYLE_FRAME, _T("mainframe.xml"));
		mainFrame.CenterWindow();
		::ShowWindow(mainFrame, SW_SHOW);

		DuiLib::CPaintManagerUI::MessageLoop();
	}


	GlobalSet::UninitGlobal();
	::OleUninitialize();

	return 0;
}


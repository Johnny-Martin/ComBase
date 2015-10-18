#include "StdAfx.h"
#include "GlobalSet.h"
#include <GdiPlus.h>
#pragma comment(lib, "Gdiplus.lib")

namespace GlobalSet
{
	DWORD    g_dwGdiplusToken;
	HANDLE   g_hMutex;
	//itstring g_sRootPath;

	//已经存在Mutex 并尝试启动实例返回
	bool ForegroundPrevInstance()
	{
		g_hMutex = CreateMutex(NULL, FALSE, MUTEX_MAINMUTEX);

		if (g_hMutex == 0 || GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// find the existed instance mainframe and show it
			HWND hPreWnd = ::FindWindow(CLASS_MAINFRAME, NULL);
			if (hPreWnd)
			{
				// BringWindowToTop() is no use here!
				if (::IsIconic(hPreWnd))
				{
					::ShowWindow(hPreWnd, SW_RESTORE);
				}

				::ShowWindow(hPreWnd, SW_SHOWNORMAL);
				::SetForegroundWindow(hPreWnd);
			}

			if (g_hMutex)
			{
				CloseHandle(g_hMutex);
				g_hMutex = NULL;
			}

			return true;
		}

		return false;
	}

	bool InitGlobal()
	{
		Gdiplus::GdiplusStartupInput gdiplusinput;
		Gdiplus::GdiplusStartup(&g_dwGdiplusToken, &gdiplusinput, NULL);

		//g_sRootPath = GetApplicationPath();

		return true;
	}

	void UninitGlobal()
	{
		if (g_dwGdiplusToken != 0)
		{
			Gdiplus::GdiplusShutdown(g_dwGdiplusToken);
			g_dwGdiplusToken = 0;
		}

		if (g_hMutex)
		{
			CloseHandle(g_hMutex);
			g_hMutex = NULL;
		}
	}


};



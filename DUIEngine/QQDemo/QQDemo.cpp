// magicbox2.cpp : main source file for magicbox2.exe
//

#include "stdafx.h"
#include "DuiSystem.h" 
#include "DuiDefaultLogger.h"
#include "menuwndhook.h"

#include "LoginDlg.h"
#include "duibuttonex.h"
#include "Dui3DView.h"
#include "duiembededit.h"
#include "duiembedbutton.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = CoInitialize(NULL);
	DUIASSERT(SUCCEEDED(hRes));
 
	TCHAR szCurrentDir[MAX_PATH+1]; memset( szCurrentDir, 0, sizeof(szCurrentDir) );
	GetModuleFileName( NULL, szCurrentDir, MAX_PATH );
	LPTSTR lpInsertPos = _tcsrchr( szCurrentDir, _T('\\') );
	*lpInsertPos = _T('\0');   

	DuiSystem duiSystem(hInstance);
	DefaultLogger loger;
	loger.setLogFilename(CDuiStringT(szCurrentDir)+_T("\\duiengine.log"));
	duiSystem.SetLogger(&loger);

  CMenuWndHook::InstallHook(hInstance,"skin_menuborder");

	duiSystem.logEvent(_T("demo started"));

	#ifdef __DUIFILE_RC 
	//从文件夹加载皮肤,指定皮肤位置
	_tcscat( szCurrentDir, _T("\\..\\skin") );
	DuiResProviderFiles *pResFiles=new DuiResProviderFiles;
	if(!pResFiles->Init(szCurrentDir))
	{
		DUIASSERT(0);
		return 1;
	}
	duiSystem.SetResProvider(pResFiles);
#else 
	//从资源加载皮肤
	duiSystem.SetResProvider(new DuiResProviderPE(hInstance));
#endif 
	BOOL bOK=duiSystem.Init(_T("IDR_DUI_INIT")); //初始化DUI系统,原来的系统初始化方式依然可以使用。
	duiSystem.SetMsgBoxTemplate(_T("IDR_DUI_MSGBOX"));

  DuiWindowFactoryManager::getSingleton().RegisterFactory(TplDuiWindowFactory<CDuiButtonEx>());
  DuiWindowFactoryManager::getSingleton().RegisterFactory(TplDuiWindowFactory<CDui3DView>());
  DuiWindowFactoryManager::getSingleton().RegisterFactory(TplDuiWindowFactory<CEmbedEdit>());
  DuiWindowFactoryManager::getSingleton().RegisterFactory(TplDuiWindowFactory<CEmbedButton>());
	int nRet = 0; 
	// BLOCK: Run application
	{
		CLoginDlg dlgLogin;  
		nRet = dlgLogin.DoModal();  
	}

	duiSystem.logEvent(_T("demo end"));

	delete duiSystem.GetResProvider();

	CoUninitialize();
	return nRet;
}

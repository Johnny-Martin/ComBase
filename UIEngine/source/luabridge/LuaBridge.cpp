// luabridge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LuaBridge.h"
#include <AccCtrl.h>
#include <Aclapi.h>
#pragma comment(lib, "Advapi32.lib")

#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

#include <lm.h>
#pragma comment(lib, "Netapi32.lib")
void example_1()
{
	LuaBridge luaBridge;
	if (LUA_ENV_SUCCESS == luaBridge.InitLuaEnv())
	{
		LUA_ENV_ERROR ret = luaBridge.LoadLuaFile("I:\\UIEngine\\example\\luacode\\test.lua");
		if (LUA_ENV_SUCCESS == ret)
		{
			luaBridge.InvokeLuaFunction("OnLoadLuaFile", 31, 32);
		}
	}
}

void test(LPTSTR lpszServer, LPTSTR lpszShare)
{
	PSHARE_INFO_502 BufPtr;
	NET_API_STATUS res;

	res = NetShareGetInfo (lpszServer,lpszShare,502,(LPBYTE *) &BufPtr);
	if(res == ERROR_SUCCESS)
	{
		printf("%S\t%S\t%u\n",BufPtr->shi502_netname, BufPtr->shi502_path, BufPtr->shi502_current_uses);
		//
		// Validate the value of the 
		//  shi502_security_descriptor member.
		//
		if (IsValidSecurityDescriptor(BufPtr->shi502_security_descriptor))
			printf("It has a valid Security Descriptor.\n");
		else
			printf("It does not have a valid Security Descriptor.\n");
		//
		// Free the allocated memory.
		//
		NetApiBufferFree(BufPtr);
	}
	else 
		printf("Error: %ld\n",res);
}


int GetSid(LPWSTR Username, PSID &pSid, DWORD &cbSid)
{
	WCHAR RefDomain[DNLEN + 1];
	DWORD cchDomain = DNLEN + 1;
	SID_NAME_USE peUse;
	//
	// initial allocation attempt for Sid
	//
#define SID_SIZE 96
	cbSid = SID_SIZE;

	pSid = (PSID)HeapAlloc(GetProcessHeap(), 0, cbSid);
	if(pSid == NULL) {
		printf("HeapAlloc error!\n");
		return 1;
	}

	//
	// get the Sid associated with the supplied user/group name
	// force Unicode API since we always pass Unicode string
	//

	if(!LookupAccountNameW(
		NULL,       // default lookup logic
		Username,   // user/group of interest from commandline
		pSid,       // Sid buffer
		&cbSid,     // size of Sid
		RefDomain,  // Domain account found on (unused)
		&cchDomain, // size of domain in chars
		&peUse
		)) {

			//
			// if the buffer wasn't large enough, try again
			//

			if(GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

				pSid = (PSID)HeapReAlloc(GetProcessHeap(), 0, pSid, cbSid);

				if(pSid == NULL) {
					printf("HeapReAlloc error!\n");
					goto cleanup;
				}

				cchDomain = DNLEN + 1;

				if(!LookupAccountNameW(
					NULL,       // default lookup logic
					Username,   // user/group of interest from commandline
					pSid,       // Sid buffer
					&cbSid,     // size of Sid
					RefDomain,  // Domain account found on (unused)
					&cchDomain, // size of domain in chars
					&peUse
					)) {
						printf("LookupAccountName error! (rc=%lu)\n",
							GetLastError());
						goto cleanup;
				}

			} else {
				printf("LookupAccountName error! (rc=%lu)\n", GetLastError());
				goto cleanup;
			}
	}

	return 1;
cleanup:

	//
	// free allocated resources
	//
	if(pSid != NULL)
		HeapFree(GetProcessHeap(), 0, pSid);

	return 0;
}
int NetShareAddDacl(LPTSTR ShareName, LPTSTR TrusteeName, DWORD AccessMask, bool bKeepExcistingDacl )
{
	DWORD InheritFlag = NO_INHERITANCE; // does nothing with shares! so set it like this
	ACCESS_MODE option = SET_ACCESS; // will change access
	EXPLICIT_ACCESS explicitaccess;

	PACL ExistingDacl;
	PACL NewAcl = NULL;
	PSECURITY_DESCRIPTOR psd = NULL;

	DWORD dwError;

	// get current Dacl on specified file
	dwError = GetNamedSecurityInfo(	 ShareName, // Not the PATH, but the NAME of the share!
		SE_LMSHARE, // specifies network Share object
		DACL_SECURITY_INFORMATION, //GROUP_SECURITY_INFORMATION
		NULL,
		NULL,
		&ExistingDacl,
		NULL,
		&psd
		);
	if(dwError != ERROR_SUCCESS) {
		goto cleanup;
	}

	BuildExplicitAccessWithName(
		&explicitaccess,
		TrusteeName,
		AccessMask,
		option,
		InheritFlag
		);

	// add specified access to the object
	//
	// USE NULL instead of ExistingDacl to delete the excisting ACL 
	// and set only the new one from BuildExplicitAccessWithName
	// The result - only the new user is in the list of the Share
	// permissions
	if (bKeepExcistingDacl)
		dwError = SetEntriesInAcl(
		1,
		&explicitaccess,
		ExistingDacl, // will add the old Dacl to the new one!
		&NewAcl
		);
	else 
		dwError = SetEntriesInAcl(
		1,
		&explicitaccess,
		NULL, // do not add the old one to the new acl! 
		&NewAcl
		);

	if(dwError != ERROR_SUCCESS) {
		goto cleanup;
	}


	PSID pSid = NULL;
	DWORD cbSid = 0;
	GetSid(_T("Everyone"), pSid, cbSid);
	// apply new security to file
	dwError = SetNamedSecurityInfo(
		ShareName, // ShareName is not the PATH of the share!
		SE_LMSHARE, // object type for network shares
		GROUP_SECURITY_INFORMATION, //DACL_SECURITY_INFORMATION | 
		pSid,
		pSid, //S-1-1-0
		NewAcl,
		NULL
		);

cleanup:

	if( NewAcl != NULL ) AccFree( NewAcl );
	if( psd != NULL) AccFree( psd );
	return dwError;
}

void ShareXmpMedia(const TCHAR* filePath)
{
	NET_API_STATUS res;
	SHARE_INFO_2 p;
	DWORD parm_err = 0;

	// Fill in the SHARE_INFO_2 structure.
	p.shi2_netname = TEXT("share3");    
	p.shi2_type = STYPE_DISKTREE; // disk drive
	p.shi2_remark = TEXT("Ñ¸À×Ó°Òô¹²Ïí¿â");
	p.shi2_permissions = ACCESS_ALL;    
	p.shi2_max_uses = 4;
	p.shi2_current_uses = 0;    
	p.shi2_path = TEXT("E:\\XMPLib3");
	p.shi2_passwd = NULL; // no password
	//
	// Call the NetShareAdd function,
	//  specifying level 2.
	//
	res=NetShareAdd(NULL, 2, (LPBYTE) &p, &parm_err);
	//
	// If the call succeeds, inform the user.
	//
	if(res==0)
		printf("Share created.\n");

	// Otherwise, print an error,
	//  and identify the parameter in error.
	//
	else
		printf("Error: %u\tparmerr=%u\n", res, parm_err);
}
int _tmain(int argc, _TCHAR* argv[])
{
	example_1();
	//ShareXmpMedia(NULL);
	//test(NULL, _T("share2"));
	//NetShareAddDacl(TEXT("share3"), TEXT("Everyone"), 2032127, true);
	//NetShareAddDacl(TEXT("share"), TEXT("Administrators"), 2032127, true);
	return 0;
}


lua_State* L;  
int CCallLua()
{  
	/*Initialize Lua*/  
	L = luaL_newstate();  
	/*Load Lua Library*/  
	luaL_openlibs(L);  
	/*Load lua file*/  
	int loadRet = luaL_dofile(L, "test.lua");  
	//int iError = lua_pcall(L, 0, 0, 0);  
	if (loadRet)  
	{  
		lua_close(L);  
		return 1;  
	}  
	/*Invoke lua function*/  
	int a = 11 ;  
	int b = 12 ;  
	lua_getglobal(L,"LuaFun4Cpp");               
	lua_pushinteger(L,a) ;  
	lua_pushinteger(L,b) ;  
	int ret = lua_pcall(L,2,1,0) ;  
	if ( ret != 0 )  
		return 0;  
	printf("sum:%d + %d = %ld\n",a,b,lua_tointeger(L,-1)) ;  
	lua_pop(L,1);  
	/*Release Lua */  
	lua_close(L);  
	return 0;  
}  
int CFun4Lua(lua_State* luaState)  
{  
	lua_Integer a = lua_tointeger(luaState, 1);  
	lua_Integer b = lua_tointeger(luaState, 2);  
	lua_pushinteger(luaState, a+b); 

	return 1;  
}  
//static bool  BSTRToLuaString( BSTR src, std::string& dest)
//{
//	if(!src)
//		return false;
//	int  iLen = (int)wcslen(src);
//	if(iLen > 0)
//	{
//		char* szdest = new  char[iLen * 4];
//		if(NULL == szdest)
//			return false;
//		ZeroMemory(szdest, iLen * 4);			
//		int nLen = WideCharToMultiByte(CP_UTF8, NULL, src, iLen, szdest, iLen * 4, 0, 0);
//		szdest[nLen] = '\0'; 
//		dest = szdest;
//		delete [] szdest ;
//		return true;
//	}
//	return false;
//}
static bool LuaStringToWideChar(const char* szSrc, wchar_t* &wszDst)
{
	wszDst = NULL;
	if(!szSrc)
		return false;

	int iLen = (int)strlen(szSrc);
	if(iLen <= 0)
		return false;

	wszDst = new wchar_t[iLen * 4];
	ZeroMemory(wszDst, iLen * 4);
	int nLen = MultiByteToWideChar(CP_UTF8, 0, szSrc, iLen, wszDst, iLen*4); 
	wszDst[nLen] = '\0';

	return true;
}
//ANSI to UTF8
static bool ANSIToUTF8(const char* pszSrc, char* &pszDst)
{
	return true;
}
static bool WideCharToLuaString(const wchar_t* wszSrc, char* &szDst)
{
	szDst = NULL;
	if(!wszSrc)
		return false;
	int  iLen = (int)wcslen(wszSrc);
	if(iLen > 0)
	{
		szDst = new char[iLen * 4];
		if(NULL == szDst)
			return false;
		ZeroMemory(szDst, iLen * 4);			
		int nLen = WideCharToMultiByte(CP_UTF8, NULL, wszSrc, iLen, szDst, iLen * 4, 0, 0);
		szDst[nLen] = '\0'; 
		return true;
	}
	return false;
}
static bool LuaStringToCComBSTR(const char* src, CComBSTR& bstr)
{
	bstr = L"";
	if(!src)
		return false;
	int iLen = (int)strlen(src);
	if(iLen > 0)
	{
		wchar_t* szm = new wchar_t[iLen * 4];
		ZeroMemory(szm, iLen * 4);
		int nLen = MultiByteToWideChar(CP_UTF8, 0, src,iLen, szm, iLen*4); 
		szm [nLen] = '\0';
		bstr = szm;
		delete [] szm;
		return true;
	}
	return false;
}
int MsgBox(lua_State* luaState)
{
	const char* luaStr = lua_tostring(luaState, 1);
	wchar_t* wszInfo = NULL;

	bool ret = LuaStringToWideChar(luaStr, wszInfo);
	if (ret)
		MessageBox(NULL, wszInfo, L"MsgBox", MB_OK);
	else
		MessageBox(NULL, L"", L"MsgBox", MB_OK);
	
	delete wszInfo;
	return 0;
}

/**/
int GetHttpContentCacheFile(lua_State* pLuaState)
{
	::CoInitialize(NULL);

	const char *pszUrlUTF8 = lua_tostring(pLuaState, 1);
	TCHAR* wstrUrl;
	LuaStringToWideChar(pszUrlUTF8,wstrUrl);
	TCHAR wszSavePath[MAX_PATH] = {0};
	HRESULT hr = ::URLDownloadToCacheFile(NULL, wstrUrl, wszSavePath, sizeof(wszSavePath), 0, NULL);
	
	if (SUCCEEDED(hr))
	{
		char* szSavePath = NULL;
		if (WideCharToLuaString(wszSavePath, szSavePath))
		{
			lua_pushstring(pLuaState, szSavePath);
			::CoUninitialize();
			return 1;
		}
	}

	::CoUninitialize();
	return 0;
}

long ShellExecHelper(HWND hWnd, const char* lpOperation, const char* lpFile, const char* lpParameters, const char* lpDirectory, const char* lpShowCmd, int iShowCmd)
{
	CComBSTR bstrOperation;
	CComBSTR bstrFile;
	CComBSTR bstrParameters;
	CComBSTR bstrDir;
	CComBSTR bstrCmd;

	if ( lpOperation )
	{
		LuaStringToCComBSTR(lpOperation,bstrOperation);
	}
	if ( lpFile )
	{
		LuaStringToCComBSTR(lpFile,bstrFile);
	}
	if ( lpParameters )
	{
		LuaStringToCComBSTR(lpParameters,bstrParameters);
	}
	if ( lpDirectory )
	{
		LuaStringToCComBSTR(lpDirectory,bstrDir);
	}
	if ( lpShowCmd )
	{
		LuaStringToCComBSTR(lpShowCmd,bstrCmd);
	}
	std::wstring strCmd = bstrCmd.m_str;
	INT nShowCmd = 0;
	if (iShowCmd != -1)
	{
		nShowCmd = iShowCmd;
	}
	else if ( strCmd == L"SW_HIDE" )
	{
		nShowCmd = SW_HIDE;
	}
	else if ( strCmd == L"SW_MAXIMIZE" )
	{
		nShowCmd = SW_MAXIMIZE;
	}
	else if ( strCmd == L"SW_MINIMIZE" )
	{
		nShowCmd = SW_MINIMIZE;
	}
	else if ( strCmd == L"SW_RESTORE" )
	{
		nShowCmd = SW_RESTORE;
	}
	else if ( strCmd == L"SW_SHOW" )
	{
		nShowCmd = SW_SHOW;
	}
	else if ( strCmd == L"SW_SHOWDEFAULT" )
	{
		nShowCmd = SW_SHOWDEFAULT;
	}
	else if ( strCmd == L"SW_SHOWMAXIMIZED" )
	{
		nShowCmd = SW_SHOWMAXIMIZED;
	}
	else if ( strCmd == L"SW_SHOWMINIMIZED" )
	{
		nShowCmd = SW_SHOWMINIMIZED;
	}
	else if ( strCmd == L"SW_SHOWMINNOACTIVE" )
	{
		nShowCmd = SW_SHOWMINNOACTIVE;
	}
	else if ( strCmd == L"SW_SHOWNA" )
	{
		nShowCmd = SW_SHOWNA;
	}
	else if ( strCmd == L"SW_SHOWNOACTIVATE" )
	{
		nShowCmd = SW_SHOWNOACTIVATE;
	}
	else if ( strCmd == L"SW_SHOWNORMAL" )
	{
		nShowCmd = SW_SHOWNORMAL;
	}

	if ( (HINSTANCE) 32 < ::ShellExecute( hWnd, bstrOperation.m_str, bstrFile.m_str, bstrParameters.m_str, bstrDir.m_str, nShowCmd ) )
	{
		return 0;
	}
	return 1;
}


int LuaShellExecute(lua_State* pLuaState)
{
	
	HWND hWnd = (HWND)lua_touserdata(pLuaState, 1);
	const char* lpOperation = luaL_checkstring(pLuaState, 2);
	const char* lpFile = luaL_checkstring(pLuaState, 3);
	const char* lpParameters = luaL_checkstring(pLuaState, 4);
	const char* lpDir = luaL_checkstring(pLuaState, 5);
	int luaType = lua_type(pLuaState, 6);
	if (luaType == LUA_TSTRING)
	{
		const char* lpCmd = luaL_checkstring(pLuaState, 6);
		lua_pushinteger(pLuaState, ShellExecHelper(hWnd, lpOperation, lpFile, lpParameters, lpDir, lpCmd, -1));
	}
	else if (luaType == LUA_TNUMBER)
	{
		int iCmd = (int)luaL_checkinteger(pLuaState, 6);
		lua_pushinteger(pLuaState, ShellExecHelper(hWnd, lpOperation, lpFile, lpParameters, lpDir, NULL, iCmd));
	}

	return 1;
}

int ANSIToLuaString(lua_State* pLuaState)
{
	const char * pszSrc = luaL_checkstring(pLuaState, 1);
	if (!pszSrc) 
		return 0;

	int iLen = (int)strlen(pszSrc);
	if(iLen <= 0)
		return 0;

	wchar_t* wszDst = new wchar_t[iLen * 4];
	ZeroMemory(wszDst, iLen * 4);

	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, iLen, wszDst, iLen*4); 
	wszDst[nLen] = '\0';

	char* pszLuaStr = NULL;
	bool ret = WideCharToLuaString(wszDst, pszLuaStr);
	if (!ret)
	{
		delete[] wszDst;
		return 0;
	}

	lua_pushstring(pLuaState, pszLuaStr);
	return 1;
}
//#define err_exit(num,fmt,args)  \  
//do{printf("[%s:%d]",fmt,"\n",__FILE__,__LINE__,##args);exit(num);} while(0)
//
//#define err_return(num,fmt,args)  \  
//do{printf("[%s:%d]",fmt,"\n",__FILE__,__LINE__,##args);return(num);} while(0)  

int LuaCallC()  
{  
	lua_State * l = luaL_newstate() ; 
	if ( l == NULL )// err_return(-1,"luaL_newstat() failed");   
		return 1 ;  
	int ret = 0 ;  
	ret = luaL_loadfile(l,"test.lua") ;  
	if ( ret != 0 ) //err_return(-1,"luaL_loadfile failed") ;  
		return 1 ;
	ret = lua_pcall(l,0,0,0) ;  
	if ( ret != 0 )// err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;  
		return 1 ;

	//luaL_newlib();
	//Register the cpp function into lua
	lua_pushcfunction(l,CFun4Lua); 
	//Bind the global name to the function
	lua_setglobal(l,"CFun4Lua");

	//Now we can invoke function "CFun4Lua" in lua file
	int a = 15 ;  
	int b = 12 ;
	lua_getglobal(l,"LuaFun4Cpp"); 
	lua_pushinteger(l,a) ;  
	lua_pushinteger(l,b) ;  
	ret = lua_pcall(l,2,1,0) ;  

	if ( ret != 0 ) //err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ; 
		return 1 ;
	printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;  
	lua_pop(l,1) ;  

	lua_close(l) ;
	return 0 ;  

}  

int LuaBridge::m_initCFunctionArraySize = 0;

void LuaBridge::InitLuaGlobalFunctions(lua_State* pLuaStat)
{
	const _LuaCFunctionInfo* pArray = LuaBridge::_GetCFunctionArray();
	int iArrayLen = m_initCFunctionArraySize / sizeof(_LuaCFunctionInfo);
	for (int i=0; i<iArrayLen; ++i)
	{
		//Register the cpp function into lua
		lua_pushcfunction(m_luaState, pArray[i].func); 
		//Bind the global name to the function
		lua_setglobal(m_luaState, pArray[i].funcName);
	}
}

LuaBridge::LuaBridge():m_luaState(NULL)
{	

}
LUA_ENV_ERROR LuaBridge::InitLuaEnv()
{
	m_luaState = luaL_newstate(); 
	if ( m_luaState == NULL )
		return LUA_ENV_INITFAILED; 

	luaL_openlibs(m_luaState);
	InitLuaGlobalFunctions(m_luaState);

	return LUA_ENV_SUCCESS;
}
LUA_ENV_ERROR LuaBridge::LoadLuaFile(const char* pszFilePath)
{
	if ( m_luaState == NULL )
		return LUA_ENV_INITFAILED;

	int ret = 0;  
	ret = luaL_loadfile(m_luaState, pszFilePath);  
	if ( ret != 0 )   
		return LUA_ENV_LOADFILEFAILED;

	ret = lua_pcall(m_luaState, 0, 0, 0);  
	if ( ret != 0 )
		return LUA_ENV_LOADFILEFAILED;

	return LUA_ENV_SUCCESS;
}
LUA_ENV_ERROR LuaBridge::InvokeLuaFunction(const char* funcName, int a, int b)
{
	lua_getglobal(m_luaState, funcName); 
	lua_pushinteger(m_luaState, a);  
	lua_pushinteger(m_luaState, b);  
	int ret = lua_pcall(m_luaState, 2, 1, 0);  
	if ( ret != 0 )  
		return LUA_ENV_INVOKEFAILED;

	int result = lua_tointeger(m_luaState,-1);
	printf("sum:%d + %d + 230 = %ld\n", a, b, result);  
	lua_pop(m_luaState,1);  

	return LUA_ENV_SUCCESS;
}
LUA_ENV_ERROR LuaBridge::UnInitLuaEnv()
{
	lua_close(m_luaState);
	return LUA_ENV_SUCCESS;
}

LuaBridge::~LuaBridge()
{
	UnInitLuaEnv();
}

int SetGlobalObject(lua_State* pLuaState)
{
	lua_Integer a = lua_tointeger(pLuaState,1);  
	lua_Integer b = lua_tointeger(pLuaState,2);  

	lua_pushinteger(pLuaState,a+b);
	return 1;
}
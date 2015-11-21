/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: Main.cpp

	Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.10.4

    Description: D2D程序必须的一些定义.为了避免名字冲突,用户在自己的文件中
	             可自己添加using namespace D2D1.此处不统一添加.

\*=========================================================================*/

#pragma once


#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")



template<class Type>
inline void SafeRelease(Type& pObjToRelease)
{
	if(pObjToRelease)
	{
		pObjToRelease->Release();
		pObjToRelease = 0;
	}
}


template<class Type>
inline void SafeDelete(Type& pObjToDelete)
{
	if(pObjToDelete)
	{
		delete pObjToDelete;
		pObjToDelete = 0;
	}
}


template<class Type>
inline void SafeDeleteArray(Type& pArrayToDelete)
{
	if(pArrayToDelete)
	{
		delete [] pArrayToDelete;
		pArrayToDelete = 0;
	}
}


#define WARNING_MSG(WarningStr) \
	MessageBox(WarningStr, _T("Error"), MB_OK | MB_ICONWARNING);


#define WARNING_HWND_MSG(hWnd, WarningStr) \
	::MessageBox(hWnd, WarningStr, _T("Error"), MB_OK | MB_ICONWARNING);


#ifndef Assert
#if defined(DEBUG) || defined(_DEBUG)
#if defined(UNICODE)
#define Assert(b) do { if(!(b)) {OutputDebugStringW(L("Assert: ") #b L("\n"));} } while(0);
#else
#define Assert(b) do { if(!(b)) {OutputDebugStringA("Assert: " #b "\n");} } while(0);
#endif //UNICODE
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif //Assert


#ifndef _T
#define _T(b) L##b
#endif //_T

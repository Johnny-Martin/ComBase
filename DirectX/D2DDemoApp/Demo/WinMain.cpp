/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: Main.cpp

	Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.10.4

    Description: Win32 Application 程序入口函数WinMain定义处.

\*=========================================================================*/

#include "DemoApp.h"


int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPreInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
	)
{
	//
	// 初始化 COM 接口并初始化窗口.
	//
	if(SUCCEEDED(CoInitialize(NULL)))
	{
		CD2DDemoApp Demo;
		if(Demo.Initialize(hInstance))
		{
			Demo.RunMessageLoop();
		}

		//
		// 销毁 COM 接口资源.
		//
		CoUninitialize();
	}

	return 0;
}

/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: Main.cpp

	Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.10.4

    Description: Win32 Application ������ں���WinMain���崦.

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
	// ��ʼ�� COM �ӿڲ���ʼ������.
	//
	if(SUCCEEDED(CoInitialize(NULL)))
	{
		CD2DDemoApp Demo;
		if(Demo.Initialize(hInstance))
		{
			Demo.RunMessageLoop();
		}

		//
		// ���� COM �ӿ���Դ.
		//
		CoUninitialize();
	}

	return 0;
}

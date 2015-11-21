/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: Main.cpp

	Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.10.4

    Description: 实现一个D2D绘制的Win32例程.

\*=========================================================================*/

#include <Windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
using namespace D2D1;


//
// 全局变量定义处.
//
HWND						g_hWnd             = NULL;		//全局句柄.
ID2D1Factory*				g_pD2DFactory      = 0;
ID2D1HwndRenderTarget*		g_pD2DRenderTarget = 0;
ID2D1SolidColorBrush*		g_pSolidBrush      = 0;			//纯色画刷.
ID2D1LinearGradientBrush*	g_pLGBrush         = 0;			//线性渐变画刷.


//
// 释放模板.
//
template<typename Type>
void SafeRelease(Type& pObjToRelease)
{
	if(pObjToRelease)
	{
		pObjToRelease->Release();
		pObjToRelease = 0;
	}
}


//
// D2D设备无关资源的创建.
//
BOOL CreateDeviceIndependentResource()
{
	HRESULT hr = NULL;
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&g_pD2DFactory
		);

	return SUCCEEDED(hr);
}


//
// D2D设备相关资源的创建.
//
BOOL CreateDeviceDependentResource()
{
	HRESULT hr = NULL;
	RECT    rc;
	::GetClientRect(g_hWnd, &rc);

	D2D1_SIZE_U size = SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
		);

	//
	// 创建绘制目标区域.
	//
	hr = g_pD2DFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(g_hWnd, size),
		&g_pD2DRenderTarget
		);

	//
	// 创建纯色画刷.
	//
	if(SUCCEEDED(hr))
	{
		hr = g_pD2DRenderTarget->CreateSolidColorBrush(
			ColorF(ColorF::OrangeRed),			//画刷颜色.
			&g_pSolidBrush
			);
	}

	ID2D1GradientStopCollection* pGStop = 0;	//渐变点集合.
	D2D1_GRADIENT_STOP stops[2];				//渐变信息描述数组.
	stops[0].color = ColorF(ColorF::White);		//第一个渐变点颜色.
	stops[1].color = ColorF(ColorF::Blue);		//第二个渐变点颜色.
	stops[0].position = 0.0f;					//第一个渐变点位置百分比.
	stops[1].position = 1.0f;					//第二个渐变点位置百分比.

	if(SUCCEEDED(hr))
	{
		hr = g_pD2DRenderTarget->CreateGradientStopCollection(
			stops,
			sizeof(stops) / sizeof(D2D1_GRADIENT_STOP),
			D2D1_GAMMA_2_2,						//启用Gamma 2.2版本校正.
			D2D1_EXTEND_MODE_CLAMP,				//扩展环绕模式.
			&pGStop
			);
	}

	//
	// 创建线性渐变画刷.
	//
	if(SUCCEEDED(hr))
	{
		hr = g_pD2DRenderTarget->CreateLinearGradientBrush(
			LinearGradientBrushProperties(
			Point2F(0.0f, 0.0f),				//第一个渐变点坐标.
			Point2F(1.0f, 1.0f)					//第二个渐变点坐标.
			),
			pGStop,								//渐变信息集合.
			&g_pLGBrush							//所创建的渐变画刷.
			);

		SafeRelease(pGStop);					//释放局部资源.
	}

	return SUCCEEDED(hr);
}


//
// 销毁资源无关资源.
//
void DiscardDeviceIndependentResource()
{
	SafeRelease(g_pD2DFactory);
}


//
// 销毁资源相关资源.
//
void DiscardDeviceDependentResource()
{
	SafeRelease(g_pLGBrush);
	SafeRelease(g_pSolidBrush);
	SafeRelease(g_pD2DRenderTarget);
}


//
// D2D绘制函数.
//
void Render()
{
	//
	// 如果设备丢失,则创建与设备相关资源.
	//
	if(!g_pD2DRenderTarget)
	{
		if(!CreateDeviceDependentResource())
		{
			return ;
		}
	}

	g_pD2DRenderTarget->BeginDraw();					//开始绘制.
	g_pD2DRenderTarget->Clear(ColorF(ColorF::White));	//将窗体设置为白色.

	//
	// 执行D2D绘制.
	//
	D2D1_POINT_2F StartPoint = Point2F(10.0f, 10.0f);
	D2D1_POINT_2F EndPoint   = Point2F(100.0f, 100.0f);
	g_pSolidBrush->SetColor(ColorF(ColorF::Red));		//设置纯色画刷颜色.

	g_pD2DRenderTarget->DrawLine(		//绘制一条线段.
		StartPoint,						//起点.
		EndPoint,						//终点.
		g_pSolidBrush,					//所使用的画刷.
		8.0f							//线宽.
		);

	D2D1_RECT_F rc = RectF(130.0f, 10.0f, 230.0f, 110.0f);
	g_pSolidBrush->SetColor(ColorF(ColorF::Pink));

	g_pD2DRenderTarget->DrawRectangle(	//绘制一个矩形.
		&rc,							//要绘制的矩形.
		g_pSolidBrush,					//所使用画刷.
		6.0f							//线宽.
		);

	//
	// 用线性渐变画刷绘制一个椭圆.
	//
	D2D1_ELLIPSE ellipse = Ellipse(
		Point2F(350.0f, 70.0f),
		80.0f,
		40.0f
		);

	g_pLGBrush->SetStartPoint(Point2F(270.0f, 30.0f));	//设置渐变起点.
	g_pLGBrush->SetEndPoint(Point2F(430.0f, 110.0f));	//设置渐变终点.
	g_pD2DRenderTarget->DrawEllipse(					//绘制椭圆.
		ellipse,
		g_pLGBrush,
		20.0f
		);

	//
	// 用线性渐变画刷填充一个矩形.
	//
	g_pLGBrush->SetStartPoint(Point2F(10.0f, 150.0f));	//设置渐变起点.
	g_pLGBrush->SetEndPoint(Point2F(160.0f, 300.0f));	//设置渐变终点.

	rc = RectF(10.0f, 150.0f, 160.0f, 300.0f);
	g_pD2DRenderTarget->FillRectangle(
		rc,
		g_pLGBrush
		);

	//
	// 绘制一个绿色圆角矩形.
	//
	rc = RectF(200.0f, 150.0f, 450.0f, 300.0f);
	D2D1_ROUNDED_RECT RoundRc = RoundedRect(rc, 10.0f, 10.0f);

	g_pSolidBrush->SetColor(ColorF(ColorF::ForestGreen));
	g_pD2DRenderTarget->DrawRoundedRectangle(
		&RoundRc,
		g_pSolidBrush,
		2.0f
		);

	HRESULT hr = g_pD2DRenderTarget->EndDraw();

	//
	// 如果设备丢失.我们丢弃设备相关资源以备下次
	// 执行绘制时创建.
	//
	if(D2DERR_RECREATE_TARGET == hr)
	{
		DiscardDeviceDependentResource();
	}
}


//
// 处理WM_SIZE消息.
//
void SizeUpdate()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	g_pD2DRenderTarget->Resize(
		SizeU(rc.right - rc.left, rc.bottom - rc.top)
		);
}


//
// 抓取消息处.
//
void RunMessageLoop()
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


//
// 程序消息回调函数.
//
LRESULT CALLBACK WndProc(
	HWND   hWnd,
	UINT   msg,
	WPARAM wParam,
	LPARAM lParam
	);


//
// Win32程序入口.
//
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       ShowCmd
	)
{
	CreateDeviceIndependentResource();	//创建设备无关资源.

	static TCHAR szAppName[] = TEXT("My_Class_Name");
	WNDCLASS WndClass;
	WndClass.cbClsExtra    = 0;
	WndClass.cbWndExtra    = 0;
	WndClass.lpszMenuName  = NULL;
	WndClass.lpfnWndProc   = WndProc;
	WndClass.hInstance     = hInstance;
	WndClass.lpszClassName = szAppName;
	WndClass.style         = CS_HREDRAW | CS_VREDRAW;
	WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if(!RegisterClass(&WndClass))
	{
		MessageBox(
			NULL,
			TEXT("This Program requires Windows NT!"),
			szAppName,
			MB_OK | MB_ICONEXCLAMATION
			);

		return 0;
	}

	g_hWnd = CreateWindow(
		szAppName,
		TEXT("D2D Demo"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		400,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	CreateDeviceDependentResource();	//创建设备相关资源.

	ShowWindow(g_hWnd, ShowCmd);
	UpdateWindow(g_hWnd);

	RunMessageLoop();

	DiscardDeviceDependentResource();	//销毁设备相关资源.
	DiscardDeviceIndependentResource();	//销毁设备无关资源.

	return 0;
}


//
// 回调函数.
//
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch(msg)
	{
	case WM_PAINT:
		Render();
		return 0;

	case WM_SIZE:
		SizeUpdate();
		return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

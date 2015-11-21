/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: Main.cpp

	Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.10.4

    Description: ʵ��һ��D2D���Ƶ�Win32����.

\*=========================================================================*/

#include <Windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
using namespace D2D1;


//
// ȫ�ֱ������崦.
//
HWND						g_hWnd             = NULL;		//ȫ�־��.
ID2D1Factory*				g_pD2DFactory      = 0;
ID2D1HwndRenderTarget*		g_pD2DRenderTarget = 0;
ID2D1SolidColorBrush*		g_pSolidBrush      = 0;			//��ɫ��ˢ.
ID2D1LinearGradientBrush*	g_pLGBrush         = 0;			//���Խ��仭ˢ.


//
// �ͷ�ģ��.
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
// D2D�豸�޹���Դ�Ĵ���.
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
// D2D�豸�����Դ�Ĵ���.
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
	// ��������Ŀ������.
	//
	hr = g_pD2DFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(g_hWnd, size),
		&g_pD2DRenderTarget
		);

	//
	// ������ɫ��ˢ.
	//
	if(SUCCEEDED(hr))
	{
		hr = g_pD2DRenderTarget->CreateSolidColorBrush(
			ColorF(ColorF::OrangeRed),			//��ˢ��ɫ.
			&g_pSolidBrush
			);
	}

	ID2D1GradientStopCollection* pGStop = 0;	//����㼯��.
	D2D1_GRADIENT_STOP stops[2];				//������Ϣ��������.
	stops[0].color = ColorF(ColorF::White);		//��һ���������ɫ.
	stops[1].color = ColorF(ColorF::Blue);		//�ڶ����������ɫ.
	stops[0].position = 0.0f;					//��һ�������λ�ðٷֱ�.
	stops[1].position = 1.0f;					//�ڶ��������λ�ðٷֱ�.

	if(SUCCEEDED(hr))
	{
		hr = g_pD2DRenderTarget->CreateGradientStopCollection(
			stops,
			sizeof(stops) / sizeof(D2D1_GRADIENT_STOP),
			D2D1_GAMMA_2_2,						//����Gamma 2.2�汾У��.
			D2D1_EXTEND_MODE_CLAMP,				//��չ����ģʽ.
			&pGStop
			);
	}

	//
	// �������Խ��仭ˢ.
	//
	if(SUCCEEDED(hr))
	{
		hr = g_pD2DRenderTarget->CreateLinearGradientBrush(
			LinearGradientBrushProperties(
			Point2F(0.0f, 0.0f),				//��һ�����������.
			Point2F(1.0f, 1.0f)					//�ڶ������������.
			),
			pGStop,								//������Ϣ����.
			&g_pLGBrush							//�������Ľ��仭ˢ.
			);

		SafeRelease(pGStop);					//�ͷžֲ���Դ.
	}

	return SUCCEEDED(hr);
}


//
// ������Դ�޹���Դ.
//
void DiscardDeviceIndependentResource()
{
	SafeRelease(g_pD2DFactory);
}


//
// ������Դ�����Դ.
//
void DiscardDeviceDependentResource()
{
	SafeRelease(g_pLGBrush);
	SafeRelease(g_pSolidBrush);
	SafeRelease(g_pD2DRenderTarget);
}


//
// D2D���ƺ���.
//
void Render()
{
	//
	// ����豸��ʧ,�򴴽����豸�����Դ.
	//
	if(!g_pD2DRenderTarget)
	{
		if(!CreateDeviceDependentResource())
		{
			return ;
		}
	}

	g_pD2DRenderTarget->BeginDraw();					//��ʼ����.
	g_pD2DRenderTarget->Clear(ColorF(ColorF::White));	//����������Ϊ��ɫ.

	//
	// ִ��D2D����.
	//
	D2D1_POINT_2F StartPoint = Point2F(10.0f, 10.0f);
	D2D1_POINT_2F EndPoint   = Point2F(100.0f, 100.0f);
	g_pSolidBrush->SetColor(ColorF(ColorF::Red));		//���ô�ɫ��ˢ��ɫ.

	g_pD2DRenderTarget->DrawLine(		//����һ���߶�.
		StartPoint,						//���.
		EndPoint,						//�յ�.
		g_pSolidBrush,					//��ʹ�õĻ�ˢ.
		8.0f							//�߿�.
		);

	D2D1_RECT_F rc = RectF(130.0f, 10.0f, 230.0f, 110.0f);
	g_pSolidBrush->SetColor(ColorF(ColorF::Pink));

	g_pD2DRenderTarget->DrawRectangle(	//����һ������.
		&rc,							//Ҫ���Ƶľ���.
		g_pSolidBrush,					//��ʹ�û�ˢ.
		6.0f							//�߿�.
		);

	//
	// �����Խ��仭ˢ����һ����Բ.
	//
	D2D1_ELLIPSE ellipse = Ellipse(
		Point2F(350.0f, 70.0f),
		80.0f,
		40.0f
		);

	g_pLGBrush->SetStartPoint(Point2F(270.0f, 30.0f));	//���ý������.
	g_pLGBrush->SetEndPoint(Point2F(430.0f, 110.0f));	//���ý����յ�.
	g_pD2DRenderTarget->DrawEllipse(					//������Բ.
		ellipse,
		g_pLGBrush,
		20.0f
		);

	//
	// �����Խ��仭ˢ���һ������.
	//
	g_pLGBrush->SetStartPoint(Point2F(10.0f, 150.0f));	//���ý������.
	g_pLGBrush->SetEndPoint(Point2F(160.0f, 300.0f));	//���ý����յ�.

	rc = RectF(10.0f, 150.0f, 160.0f, 300.0f);
	g_pD2DRenderTarget->FillRectangle(
		rc,
		g_pLGBrush
		);

	//
	// ����һ����ɫԲ�Ǿ���.
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
	// ����豸��ʧ.���Ƕ����豸�����Դ�Ա��´�
	// ִ�л���ʱ����.
	//
	if(D2DERR_RECREATE_TARGET == hr)
	{
		DiscardDeviceDependentResource();
	}
}


//
// ����WM_SIZE��Ϣ.
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
// ץȡ��Ϣ��.
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
// ������Ϣ�ص�����.
//
LRESULT CALLBACK WndProc(
	HWND   hWnd,
	UINT   msg,
	WPARAM wParam,
	LPARAM lParam
	);


//
// Win32�������.
//
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       ShowCmd
	)
{
	CreateDeviceIndependentResource();	//�����豸�޹���Դ.

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

	CreateDeviceDependentResource();	//�����豸�����Դ.

	ShowWindow(g_hWnd, ShowCmd);
	UpdateWindow(g_hWnd);

	RunMessageLoop();

	DiscardDeviceDependentResource();	//�����豸�����Դ.
	DiscardDeviceIndependentResource();	//�����豸�޹���Դ.

	return 0;
}


//
// �ص�����.
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

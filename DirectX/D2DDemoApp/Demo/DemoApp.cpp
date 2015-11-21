//////////////////////////////////////////////////////////////////////////
// DemoApp.cpp: CDemoApp类的实现文件.
//

#include "DemoApp.h"


CD2DDemoApp::CD2DDemoApp()
	: m_hWnd(NULL)
	, m_pD2DFactory(0)
	, m_pRenderTarget(0)
	, m_pSolidBrush(0)
	, m_pLGBrush(0)
	, m_pRGBrush(0)
{
}


CD2DDemoApp::~CD2DDemoApp()
{
	DiscardDeviceDependentResources();
	DiscardDeviceIndependentResources();
}


//
// 丢弃依赖于设备的资源.
//
void CD2DDemoApp::DiscardDeviceDependentResources()
{
	SafeRelease(m_pLGBrush);
	SafeRelease(m_pRGBrush);
	SafeRelease(m_pSolidBrush);
	SafeRelease(m_pRenderTarget);
}


//
// 丢弃设备无关资源.
//
void CD2DDemoApp::DiscardDeviceIndependentResources()
{
	SafeRelease(m_pD2DFactory);
}


HWND CD2DDemoApp::GetHwnd() const
{
	return m_hWnd;
}


//
// 创建依赖于非设备的资源.
//
BOOL CD2DDemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = NULL;

	//
	// 创建D2D工厂.
	//
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory
		);

	return SUCCEEDED(hr);
}


//
// 创建依赖于设备的资源.
//
BOOL CD2DDemoApp::CreateDeviceDependentResources()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);

	D2D1_SIZE_U size = SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
		);

	//
	// 创建D2D目标绘制区域.
	//
	HRESULT hr = NULL;
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(m_hWnd, size),
		&m_pRenderTarget
		);

	//
	// 创建纯色画刷.
	//
	if(SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(
			ColorF(ColorF::OrangeRed),
			&m_pSolidBrush
			);
	}

	ID2D1GradientStopCollection* pGStop = 0;
	D2D1_GRADIENT_STOP stops[2];

	stops[0].color = ColorF(ColorF::White);
	stops[1].color = ColorF(ColorF::Blue);
	stops[0].position = 0.0f;
	stops[1].position = 1.0f;

	if(SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateGradientStopCollection(
			stops,
			sizeof(stops) / sizeof(D2D1_GRADIENT_STOP),
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGStop
			);
	}

	//
	// 创建线性渐变画刷.
	//
	if(SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateLinearGradientBrush(
			LinearGradientBrushProperties(
				Point2F(0.0f, 0.0f),
				Point2F(1.0f, 1.0f)
				),
			pGStop,
			&m_pLGBrush
			);

		SafeRelease(pGStop);
	}

	//
	// 创建放射渐变画刷.
	//
	if(SUCCEEDED(hr))
	{
		stops[0].color = ColorF(ColorF::Red);
		stops[1].color = ColorF(ColorF::Yellow);

		hr = m_pRenderTarget->CreateGradientStopCollection(
			stops,
			sizeof(stops) / sizeof(D2D1_GRADIENT_STOP),
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGStop
			);

		if(FAILED(hr))
		{
			return FALSE;
		}

		hr = m_pRenderTarget->CreateRadialGradientBrush(
			RadialGradientBrushProperties(
				Point2F(10.0f, 10.0f),		//绘制中心.
				Point2F(0.0f, 0.0f),		//放射点距绘制中心的偏移.
				20.0f,						//x半径.
				20.0f						//y半径.
				),
			pGStop,
			&m_pRGBrush
			);

		SafeRelease(pGStop);
	}

	return SUCCEEDED(hr);
}


BOOL CD2DDemoApp::Initialize(HINSTANCE hInstance)
{
	WNDCLASSEX wcex    = { sizeof(WNDCLASSEX) };
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = CD2DDemoApp::WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = sizeof(LONG_PTR);		//为用户留4自节的指针存放区域.
	wcex.hInstance     = hInstance;
	wcex.lpszMenuName  = NULL;
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hbrBackground = NULL;
	wcex.lpszClassName = _T("D2DDemoApp");

	if(!RegisterClassEx(&wcex))
	{
		WARNING_HWND_MSG(NULL, _T("注册窗口类失败!"));
		return FALSE;
	}

	//
	// 创建窗口.
	//
	m_hWnd = CreateWindow(
		_T("D2DDemoApp"),
		_T("Direct2D Demo -- ZYF"),
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		480,
		480,
		NULL,
		NULL,
		hInstance,
		this			//保存this指针.
		);

	//
	// 创建D2D资源.
	//
	if(!CreateDeviceIndependentResources() || !CreateDeviceDependentResources())
	{
		WARNING_HWND_MSG(m_hWnd, _T("创建Dirct2D资源失败!"));
		return FALSE;
	}

	//
	// 显示并更新窗口.
	//
	if(m_hWnd)
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}
	else
	{
		return FALSE;
	}

	OnRender();				//窗体一显示出来就执行绘制.

	return TRUE;
}


//
// 消息循环.
//
void CD2DDemoApp::RunMessageLoop()
{
	MSG message;

	while(GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}


//
// 窗体客户区绘制函数.
//
void CD2DDemoApp::OnRender()
{
	//
	// 检查设备是否丢失,如果丢失则需要重新创建.
	//
	if(!m_pRenderTarget)
	{
		if(!CreateDeviceDependentResources())
		{
			return ;
		}
	}

	HRESULT hr = NULL;

	//
	//初始化绘制信息.
	//
	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
	m_pRenderTarget->Clear(ColorF(ColorF::White));

	D2D1_SIZE_F size = m_pRenderTarget->GetSize();
	int iWidth  = static_cast<int>(size.width);
	int iHeight = static_cast<int>(size.height);

	//
	// 开始绘制.
	//
	D2D1_POINT_2F StartPoint = Point2F(10.0f, 10.0f);
	D2D1_POINT_2F EndPoint   = Point2F(100.0f, 100.0f);

	m_pSolidBrush->SetColor(ColorF(ColorF::Red));
	m_pRenderTarget->DrawLine(		//纯色画刷绘制一条线.
		StartPoint,
		EndPoint,
		m_pSolidBrush,
		8.0f
		);

	D2D1_RECT_F rc = RectF(130.0f, 10.0f, 230.0f, 110.0f);
	m_pSolidBrush->SetColor(ColorF(ColorF::Pink));
	m_pRenderTarget->DrawRectangle(
		&rc,
		m_pSolidBrush,
		6.0f
		);

	//用线性渐变画刷绘制一个椭圆.
	D2D1_ELLIPSE ellipse = Ellipse(
		Point2F(350.0f, 70.0f),
		80.0f,
		40.0f
		);
	m_pLGBrush->SetStartPoint(Point2F(270.0f, 30.0f));
	m_pLGBrush->SetEndPoint(Point2F(430.0f, 110.0f));
	m_pRenderTarget->DrawEllipse(
		ellipse,
		m_pLGBrush,
		20.0f
		);

	//用线性渐变画刷绘制一个矩形.
	m_pLGBrush->SetStartPoint(Point2F(10.0f, 150.0f));
	m_pLGBrush->SetEndPoint(Point2F(160.0f, 300.0f));
	rc = RectF(10.0f, 150.0f, 160.0f, 300.0f);
	m_pRenderTarget->FillRectangle(
		rc,
		m_pLGBrush
		);

	//用放射状画刷绘制一个圆.
	D2D1_POINT_2F CenterPt = Point2F(250.0f, 230.0f);
	const FLOAT fRadial = 80.0f;
	ellipse = Ellipse(
		CenterPt,
		fRadial,
		fRadial
		);
	m_pRGBrush->SetCenter(CenterPt);
	m_pRGBrush->SetRadiusX(fRadial);
	m_pRGBrush->SetRadiusY(fRadial);
	m_pRenderTarget->FillEllipse(
		&ellipse,
		m_pRGBrush
		);

	//绘制一个绿色圆角矩形.
	rc = RectF(350.0f, 150.0f, 450.0f, 300.0f);
	D2D1_ROUNDED_RECT RoundRc = RoundedRect(rc, 10.0f, 10.0f);
	m_pSolidBrush->SetColor(ColorF(ColorF::ForestGreen));
	m_pRenderTarget->DrawRoundedRectangle(
		&RoundRc,
		m_pSolidBrush,
		2.0f
		);

	//
	//结束绘制.
	//
	hr = m_pRenderTarget->EndDraw();

	//
	// 检查设备,如果丢失,则丢弃以备下次渲染时重新创建.
	//
	if(hr == D2DERR_RECREATE_TARGET)
	{
		DiscardDeviceDependentResources();
	}
}


//
// 窗口大小变更响应函数.
//
void CD2DDemoApp::OnResize(UINT uWidth, UINT uHeight)
{
	if(m_pRenderTarget)
	{
		m_pRenderTarget->Resize(SizeU(uWidth, uHeight));
	}
}


//
// 窗体消息过程处理函数.
//
LRESULT CALLBACK CD2DDemoApp::WndProc(
	HWND   hWnd,
	UINT   uMessage,
	WPARAM wParam,
	LPARAM lParam
	)
{
	LRESULT result = 0;

	if(uMessage == WM_CREATE)
	{
		LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;
		CD2DDemoApp* pDemoApplication = (CD2DDemoApp*)lpCreateStruct->lpCreateParams;

		//
		// 把this指针保存到用户自定义数据区域.
		//
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pDemoApplication));

		result = 1;
	}
	else
	{
		//
		// 从客户自定义数据区中取出this指针.
		//
		CD2DDemoApp* pDemoApp = reinterpret_cast<CD2DDemoApp*>(
			static_cast<LONG_PTR>(::GetWindowLongPtr(
				hWnd,
				GWLP_USERDATA
				)));

		result = 0;
		BOOL bMsgHandled = FALSE;

		if(pDemoApp)
		{
			switch(uMessage)
			{
			case WM_SIZE:
				pDemoApp->OnResize(
					LOWORD(lParam),
					HIWORD(lParam)
					);
				bMsgHandled = TRUE;
				break;
			
			case WM_DISPLAYCHANGE:
				InvalidateRect(
					hWnd,
					NULL,
					FALSE
					);		//使客户区无效以致重绘.
				bMsgHandled = TRUE;
				break;

			case WM_PAINT:
				pDemoApp->OnRender();
				ValidateRect(
					hWnd,
					NULL
					);		//客户区重绘完成后设为有效.
				bMsgHandled = TRUE;
				break;

			case WM_ERASEBKGND:
				return FALSE;

			case WM_DESTROY:
				PostQuitMessage(0);
				result = 1;
				bMsgHandled = TRUE;
				break;

			default:
				break;
			}
		}

		if(!bMsgHandled)
		{
			return DefWindowProc(
				hWnd,
				uMessage,
				wParam,
				lParam
				);
		}
	}

	return result;
}

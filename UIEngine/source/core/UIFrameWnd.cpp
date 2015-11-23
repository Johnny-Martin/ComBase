#include "stdafx.h"
#include "UIFrameWnd.h"
#include "ResManager.h"

bool FrameWnd::Initialize(HINSTANCE hInstance)
{
	WNDCLASSEX wcex    = { sizeof(WNDCLASSEX) };
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = sizeof(LONG_PTR);		//Ϊ�û���4�Խڵ�ָ��������.
	wcex.hInstance     = hInstance;
	wcex.lpszMenuName  = NULL;
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hbrBackground = NULL;
	wcex.lpszClassName = _T("FrameWnd");

	if(!RegisterClassEx(&wcex))
	{
		WARNING_HWND_MSG(NULL, _T("ע�ᴰ����ʧ��!"));
		return false;
	}

	//
	// ��������.
	//
	m_hWnd = CreateWindow(
		_T("FrameWnd"),
		_T("FrameWnd Demo -- MJT"),
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		480,
		480,
		NULL,
		NULL,
		hInstance,
		this			//����thisָ��.
		);

	//
	// ����D2D��Դ.
	//
	if(!CreateDeviceIndependentResources() || !CreateDeviceDependentResources())
	{
		WARNING_HWND_MSG(m_hWnd, _T("����Dirct2D��Դʧ��!"));
		return FALSE;
	}


	if(m_hWnd)
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}else
	{
		return false;
	}

	OnRender();				//����һ��ʾ������ִ�л���.

	return TRUE;
}

//
// �����������豸����Դ.
//
void FrameWnd::DiscardDeviceDependentResources()
{
	SafeRelease(m_pLGBrush);
	SafeRelease(m_pRGBrush);
	SafeRelease(m_pSolidBrush);
	SafeRelease(m_pRenderTarget);
}


//
// �����豸�޹���Դ.
//
void FrameWnd::DiscardDeviceIndependentResources()
{
	SafeRelease(m_pD2DFactory);
}


//
// ���������ڷ��豸����Դ.
//
BOOL FrameWnd::CreateDeviceIndependentResources()
{
	HRESULT hr = NULL;

	//
	// ����D2D����.
	//
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory
		);
	
	return SUCCEEDED(hr);
}


//
// �����������豸����Դ.
//
BOOL FrameWnd::CreateDeviceDependentResources()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);

	D2D1_SIZE_U size = SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
		);

	//
	// ����D2DĿ���������.
	//
	HRESULT hr = NULL;
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(m_hWnd, size),
		&m_pRenderTarget
		);

	//
	// ������ɫ��ˢ.
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
	// �������Խ��仭ˢ.
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
	// �������佥�仭ˢ.
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
			Point2F(10.0f, 10.0f),		//��������.
			Point2F(0.0f, 0.0f),		//������������ĵ�ƫ��.
			20.0f,						//x�뾶.
			20.0f						//y�뾶.
			),
			pGStop,
			&m_pRGBrush
			);

		SafeRelease(pGStop);
	}

	return SUCCEEDED(hr);
}


LRESULT CALLBACK FrameWnd::WndProc(
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
		FrameWnd* pDemoApplication = (FrameWnd*)lpCreateStruct->lpCreateParams;

		//
		// ��thisָ�뱣�浽�û��Զ�����������.
		//
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pDemoApplication));

		result = 1;
	}
	else
	{
		//
		// �ӿͻ��Զ�����������ȡ��thisָ��.
		//
		FrameWnd* pDemoApp = reinterpret_cast<FrameWnd*>(
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
					);		//ʹ�ͻ�����Ч�����ػ�.
				bMsgHandled = TRUE;
				break;

			case WM_PAINT:
				pDemoApp->OnRender();
				ValidateRect(
					hWnd,
					NULL
					);		//�ͻ����ػ���ɺ���Ϊ��Ч.
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

void FrameWnd::OnResize(UINT uWidth, UINT uHeight)
{
	if(m_pRenderTarget)
	{
		m_pRenderTarget->Resize(SizeU(uWidth, uHeight));
	}
}

void FrameWnd::OnRender()
{
	//
	// ����豸�Ƿ�ʧ,�����ʧ����Ҫ���´���.
	//
	if(!m_pRenderTarget)
	{
		return;
	}

	HRESULT hr = NULL;

	//
	//��ʼ��������Ϣ.
	//
	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
	m_pRenderTarget->Clear(ColorF(ColorF::White));

	D2D1_SIZE_F size = m_pRenderTarget->GetSize();
	int iWidth  = static_cast<int>(size.width);
	int iHeight = static_cast<int>(size.height);

	//
	// ��ʼ����.
	//
	D2D1_POINT_2F StartPoint = Point2F(10.0f, 10.0f);
	D2D1_POINT_2F EndPoint   = Point2F(100.0f, 100.0f);

	m_pSolidBrush->SetColor(ColorF(ColorF::Red));
	m_pRenderTarget->DrawLine(		//��ɫ��ˢ����һ����.
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

	ResManager resManager(L"E:\\code\\ComBase\\trunk\\UIEngine\\docs");
	RPicture *pic;
	RESERROR resErr = resManager.GetResPicHandle("image.pngImage", &pic);
	if (RES_SUCCESS == resErr)
	{
		//����λͼ +++++++++++++++++++++++++++++++++++++++++++=
		//m_pRenderTarget->CreateBitmap()
		//����λͼ��ˢ
	}
	//E:\\code\\ComBase\\trunk\\UIEngine\\docs\\image.pngImage.png
	//LoadBitmapFromFile(p_pRenderTarget,p_pImageFactory,L"car.jpg",0,0,&p_pBitmap);
	//
	//��������.
	//
	hr = m_pRenderTarget->EndDraw();

	//
	// ����豸,�����ʧ,�����Ա��´���Ⱦʱ���´���.
	//
	if(hr == D2DERR_RECREATE_TARGET)
	{
		SafeRelease(m_pD2DFactory);
	}
}

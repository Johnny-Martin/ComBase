#include "stdafx.h"
#include "UIFrameWnd.h"
#include "ResManager.h"
#include "Wincodec.h"

#pragma comment(lib, "Windowscodecs.lib")

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
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = _T("FrameWnd");

	if(!RegisterClassEx(&wcex))
	{
		WARNING_HWND_MSG(NULL, _T("ע�ᴰ����ʧ��!"));
		return false;
	}

	// ��������.
	m_hWnd = CreateWindowEx(WS_EX_TOPMOST,
		_T("FrameWnd"),
		_T("FrameWnd Demo -- MJT"),
		WS_POPUP,//WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		480,
		480,
		NULL,
		NULL,
		hInstance,
		this			//����thisָ��.
		);

	// ����D2D��Դ.
	if(!CreateDeviceIndependentResources() || !CreateDeviceDependentResources())
	{
		WARNING_HWND_MSG(m_hWnd, _T("����Dirct2D��Դʧ��!"));
		return FALSE;
	}


	if(m_hWnd)
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);

/*		RECT wndRect;  
		::GetWindowRect(m_hWnd,&wndRect);  
		SIZE wndSize = {wndRect.right-wndRect.left,wndRect.bottom-wndRect.top};  
		HDC hdc = ::GetDC(m_hWnd);  
		m_hWndDC = ::CreateCompatibleDC(hdc);  
		
		HDC screenDC = GetDC(NULL);  
		POINT ptSrc = {0,0};  

		BLENDFUNCTION blendFunction;  
		blendFunction.AlphaFormat = AC_SRC_ALPHA;  
		blendFunction.BlendFlags = 0;  
		blendFunction.BlendOp = AC_SRC_OVER;  
		blendFunction.SourceConstantAlpha = 255;  
		UpdateLayeredWindow(m_hWnd,screenDC,&ptSrc,&wndSize,m_hWndDC,&ptSrc,0,&blendFunction,2);*/  

		//::DeleteDC(memDC);  
	}else
	{
		return false;
	}

	OnRender();				//����һ��ʾ������ִ�л���.

	return TRUE;
}

BOOL FrameWnd::CreateDeviceIndependentResources()
{
	HRESULT hr = NULL;
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory
		);
	
	return SUCCEEDED(hr);
}

BOOL FrameWnd::CreateDeviceDependentResources()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);

	D2D1_SIZE_U size = SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
		);

	//DXGI_FORMAT_R8G8B8A8_UNORM
	// ����D2DĿ���������.
	//DXGI_FORMAT_B8G8R8A8_UNORM
	D2D1_PIXEL_FORMAT pixelFormat =  PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,pixelFormat);
	HRESULT hr = NULL;
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		renderTargetProperties,
		HwndRenderTargetProperties(m_hWnd, size),
		&m_pRenderTarget
		);

	//To enable the device context (DC) render target to work with GDI, set the DXGI format 
	//to DXGI_FORMAT_B8G8R8A8_UNORM and the alpha mode to D2D1_ALPHA_MODE_PREMULTIPLIED or 
	//D2D1_ALPHA_MODE_IGNORE. 
	pixelFormat =  PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	renderTargetProperties = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,pixelFormat);
	hr = m_pD2DFactory->CreateDCRenderTarget(
		&renderTargetProperties,
		&m_pDCRenderTarget
		);/**/
	if (SUCCEEDED(hr))
	{
		HDC windowDC = GetWindowDC(m_hWnd);
		GetWindowRect(m_hWnd, &rc);
		m_pDCRenderTarget->BindDC(windowDC, &rc);
	}
	return SUCCEEDED(hr);
}


LRESULT CALLBACK FrameWnd::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if(uMessage == WM_CREATE)
	{
		LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;
		FrameWnd* pDemoApplication = (FrameWnd*)lpCreateStruct->lpCreateParams;

		// ��thisָ�뱣�浽�û��Զ�����������.
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pDemoApplication));

		// ���÷ֲ�����  
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);  
		// ����͸��ɫ   
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);   
		result = 1;
	}
	else
	{
		// �ӿͻ��Զ�����������ȡ��thisָ��.
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
			case WM_ERASEBKGND: //�ڴ��ڱ�����ֱ����ͼ  
				{  
					return TRUE;  
				}  
			case WM_SIZE:
				/*pDemoApp->OnResize(
					LOWORD(lParam),
					HIWORD(lParam)
					);*/
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
				//pDemoApp->OnRender();
				ValidateRect(
					hWnd,
					NULL
					);		//�ͻ����ػ���ɺ���Ϊ��Ч.
				bMsgHandled = TRUE;
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				result = 1;
				bMsgHandled = TRUE;
				break;
			case WM_CTLCOLORDLG:
				
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

ResManager resManager(L"I:\\UIEngine\\docs");
RPicture* pic = NULL;
void FrameWnd::OnRender()
{
	// ����豸�Ƿ�ʧ,�����ʧ����Ҫ���´���.
	if(!m_pRenderTarget)
	{
		return;
	}

	HRESULT hr = NULL;

	//��ʼ��������Ϣ.
	/*m_pRenderTarget->BeginDraw();
	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
	m_pRenderTarget->Clear(ColorF(ColorF::Black));*/

	m_pDCRenderTarget->BeginDraw();
	m_pDCRenderTarget->SetTransform(Matrix3x2F::Identity());
	m_pDCRenderTarget->Clear(ColorF(ColorF::Black));

	D2D1_SIZE_F size = m_pRenderTarget->GetSize();
	int iWidth  = static_cast<int>(size.width);
	int iHeight = static_cast<int>(size.height);

	
	if (NULL == pic)
	{
		//RESERROR resErr = resManager.GetResPicHandle("texturelist.Nine.btnbkg.4", &pic);
		//if (RES_SUCCESS == resErr)/**/
		{
			pic = new RTexture(L"I:\\UIEngine\\docs\\texture.NineInOne.wndbkg_shadow.png", "texture.NineInOne.wndbkg_shadow");
			//pic = new RTexture(L"I:\\UIEngine\\docs\\texture.ThreeV.xBtnBkg_Hover.png", "texture.ThreeV.xBtnBkg_Hover");
			//pic = new RTexture(L"I:\\UIEngine\\docs\\texture.ThreeH.listctrl_scroll_bkg.png", "texture.ThreeH.listctrl_scroll_bkg");
			//pic->WritePngFile(L"I:\\UIEngine\\docs\\AAABBB.png");	
		}
	}
	//pic->Draw(m_pRenderTarget, 120, 210, 340, 440);
	pic->Draw(m_pDCRenderTarget, 120, 210, 340, 440);

	//��������.
	//hr = m_pRenderTarget->EndDraw();
	

	POINT sourcePosition = {};
	POINT windowPosition = {};
	SIZE  dcSize = { 480, 480 };

	BLENDFUNCTION blend = {};
	blend.SourceConstantAlpha = 0;
	blend.AlphaFormat = AC_SRC_ALPHA;

	UPDATELAYEREDWINDOWINFO info = {};
	info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
	info.pptSrc = &sourcePosition;
	info.pptDst = &windowPosition;
	info.psize = &dcSize;
	info.pblend = &blend;
	info.dwFlags = ULW_ALPHA;
	info.hdcSrc = GetWindowDC(m_hWnd);
	
	UpdateLayeredWindowIndirect(m_hWnd, &info);

	hr = m_pDCRenderTarget->EndDraw();

	//RenderTargetDC dc(m_interopTarget);
	// ����豸,�����ʧ,�����Ա��´���Ⱦʱ���´���.
	if(hr == D2DERR_RECREATE_TARGET)
	{
		SafeRelease(m_pD2DFactory);
	}
}

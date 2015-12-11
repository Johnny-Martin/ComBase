#include "stdafx.h"
#include "UIFrameWnd.h"
#include "ResManager.h"
#include "Wincodec.h"

#pragma comment(lib, "Windowscodecs.lib")


set<string> CBaseWnd::m_attrNameSet = CBaseWnd::InitAttrNameSet();
set<string> CBaseWnd::m_eventNameSet = CBaseWnd::InitEventNameSet();

set<string> CBaseWnd::InitAttrNameSet()
{
	set<string> attrNameSet;
	attrNameSet.insert("position");
	attrNameSet.insert("left");
	attrNameSet.insert("top");
	attrNameSet.insert("right");
	attrNameSet.insert("bottom");
	attrNameSet.insert("leftexp");
	attrNameSet.insert("topexp");
	attrNameSet.insert("rightexp");
	attrNameSet.insert("bottomexp");
	attrNameSet.insert("title");
	attrNameSet.insert("visible");
	attrNameSet.insert("enable");
	attrNameSet.insert("topmost");
	attrNameSet.insert("layered");
	attrNameSet.insert("appwnd");
	attrNameSet.insert("blur");
	attrNameSet.insert("minenable");
	attrNameSet.insert("maxenable");
	attrNameSet.insert("rootobjectid");

	return attrNameSet;
}
bool CBaseWnd::InitAttrMap()
{
	m_attrMap.insert(pair<string, string>("position", ""));
	m_attrMap.insert(pair<string, string>("left", "0"));
	m_attrMap.insert(pair<string, string>("top", "0"));
	m_attrMap.insert(pair<string, string>("right", "0"));
	m_attrMap.insert(pair<string, string>("bottom", "0"));
	m_attrMap.insert(pair<string, string>("leftexp", ""));
	m_attrMap.insert(pair<string, string>("topexp", ""));
	m_attrMap.insert(pair<string, string>("rightexp", ""));
	m_attrMap.insert(pair<string, string>("bottomexp", ""));
	m_attrMap.insert(pair<string, string>("visible", "1"));
	m_attrMap.insert(pair<string, string>("enable", "1"));
	m_attrMap.insert(pair<string, string>("topmost", "0"));
	m_attrMap.insert(pair<string, string>("layered", "1"));
	m_attrMap.insert(pair<string, string>("appwnd", "1"));
	m_attrMap.insert(pair<string, string>("blur", "0"));
	m_attrMap.insert(pair<string, string>("minenable", "1"));
	m_attrMap.insert(pair<string, string>("maxenable", "1"));
	m_attrMap.insert(pair<string, string>("rootobjectid", ""));
	return true;
}
set<string> CBaseWnd::InitEventNameSet()
{
	set<string> eventNameSet;
	eventNameSet.insert("OnCreate");
	eventNameSet.insert("OnShowWnd");
	eventNameSet.insert("OnShowWnd");
	eventNameSet.insert("OnDestory");
	eventNameSet.insert("OnStateChange");
	eventNameSet.insert("OnMove");
	eventNameSet.insert("OnSize");
	eventNameSet.insert("OnVisibleChange");
	eventNameSet.insert("OnEnableChange");
	eventNameSet.insert("OnCreate");

	return eventNameSet;
}
bool CBaseWnd::SetAttr(string key, string value)
{
	if (m_attrNameSet.end() != m_attrNameSet.find(key))
	{
		m_attrMap.insert(pair<string, string>(key, value));
		return true;
	}
	return false;
}
bool CBaseWnd::GetAttr(string key, string* value)
{
	map<string, string>::iterator iter = m_attrMap.find(key);
	if (m_attrMap.end() != iter)
	{
		*value = iter->second;
		return true;
	}
	return false;
}

FrameWnd::FrameWnd():m_hWnd(NULL)
,m_hWndDC(NULL)
,m_pD2DFactory(NULL)
,m_pRenderTarget(NULL)
,m_pDCRenderTarget(NULL)
{}

FrameWnd::~FrameWnd()
{
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pDCRenderTarget);
	SafeRelease(m_pSolidBrush);
};

void FrameWnd::RunMessageLoop()
{
	MSG message;
	while(GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}
HWND FrameWnd::GetWndHandle()
{
	return m_hWnd;
}
void FrameWnd::DiscardDeviceResources()
{
	SafeRelease(m_pDCRenderTarget);
	SafeRelease(m_pSolidBrush);
}
HRESULT FrameWnd::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateDeviceIndependentResources();
	if (!SUCCEEDED(hr))
		return hr;

	WNDCLASSEX wcex    = { sizeof(WNDCLASSEX) };
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = sizeof(LONG_PTR);		//extra window memory for saving 'this' pointer
	wcex.hInstance     = hInstance;
	wcex.lpszMenuName  = NULL;
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = _T("FrameWnd");

	if(!RegisterClassEx(&wcex))
	{
		WARNING_HWND_MSG(NULL, _T("register window failed!"));
		return false;
	}

	// ´´½¨´°¿Ú.
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
		this  //be passed to the window through the CREATESTRUCT structure (lpCreateParams member),
		      // CREATESTRUCT structure is pointed by the lParam param of the WM_CREATE message.
		);

	hr = m_hWnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}

	return hr;
}

HRESULT FrameWnd::CreateDeviceIndependentResources()
{
	return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
}

HDC MemDC;
HBITMAP DIBSectionBitmap;
HBITMAP OldBmp;
HRESULT FrameWnd::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_pDCRenderTarget)
	{
		// Create Dirct2D render target.
		/*D2D1_PIXEL_FORMAT pixelFormat =  PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
		D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,pixelFormat);
		HRESULT hr = NULL;
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			renderTargetProperties,
			HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget
			);*/

		//ID2D1DCRenderTarget do not support the DXGI_FORMAT_R8G8B8A8_UNORM pixel format
		D2D1_PIXEL_FORMAT pixelFormat =  PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
		D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,pixelFormat);
		
		hr = m_pD2DFactory->CreateDCRenderTarget(&renderTargetProperties, &m_pDCRenderTarget);
		if (SUCCEEDED(hr))
		{
			hr = m_pDCRenderTarget->CreateSolidColorBrush(ColorF(ColorF::OrangeRed), &m_pSolidBrush);
		}
	}
	//RECT rc;
	//::GetClientRect(m_hWnd, &rc);

	//D2D1_SIZE_U size = SizeU(
	//	rc.right - rc.left,
	//	rc.bottom - rc.top
	//	);

	//

	////initialize the device context
	//RECT r = {0};
	//GetWindowRect(m_hWnd,&r);
	//HDC scrDC = GetDC(0);
	//MemDC = CreateCompatibleDC(scrDC);
	//ReleaseDC(0,scrDC);
	//if(!MemDC)
	//{
	//	// FailInit();
	//}
	//BITMAPINFO bmi = {0};
	//bmi.bmiHeader.biBitCount = 32;
	//bmi.bmiHeader.biCompression = BI_RGB;
	//bmi.bmiHeader.biPlanes = 1;
	//bmi.bmiHeader.biWidth = r.right-r.left;
	//bmi.bmiHeader.biHeight = r.bottom-r.top;
	//bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	//DIBSectionBitmap = CreateDIBSection(MemDC,&bmi,DIB_RGB_COLORS,0,0,0);
	//if(!DIBSectionBitmap)
	//	return 0;
	//OldBmp = (HBITMAP)SelectObject(MemDC,DIBSectionBitmap);
	//// Now create the HWND D2D1 RenderTarget.


	//To enable the device context (DC) render target to work with GDI, set the DXGI format 
	//to DXGI_FORMAT_B8G8R8A8_UNORM and the alpha mode to D2D1_ALPHA_MODE_PREMULTIPLIED or 
	//D2D1_ALPHA_MODE_IGNORE. 
	
	return hr;
}

// FrameWnd::WndProc is a static member function, so there's no 'this' pointer in the 
//list of arguments. We can specify 'this' pointer to the last parameter of CreateWindowEx
//And get 'this' pointer in WM_CREATE event and save it to 
LRESULT CALLBACK FrameWnd::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if(uMessage == WM_CREATE)
	{
		LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;
		FrameWnd* pDemoApplication = (FrameWnd*)lpCreateStruct->lpCreateParams;

		// Save 'this' pointer to extra window memory
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pDemoApplication));

		// set 'WS_EX_LAYERED' style
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);      
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);   
		result = 1;
	}
	else
	{
		// get 'this' pointer from extra window memory.
		FrameWnd* pDemoApp = reinterpret_cast<FrameWnd*>(static_cast<LONG_PTR>(::GetWindowLongPtr(hWnd, GWLP_USERDATA)));

		result = 0;
		BOOL bMsgHandled = FALSE;

		if(pDemoApp)
		{
			switch(uMessage)
			{
			case WM_ERASEBKGND: //draw bitmap on the background of the window  
				{  
					//return TRUE;  
				}  
			case WM_SIZE:
				pDemoApp->OnResize(
					LOWORD(lParam),
					HIWORD(lParam)
					);/**/
				bMsgHandled = TRUE;
				break;

			//case WM_DISPLAYCHANGE:
			//	//add a rectangle to the window's update region,so that 
			//	//the window's client area can be redrawn
			//	InvalidateRect(hWnd, NULL, FALSE);
			//	bMsgHandled = TRUE;
			//	break;

			case WM_PAINT:
			case WM_DISPLAYCHANGE:
				PAINTSTRUCT ps;
				BeginPaint(pDemoApp->GetWndHandle(), &ps);
				pDemoApp->OnRender(ps);
				EndPaint(pDemoApp->GetWndHandle(), &ps);
				//remove the rectangle from client area after being drawn
				ValidateRect(hWnd, NULL);
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
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
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
void FrameWnd::OnRender(const PAINTSTRUCT &ps)
{
	HRESULT hr = CreateDeviceResources();
	if (!SUCCEEDED(hr))
	{
		WARNING_HWND_MSG(NULL, _T("CreateDeviceResources failed!"));
		return;
	}
	
	//Initialize the render target.
	RECT dcRect;
	// Get the dimensions of the client drawing area.
	GetClientRect(m_hWnd, &dcRect);
	m_pDCRenderTarget->BindDC(ps.hdc, &dcRect);
	
	m_pDCRenderTarget->BeginDraw();
	m_pDCRenderTarget->SetTransform(Matrix3x2F::Identity());
	//m_pDCRenderTarget->Clear(ColorF(ColorF::Black));
	m_pDCRenderTarget->Clear(ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	D2D1_RECT_F rc = RectF(130.0f, 10.0f, 230.0f, 110.0f);
	m_pSolidBrush->SetColor(ColorF(0.0f,0.7f,0.1f,0.5f));
	m_pDCRenderTarget->DrawRectangle(&rc, m_pSolidBrush,6.0f);

	if (NULL == pic)
	{
		//RESERROR resErr = resManager.GetResPicHandle("texturelist.Nine.btnbkg.4", &pic);
		//if (RES_SUCCESS == resErr)/**/
		{
			pic = new RTexture(L"I:\\UIEngine\\docs\\texture.NineInOne.wndbkg_shadow.png", "texture.NineInOne.wndbkg_shadow");
			//pic = new RTexture(L"I:\\UIEngine\\docs\\texture.ThreeV.xBtnBkg_Hover.png", "texture.ThreeV.xBtnBkg_Hover");
			//pic = new RTexture(L"I:\\UIEngine\\docs\\texture.ThreeH.listctrl_scroll_bkg.png", "texture.ThreeH.listctrl_scroll_bkg");
			//pic = new RImage(L"I:\\UIEngine\\docs\\image.settingIcon.png", "image.settingIcon");
			//pic->WritePngFile(L"I:\\UIEngine\\docs\\AAABBB.png");	
		}
	}
	//pic->Draw(m_pRenderTarget, 120, 210, 340, 440);
	pic->Draw(m_pDCRenderTarget, 120, 210, 340, 440);

	//POINT sourcePosition = {};
	//POINT windowPosition = {};
	//SIZE  dcSize = { 480, 480 };

	//BLENDFUNCTION blend = {};
	//blend.SourceConstantAlpha = 0;
	//blend.AlphaFormat = AC_SRC_ALPHA;

	//UPDATELAYEREDWINDOWINFO info = {};
	//info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
	//info.pptSrc = NULL;//&sourcePosition;
	//info.pptDst = NULL;//&windowPosition;
	//info.psize = NULL;//&dcSize;
	//info.pblend = &blend;
	//info.dwFlags = ULW_ALPHA; 
	//info.crKey = RGB(0,0,0); 
	//info.dwFlags = ULW_COLORKEY;
    //nfo.hdcSrc = GetWindowDC(m_hWnd);
	//info.hdcSrc = m_hWndDC;
	
	//UpdateLayeredWindowIndirect(m_hWnd, &info);

	//HDC dstDC = GetWindowDC(m_hWnd);
	//HDC srcDC = CreateCompatibleDC(m_hWndDC);
	//UpdateLayeredWindow(m_hWnd, NULL/*hdcDst*/, NULL/*pptDst*/, NULL/*psize*/, srcDC, NULL/*pptSrc*/, RGB(0,0,0), &blend, ULW_COLORKEY );

	//Rendering finish.
	hr = m_pDCRenderTarget->EndDraw();

	//RECT rcWin = {0};
	//GetWindowRect(m_hWnd,&rcWin);
	//POINT ptw = {rcWin.left,rcWin.top};
	//SIZE pts = {rcWin.right-rcWin.left,rcWin.bottom-rcWin.top};
	//POINT ptsrc = {0};
	//HDC ScreenDC = GetDC(0);
	//UpdateLayeredWindow(m_hWnd, ScreenDC, &ptw, &pts, MemDC, &ptsrc, 0, &blend, ULW_ALPHA);

	//ReleaseDC(0,ScreenDC);
	//RenderTargetDC dc(m_interopTarget);
	
	if(hr == D2DERR_RECREATE_TARGET)
	{// The caller needs to re-create the render target then attempt to render the frame again
		DiscardDeviceResources();
	}
}

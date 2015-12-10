#include "stdafx.h"

using namespace D2D1;

template<class Type>
inline void SafeRelease(Type& pObjToRelease)
{
	if(pObjToRelease)
	{
		pObjToRelease->Release();
		pObjToRelease = 0;
	}
}

class FrameWnd
{
public:
	FrameWnd();
	~FrameWnd();
	HRESULT Initialize(HINSTANCE hInstance);
	
	void RunMessageLoop();
	HWND GetWndHandle();
protected:
	HRESULT						CreateDeviceIndependentResources();
	HRESULT						CreateDeviceResources();
	void						DiscardDeviceResources();
	void						OnRender(const PAINTSTRUCT &ps);
	void						OnResize(UINT uWidth, UINT uHeight);
	static LRESULT CALLBACK     WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
private:
	HWND						m_hWnd;
	HDC 						m_hWndDC;
	ID2D1Factory*				m_pD2DFactory;
	ID2D1HwndRenderTarget*		m_pRenderTarget;
    ID2D1SolidColorBrush*		m_pSolidBrush;

	//use ID2D1DCRenderTarget when drawing on the window instead of 
	//ID2D1HwndRenderTarget. if use the ID2D1HwndRenderTarget, the
	//background of the window will not be transparent even though
	//set the layered-style window and set colorkey with function
	//SetLayeredWindowAttributes
	ID2D1DCRenderTarget*		m_pDCRenderTarget;
	//ID2D1GdiInteropRenderTarget m_pGdiRenderTarget;
};

class ModalWnd:public FrameWnd
{
public:
protected:
private:
};

class ModelessWnd:public FrameWnd
{
public:
protected:
private:
};

class MenuWnd:public FrameWnd
{
public:
protected:
private:
};
class TipsWnd:public FrameWnd
{
public:
protected:
private:
};
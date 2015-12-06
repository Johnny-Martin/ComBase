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
	FrameWnd(){};
	~FrameWnd(){
		SafeRelease(m_pD2DFactory);
		SafeRelease(m_pRenderTarget);
	};
	bool	Initialize(HINSTANCE hInstance);
	//
	// 消息循环.
	//
	void RunMessageLoop()
	{
		MSG message;

		while(GetMessage(&message, NULL, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
protected:
	BOOL CreateDeviceIndependentResources();		//创建设备无关资源.
	BOOL CreateDeviceDependentResources();			//创建设备相关资源.

	void	OnRender();
	void	OnResize(UINT uWidth, UINT uHeight);
	static  LRESULT CALLBACK WndProc(	//消息过程函数.
		HWND   hWnd,
		UINT   uMessage,
		WPARAM wParam,
		LPARAM lParam
		);
private:
	HWND						m_hWnd;				//窗口句柄.
	HDC 						m_hWndDC;				//窗口句柄.
	ID2D1Factory*				m_pD2DFactory;		//D2D对象工厂.
	ID2D1HwndRenderTarget*		m_pRenderTarget;	//绘制目标区域.
	
	//use ID2D1DCRenderTarget when drawing on the window instead of 
	//ID2D1HwndRenderTarget. if use the ID2D1HwndRenderTarget, the
	//background of the window will not be transparent even though
	//set the layered-style window and set colorkey with function
	//SetLayeredWindowAttributes
	ID2D1DCRenderTarget*		m_pDCRenderTarget;	//绘制目标区域.
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
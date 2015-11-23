#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

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
	void DiscardDeviceDependentResources();			//丢弃设备资源.
	void DiscardDeviceIndependentResources();		//丢弃设备无关资源.

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
	ID2D1Factory*				m_pD2DFactory;		//D2D对象工厂.
	ID2D1HwndRenderTarget*		m_pRenderTarget;	//绘制目标区域.

	ID2D1SolidColorBrush*		m_pSolidBrush;		//纯色画刷.
	ID2D1LinearGradientBrush*	m_pLGBrush;			//线性渐变画刷.
	ID2D1RadialGradientBrush*	m_pRGBrush;			//放射状渐变画刷.
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
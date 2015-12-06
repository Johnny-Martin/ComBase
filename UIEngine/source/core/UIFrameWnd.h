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
	// ��Ϣѭ��.
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
	BOOL CreateDeviceIndependentResources();		//�����豸�޹���Դ.
	BOOL CreateDeviceDependentResources();			//�����豸�����Դ.

	void	OnRender();
	void	OnResize(UINT uWidth, UINT uHeight);
	static  LRESULT CALLBACK WndProc(	//��Ϣ���̺���.
		HWND   hWnd,
		UINT   uMessage,
		WPARAM wParam,
		LPARAM lParam
		);
private:
	HWND						m_hWnd;				//���ھ��.
	HDC 						m_hWndDC;				//���ھ��.
	ID2D1Factory*				m_pD2DFactory;		//D2D���󹤳�.
	ID2D1HwndRenderTarget*		m_pRenderTarget;	//����Ŀ������.
	
	//use ID2D1DCRenderTarget when drawing on the window instead of 
	//ID2D1HwndRenderTarget. if use the ID2D1HwndRenderTarget, the
	//background of the window will not be transparent even though
	//set the layered-style window and set colorkey with function
	//SetLayeredWindowAttributes
	ID2D1DCRenderTarget*		m_pDCRenderTarget;	//����Ŀ������.
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
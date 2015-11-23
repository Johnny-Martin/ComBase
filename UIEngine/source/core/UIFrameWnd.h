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
	void DiscardDeviceDependentResources();			//�����豸��Դ.
	void DiscardDeviceIndependentResources();		//�����豸�޹���Դ.

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
	ID2D1Factory*				m_pD2DFactory;		//D2D���󹤳�.
	ID2D1HwndRenderTarget*		m_pRenderTarget;	//����Ŀ������.

	ID2D1SolidColorBrush*		m_pSolidBrush;		//��ɫ��ˢ.
	ID2D1LinearGradientBrush*	m_pLGBrush;			//���Խ��仭ˢ.
	ID2D1RadialGradientBrush*	m_pRGBrush;			//����״���仭ˢ.
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
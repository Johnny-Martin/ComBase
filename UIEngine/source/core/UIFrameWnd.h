#ifndef UIFRAMEWND_H
#define UIFRAMEWND_H

#include "stdafx.h"
#include <list>
#include <xstring>

using namespace D2D1;
using namespace std;

template<class Type>
inline void SafeRelease(Type& pObjToRelease)
{
	if(pObjToRelease)
	{
		pObjToRelease->Release();
		pObjToRelease = 0;
	}
}


/////////////////CBaseWnd version 0.1 ////////////
// create window object from xml file
// the attribute in xml saved in map as a class member
// the attribute include:
// left, top, right, bottom, leftexp, topexp, rightexp, 
// bottomexp,title, visible, enable,
// topmost, layered, appwnd, blur, maxenable, minenable
// rootobjectid, 
//
class CBaseWnd
{
public:
	CBaseWnd(){ InitAttrMap(); }
	bool SetAttr(string key, string value);
	bool GetAttr(string key, string* value);
	static bool CheckAttrName(string strName){ return (m_attrNameSet.end() != m_attrNameSet.find(strName)) ? true:false;}
	static bool CheckEventName(string strName){ return (m_attrNameSet.end() != m_eventNameSet.find(strName)) ? true:false;}
private:
	static set<string> InitAttrNameSet();
	static set<string> InitEventNameSet();

	static set<string> m_attrNameSet;
	static set<string> m_eventNameSet;

	bool InitAttrMap();
	map<string, string> m_attrMap;
	map<string, string> m_eventMap;	
};

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

#endif
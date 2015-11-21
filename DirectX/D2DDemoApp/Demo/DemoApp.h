/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: DemoApp.h
    
    Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.9.9

    Module Name: CD2DDemoApp.

    Description: 实现一个通用的类CD2DDemoApp.

\*=========================================================================*/

#pragma once


#include "Required.h"
using namespace D2D1;

//
// 类体定义.
//
class CD2DDemoApp
{
//
// 构造与析构.
//
public:
	CD2DDemoApp();
	virtual ~CD2DDemoApp();


//
// 对外接口.
//
	BOOL	Initialize(					//完成程序的初始化.
		HINSTANCE hInstance
		);

	void	RunMessageLoop();			//运行消息循环.

	HWND	GetHwnd() const;			//获取句柄.


//
// 内部接口.
//
private:
	void	OnRender();					//执行绘制.

	void	OnResize(					//重置了窗口大.
		UINT uWidth,
		UINT uHeight
		);

	static LRESULT CALLBACK WndProc(	//消息过程函数.
		HWND   hWnd,
		UINT   uMessage,
		WPARAM wParam,
		LPARAM lParam
		);

	BOOL CreateDeviceIndependentResources();		//创建设备无关资源.
	BOOL CreateDeviceDependentResources();			//创建设备相关资源.
	void DiscardDeviceDependentResources();			//丢弃设备资源.
	void DiscardDeviceIndependentResources();		//丢弃设备无关资源.


//
// 私有数据.
//
private:
	HWND						m_hWnd;				//窗口句柄.
	ID2D1Factory*				m_pD2DFactory;		//D2D对象工厂.
	ID2D1HwndRenderTarget*		m_pRenderTarget;	//绘制目标区域.

	ID2D1SolidColorBrush*		m_pSolidBrush;		//纯色画刷.
	ID2D1LinearGradientBrush*	m_pLGBrush;			//线性渐变画刷.
	ID2D1RadialGradientBrush*	m_pRGBrush;			//放射状渐变画刷.
};

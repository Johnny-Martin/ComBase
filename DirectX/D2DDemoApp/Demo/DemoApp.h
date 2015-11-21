/*=========================================================================*\

    Copyright (c) YaFengZhang.  All rights reserved.

    File: DemoApp.h
    
    Email: zyfgood12@163.com  798446303@qq.com

	Time: 2010.9.9

    Module Name: CD2DDemoApp.

    Description: ʵ��һ��ͨ�õ���CD2DDemoApp.

\*=========================================================================*/

#pragma once


#include "Required.h"
using namespace D2D1;

//
// ���嶨��.
//
class CD2DDemoApp
{
//
// ����������.
//
public:
	CD2DDemoApp();
	virtual ~CD2DDemoApp();


//
// ����ӿ�.
//
	BOOL	Initialize(					//��ɳ���ĳ�ʼ��.
		HINSTANCE hInstance
		);

	void	RunMessageLoop();			//������Ϣѭ��.

	HWND	GetHwnd() const;			//��ȡ���.


//
// �ڲ��ӿ�.
//
private:
	void	OnRender();					//ִ�л���.

	void	OnResize(					//�����˴��ڴ�.
		UINT uWidth,
		UINT uHeight
		);

	static LRESULT CALLBACK WndProc(	//��Ϣ���̺���.
		HWND   hWnd,
		UINT   uMessage,
		WPARAM wParam,
		LPARAM lParam
		);

	BOOL CreateDeviceIndependentResources();		//�����豸�޹���Դ.
	BOOL CreateDeviceDependentResources();			//�����豸�����Դ.
	void DiscardDeviceDependentResources();			//�����豸��Դ.
	void DiscardDeviceIndependentResources();		//�����豸�޹���Դ.


//
// ˽������.
//
private:
	HWND						m_hWnd;				//���ھ��.
	ID2D1Factory*				m_pD2DFactory;		//D2D���󹤳�.
	ID2D1HwndRenderTarget*		m_pRenderTarget;	//����Ŀ������.

	ID2D1SolidColorBrush*		m_pSolidBrush;		//��ɫ��ˢ.
	ID2D1LinearGradientBrush*	m_pLGBrush;			//���Խ��仭ˢ.
	ID2D1RadialGradientBrush*	m_pRGBrush;			//����״���仭ˢ.
};

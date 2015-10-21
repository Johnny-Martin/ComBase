/*----------------------------------------------------------------
【Cpp文件】：main.cpp	    Create by Beyond Ray,2014年2月
（描述）：简单的GUI界面实现
------------------------------------------------------------------*/

//【头文件】
#include"D3DComMacro.h"
#include"GUIClass.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//【库文件】
#pragma comment (lib, "winmm.lib")                                     //为调用 PlaySound 函数
#pragma comment (lib, "d3d9.lib")								       //Direct3D9相关库文件	
#pragma comment (lib, "d3dx9.lib")									   //ID3DX相关库文件
#pragma comment (lib, "dxguid.lib")									   //GUID相关识别
#pragma comment (lib, "dinput8.lib")								   //DirectInput相关库文件

//【宏定义】
#define WINDOW_TITLE	L"致我们永不熄灭的游戏梦想----【我爱杨】"      //窗口标题

//【全局变量声明】
TCHAR		lpClassName[] = TEXT("GDIDemo");						   //声明类名
HINSTANCE   g_hInst = NULL;											   //实例句柄
LPDIRECT3DDEVICE9	g_pd3dDevice9 = NULL;							   //Direct3D设备对象(COM接口对象）

LPD3DXFONT	g_pFont =  NULL;										   //字体对象（COM接口对象)

wchar_t		g_str[50];												   //用于输出的字符串数组
int			g_cxClient = 1280;										   //窗口客户区的宽度
int			g_cyClient = 775;										   //窗口客户区的高度

//-----------------------------------【GUI相关的全局变量声明】---------------------------------------
int			g_cxMouse = 0;											   //鼠标指针X位置
int			g_cyMouse = 0;											   //鼠标指针Y位置
BOOL		g_bLMBDown = false;										   //判断鼠标左键是否按下的标志

GUIClass	* g_pGUIMain = NULL;
GUIClass	* g_pGUIStart = NULL;
GUIClass	* g_pGUILoad = NULL;
GUIClass	* g_pGUIOption = NULL;									   //4个GUI类的对象

int			g_currentGUI = GUI_BKGROUND_MAIN;						   //默认的GUI标识
WORD		g_GUIMainFont = -1; 									   //GUI字体对象的索引
//--------------------------------------------------------------------------------------------------

//【全局函数声明】
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				   //窗口过程函数
HRESULT Direct3D_Init(HWND hwnd);									   //在该函数中进行Direct3D初始化
HRESULT Object_Init(HWND hwnd);										   //在该函数中进行绘制物体资源的初始化
VOID  Direct3D_Render(HWND hwnd);						               //在该函数中进行Direct3D的渲染
VOID  Direct3D_CleanUp();											   //在该函数中进行COM资源及其他资源的清理
VOID  HelpText_Render(HWND);										   //在该函数中渲染帮助文本
VOID  CALLBACK GUICallBack(int Id, int State);						   //GUI回调函数
VOID  Light_Set(LPDIRECT3DDEVICE9, int);							   //在该函数中进行光源的设置
VOID  ClipClient(HWND hwnd);										   //该函数将限制鼠标在客户区中
float GetFPS();														   //得到每秒的帧率（Frame Per Second)	

//----------------------------------------【WinMain主函数】-------------------------------------------
//  描述：Windows程序的入口函数，程序从这里开始
//----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND	   hwnd;
	MSG		   msg = { 0 };

	g_hInst = hInstance;
	//【设计窗口类】

	wndclass.cbSize = sizeof (WNDCLASSEX);								//结构大小
	wndclass.style = CS_HREDRAW | CS_VREDRAW;							//窗口风格样式
	wndclass.lpfnWndProc = WndProc;										//设置窗口过程地址
	wndclass.cbClsExtra = 0;											//额外的窗口类内存空间
	wndclass.cbWndExtra = 0;											//额外的窗口内存空间
	wndclass.hInstance = hInstance;										//窗口的实例句柄
	wndclass.hIcon = (HICON)::LoadImage(NULL, L"pictures/icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//加载图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);						//加载鼠标指针
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//加载窗口背景
	wndclass.lpszMenuName = NULL;										//窗口菜单名称
	wndclass.lpszClassName = lpClassName;								//窗口类名称
	wndclass.hIconSm = 0;												//任务栏中的小图标

	//【注册窗口类】

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("窗口类注册失败"), lpClassName, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//【创建窗口】

	hwnd = CreateWindow(lpClassName, WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, g_cxClient, g_cyClient,
		NULL, NULL, hInstance, NULL);                                  //父窗口句柄，菜单句柄，实例句柄，额外字段


	if (S_OK != Direct3D_Init(hwnd))
	{
		MessageBox(hwnd, _T("Direct3D初始化资源失败！！"), _T("我爱杨的消息窗口"), MB_OK);
		return E_FAIL;
	}

	//【移动,显示和更新窗口】

	MoveWindow(hwnd, 0, 0, g_cxClient, g_cyClient, TRUE);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//播放背景音乐
	PlaySound(_T("GUI/天下贰 - 太古铜门.wav"), NULL, SND_ASYNC | SND_FILENAME);

	//进入游戏消息循环

	static FLOAT fCurrentTime = (float)timeGetTime();
	static FLOAT fLastTime = (float)timeGetTime();
	static FLOAT fElapsedTime = 0.0f;

	while (msg.message != WM_QUIT)
	{

		fLastTime = fCurrentTime;
		fCurrentTime = (float)timeGetTime();
		fElapsedTime = (fCurrentTime - fLastTime) / 1000.0f;

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Direct3D_Render(hwnd);
		}
	}

	//注销窗口类
	UnregisterClass(lpClassName, wndclass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_SIZE:
		g_cxClient = LOWORD(lParam);
		g_cyClient = HIWORD(lParam);
		if (g_cxClient == 0 || g_cyClient == 0)
			return 0;
		ClipClient(hwnd);
		return 0;

	case WM_MOUSEMOVE:
		g_cxMouse = LOWORD(lParam) + GetSystemMetrics(SM_CXBORDER);
		g_cyMouse = HIWORD(lParam) + GetSystemMetrics(SM_CYCAPTION);
		return 0;

	case WM_LBUTTONDOWN:
		g_bLMBDown = TRUE;
		return 0;

	case WM_LBUTTONUP:
		g_bLMBDown = FALSE;
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		return 0;

	case WM_PAINT:
		Direct3D_Render(hwnd);
		ValidateRect(hwnd, NULL);
		return 0;

	case WM_DESTROY:
		Direct3D_CleanUp();
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

}
//--------------------------------------------【Direct3D_Init】---------------------------------------------
//  描述:在该函数中进行Direct3D初始化
//----------------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// 【创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9		pD3D9;										//创建Direct3D接口对象
	if (FAILED(pD3D9 = Direct3DCreate9(D3D_SDK_VERSION)))	    //初始化Direct3D接口对象，并与DirectX_SDK版本协商，
		return E_FAIL;											//以便包含的头文件与DirectX Runtime匹配

	//--------------------------------------------------------------------------------------
	// 【取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;				//支持硬件顶点运算方式
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;				//支持软件顶点运算方式

	//--------------------------------------------------------------------------------------
	// 【填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = g_cxClient;							//后台缓冲区宽度
	d3dpp.BackBufferHeight = g_cyClient;						//后台缓冲区高度
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					//后台缓冲区保存像素格式
	d3dpp.BackBufferCount = 2;									//后台缓冲区数量
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				//多重采样的类型
	d3dpp.MultiSampleQuality = 0;								//多重采样的格式
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//指定如何将后台缓冲区内容复制到前台的缓存中
	d3dpp.hDeviceWindow = hwnd;									//当前与设备关联的窗口，被激活的窗口
	d3dpp.Windowed = true;										//对激活窗口表示是否使用窗口模式
	d3dpp.EnableAutoDepthStencil = true;						//自动管理深度缓存，模版缓存
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//深度缓存，模版缓存格式，(这里为24位深度，8位模版)
	d3dpp.Flags = 0;											//附加属性
	d3dpp.FullScreen_RefreshRateInHz = 0;						//全屏模式时的屏幕刷新率
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //后台缓冲区和前台缓冲区的最大交换频率


	//--------------------------------------------------------------------------------------
	// 【创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------

	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &g_pd3dDevice9)))
	{
		SAFE_RELEASE(pD3D9);
		return E_FAIL;
	}
	SAFE_RELEASE(pD3D9);		//Direct3D接口对象使命完成，我们将其释放掉


	if (S_OK != Object_Init(hwnd))
		return E_FAIL;

	return S_OK;
}
//--------------------------------------------【Object_Init】------------------------------------------------
//  描述：在该函数中进行绘制物体资源的初始化
//-----------------------------------------------------------------------------------------------------------
HRESULT Object_Init(HWND hwnd)
{
	
	//创建字体
	HR(D3DXCreateFont(g_pd3dDevice9, 36, 0, 0, 0, FALSE, DEFAULT_CHARSET,     //创建微软雅黑字体
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Clambria"), &g_pFont));
	
	srand(GetTickCount());	//用操作系统启动到现在的时间来初始化随机种子

//----------------------------------------【GUI相关资源的添加】----------------------------------------------
	//GUI对象的初始化
	g_pGUIMain = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIStart = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUILoad = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIOption = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);

	//添加GUI字体对象
	g_pGUIMain->CreateTextFont(_T("微软雅黑"), 36, &g_GUIMainFont);

	//添加背景图片
	if (!g_pGUIMain->AddBackground(TEXT("GUI/main.jpg")))	return E_FAIL;
	if (!g_pGUIStart->AddBackground(TEXT("GUI/startgame.jpg")))	return E_FAIL;
	if (!g_pGUILoad->AddBackground(TEXT("GUI/loadgame.jpg")))	return E_FAIL;
	if (!g_pGUIOption->AddBackground(TEXT("GUI/option.jpg")))	return E_FAIL;


	//---------------------------------【主菜单main页面相关的页面布局代码】---------------------------------
	// 添加静态文本到页面中
	if (!g_pGUIMain->AddStaticText(TEXT("Version 我爱杨V2.0倾情制造版"), GUI_STATIC_TEXT,
		D3DCOLOR_RGBA(255, 255, 255, 210), 850, 720, g_GUIMainFont)) return false;

	// 添加4个按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if (!g_pGUIMain->AddButton(GUI_BUTTON_START, 585, 340, TEXT("GUI\\startDown.png"),
		TEXT("GUI\\StartUp.png"), TEXT("GUI\\startOver.png"))) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_LOAD, 585, 385, TEXT("GUI\\loadDown.png"),
		TEXT("GUI\\loadUp.png"),TEXT("GUI\\loadOver.png"))) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_OPTION, 585, 430, L"GUI\\optionsDown.png",
		L"GUI\\optionsUp.png", L"GUI\\optionsOver.png")) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_QUIT, 585, 475, L"GUI\\quitDown.png",
		L"GUI\\quitUp.png", L"GUI\\quitOver.png")) return false;


	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------
	// 添加按钮到页面中

	if (!g_pGUIStart->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;

	if (!g_pGUIStart->AddButton(GUI_BUTTON_BACK, 300, 200, L"GUI/飞行模式down.png", L"GUI/飞行模式up.png",
		L"GUI/飞行模式over.png")) return false;

	//------------------------【载入游戏load页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_pGUILoad->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;


	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_pGUIOption->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;
	
	return S_OK;
	
}


//--------------------------------------------【Direct3D_Render】--------------------------------------------
//  描述：在该函数中进行Direct3D的渲染
//-----------------------------------------------------------------------------------------------------------
VOID Direct3D_Render(HWND hwnd)
{
	//----------------------------------------------------------------------------------
	//【清屏操作】
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 100, 200), 1.0f, 0);

	//----------------------------------------------------------------------------------
	//【开始场景】
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->BeginScene();

	//----------------------------------------------------------------------------------
	//【正式绘制】
	//----------------------------------------------------------------------------------
	switch (g_currentGUI)
	{
	case GUI_BKGROUND_MAIN:
		ProcessGUI(g_pGUIMain, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_START:
		ProcessGUI(g_pGUIStart, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_LOAD:
		ProcessGUI(g_pGUILoad, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;

	case GUI_BKGROUND_OPTION:
		ProcessGUI(g_pGUIOption, g_bLMBDown, g_cxMouse, g_cyMouse, GUICallBack);
		break;
	}

	//渲染帮助文本：
	HelpText_Render(hwnd);

	//----------------------------------------------------------------------------------
	//【结束场景】
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->EndScene();

	//----------------------------------------------------------------------------------
	//【翻转与显示】
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Present(NULL, NULL, NULL, NULL);
	//源矩形区域，目标矩形区域，当前绘制的窗口，最小更新区域

}

//--------------------------------------------【Game_CleanUp】------------------------------------------------
//  描述：在该函数中进行COM资源及其他资源的清理
//------------------------------------------------------------------------------------------------------------
VOID Direct3D_CleanUp()
{
	SAFE_RELEASE(g_pd3dDevice9);			//释放Direct3D设备接口
	SAFE_RELEASE(g_pFont);					//释放字体接口对象
}

//----------------------------------------【HelpText_Render()函数】-------------------------------------------
// Desc:帮助文本信息的输出
//------------------------------------------------------------------------------------------------------------
VOID HelpText_Render(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	//在窗口右上角输出帧率
	int icharCount = swprintf_s(g_str, 20, _T("FPS:%0.3f"), GetFPS());
	g_pFont->DrawText(NULL, g_str, icharCount, &rect, DT_RIGHT | DT_TOP, D3DCOLOR_XRGB(255, 10, 10));

}

//-----------------------------------------------【GUICallBack()函数】----------------------------------------
// Desc:GUI回调函数（更改GUI对象标识和鼠标状态）
//------------------------------------------------------------------------------------------------------------
VOID CALLBACK GUICallBack(int Id, int State)
{
	switch (Id)
	{
	case GUI_BUTTON_BACK:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_MAIN;
		break;

	case GUI_BUTTON_START:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_START;
		break;

	case GUI_BUTTON_LOAD:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_LOAD;
		break;

	case GUI_BUTTON_OPTION:
		if (State == GUI_LBUTTON_DOWN)
			g_currentGUI = GUI_BKGROUND_OPTION;
		break;

	case GUI_BUTTON_QUIT:
		if (State == GUI_LBUTTON_DOWN)
			PostQuitMessage(0);
		break;
	}
}
//-----------------------------------------------【ClipClient()函数】-----------------------------------------
// Desc:该函数将限制鼠标在客户区中
//------------------------------------------------------------------------------------------------------------
VOID ClipClient(HWND hwnd)
{
	POINT lt, rb;
	RECT rect;
	GetClientRect(hwnd, &rect);
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);

	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;

	ClipCursor(&rect);
	//ShowCursor(false);
}

//--------------------------------------------【GetFPS()函数】-----------------------------------------------
//  描述:计算每秒画面刷新的帧数
//-----------------------------------------------------------------------------------------------------------
float GetFPS()
{
	static float fps = 0;					//每秒的帧率
	static int	 iframeCount = 0;			//图形绘制的帧数
	static DWORD dwlastTime = 0;			//上次获取的时间
	static DWORD dwcurrentTime;				//当前获取的时间

	iframeCount++;

	dwcurrentTime = timeGetTime();

	if (dwcurrentTime - dwlastTime >= 1000.0f)
	{
		fps = (float)iframeCount * 1000.0f / (dwcurrentTime - dwlastTime);
		dwlastTime = dwcurrentTime;
		iframeCount = 0;
	}

	return fps;
}
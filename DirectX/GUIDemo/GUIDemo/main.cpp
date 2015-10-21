/*----------------------------------------------------------------
��Cpp�ļ�����main.cpp	    Create by Beyond Ray,2014��2��
�����������򵥵�GUI����ʵ��
------------------------------------------------------------------*/

//��ͷ�ļ���
#include"D3DComMacro.h"
#include"GUIClass.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//�����ļ���
#pragma comment (lib, "winmm.lib")                                     //Ϊ���� PlaySound ����
#pragma comment (lib, "d3d9.lib")								       //Direct3D9��ؿ��ļ�	
#pragma comment (lib, "d3dx9.lib")									   //ID3DX��ؿ��ļ�
#pragma comment (lib, "dxguid.lib")									   //GUID���ʶ��
#pragma comment (lib, "dinput8.lib")								   //DirectInput��ؿ��ļ�

//���궨�塿
#define WINDOW_TITLE	L"����������Ϩ�����Ϸ����----���Ұ��"      //���ڱ���

//��ȫ�ֱ���������
TCHAR		lpClassName[] = TEXT("GDIDemo");						   //��������
HINSTANCE   g_hInst = NULL;											   //ʵ�����
LPDIRECT3DDEVICE9	g_pd3dDevice9 = NULL;							   //Direct3D�豸����(COM�ӿڶ���

LPD3DXFONT	g_pFont =  NULL;										   //�������COM�ӿڶ���)

wchar_t		g_str[50];												   //����������ַ�������
int			g_cxClient = 1280;										   //���ڿͻ����Ŀ��
int			g_cyClient = 775;										   //���ڿͻ����ĸ߶�

//-----------------------------------��GUI��ص�ȫ�ֱ���������---------------------------------------
int			g_cxMouse = 0;											   //���ָ��Xλ��
int			g_cyMouse = 0;											   //���ָ��Yλ��
BOOL		g_bLMBDown = false;										   //�ж��������Ƿ��µı�־

GUIClass	* g_pGUIMain = NULL;
GUIClass	* g_pGUIStart = NULL;
GUIClass	* g_pGUILoad = NULL;
GUIClass	* g_pGUIOption = NULL;									   //4��GUI��Ķ���

int			g_currentGUI = GUI_BKGROUND_MAIN;						   //Ĭ�ϵ�GUI��ʶ
WORD		g_GUIMainFont = -1; 									   //GUI������������
//--------------------------------------------------------------------------------------------------

//��ȫ�ֺ���������
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				   //���ڹ��̺���
HRESULT Direct3D_Init(HWND hwnd);									   //�ڸú����н���Direct3D��ʼ��
HRESULT Object_Init(HWND hwnd);										   //�ڸú����н��л���������Դ�ĳ�ʼ��
VOID  Direct3D_Render(HWND hwnd);						               //�ڸú����н���Direct3D����Ⱦ
VOID  Direct3D_CleanUp();											   //�ڸú����н���COM��Դ��������Դ������
VOID  HelpText_Render(HWND);										   //�ڸú�������Ⱦ�����ı�
VOID  CALLBACK GUICallBack(int Id, int State);						   //GUI�ص�����
VOID  Light_Set(LPDIRECT3DDEVICE9, int);							   //�ڸú����н��й�Դ������
VOID  ClipClient(HWND hwnd);										   //�ú�������������ڿͻ�����
float GetFPS();														   //�õ�ÿ���֡�ʣ�Frame Per Second)	

//----------------------------------------��WinMain��������-------------------------------------------
//  ������Windows�������ں�������������￪ʼ
//----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	HWND	   hwnd;
	MSG		   msg = { 0 };

	g_hInst = hInstance;
	//����ƴ����ࡿ

	wndclass.cbSize = sizeof (WNDCLASSEX);								//�ṹ��С
	wndclass.style = CS_HREDRAW | CS_VREDRAW;							//���ڷ����ʽ
	wndclass.lpfnWndProc = WndProc;										//���ô��ڹ��̵�ַ
	wndclass.cbClsExtra = 0;											//����Ĵ������ڴ�ռ�
	wndclass.cbWndExtra = 0;											//����Ĵ����ڴ�ռ�
	wndclass.hInstance = hInstance;										//���ڵ�ʵ�����
	wndclass.hIcon = (HICON)::LoadImage(NULL, L"pictures/icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//����ͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);						//�������ָ��
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//���ش��ڱ���
	wndclass.lpszMenuName = NULL;										//���ڲ˵�����
	wndclass.lpszClassName = lpClassName;								//����������
	wndclass.hIconSm = 0;												//�������е�Сͼ��

	//��ע�ᴰ���ࡿ

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("������ע��ʧ��"), lpClassName, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//���������ڡ�

	hwnd = CreateWindow(lpClassName, WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, g_cxClient, g_cyClient,
		NULL, NULL, hInstance, NULL);                                  //�����ھ�����˵������ʵ������������ֶ�


	if (S_OK != Direct3D_Init(hwnd))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ����Դʧ�ܣ���"), _T("�Ұ������Ϣ����"), MB_OK);
		return E_FAIL;
	}

	//���ƶ�,��ʾ�͸��´��ڡ�

	MoveWindow(hwnd, 0, 0, g_cxClient, g_cyClient, TRUE);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//���ű�������
	PlaySound(_T("GUI/���·� - ̫��ͭ��.wav"), NULL, SND_ASYNC | SND_FILENAME);

	//������Ϸ��Ϣѭ��

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

	//ע��������
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
//--------------------------------------------��Direct3D_Init��---------------------------------------------
//  ����:�ڸú����н���Direct3D��ʼ��
//----------------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// �����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9		pD3D9;										//����Direct3D�ӿڶ���
	if (FAILED(pD3D9 = Direct3DCreate9(D3D_SDK_VERSION)))	    //��ʼ��Direct3D�ӿڶ��󣬲���DirectX_SDK�汾Э�̣�
		return E_FAIL;											//�Ա������ͷ�ļ���DirectX Runtimeƥ��

	//--------------------------------------------------------------------------------------
	// ��ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;				//֧��Ӳ���������㷽ʽ
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;				//֧������������㷽ʽ

	//--------------------------------------------------------------------------------------
	// �������ݡ������D3DPRESENT_PARAMETERS�ṹ��
	//--------------------------------------------------------------------------------------

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = g_cxClient;							//��̨���������
	d3dpp.BackBufferHeight = g_cyClient;						//��̨�������߶�
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					//��̨�������������ظ�ʽ
	d3dpp.BackBufferCount = 2;									//��̨����������
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				//���ز���������
	d3dpp.MultiSampleQuality = 0;								//���ز����ĸ�ʽ
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ָ����ν���̨���������ݸ��Ƶ�ǰ̨�Ļ�����
	d3dpp.hDeviceWindow = hwnd;									//��ǰ���豸�����Ĵ��ڣ�������Ĵ���
	d3dpp.Windowed = true;										//�Լ���ڱ�ʾ�Ƿ�ʹ�ô���ģʽ
	d3dpp.EnableAutoDepthStencil = true;						//�Զ�������Ȼ��棬ģ�滺��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//��Ȼ��棬ģ�滺���ʽ��(����Ϊ24λ��ȣ�8λģ��)
	d3dpp.Flags = 0;											//��������
	d3dpp.FullScreen_RefreshRateInHz = 0;						//ȫ��ģʽʱ����Ļˢ����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //��̨��������ǰ̨����������󽻻�Ƶ��


	//--------------------------------------------------------------------------------------
	// �����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------

	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &g_pd3dDevice9)))
	{
		SAFE_RELEASE(pD3D9);
		return E_FAIL;
	}
	SAFE_RELEASE(pD3D9);		//Direct3D�ӿڶ���ʹ����ɣ����ǽ����ͷŵ�


	if (S_OK != Object_Init(hwnd))
		return E_FAIL;

	return S_OK;
}
//--------------------------------------------��Object_Init��------------------------------------------------
//  �������ڸú����н��л���������Դ�ĳ�ʼ��
//-----------------------------------------------------------------------------------------------------------
HRESULT Object_Init(HWND hwnd)
{
	
	//��������
	HR(D3DXCreateFont(g_pd3dDevice9, 36, 0, 0, 0, FALSE, DEFAULT_CHARSET,     //����΢���ź�����
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Clambria"), &g_pFont));
	
	srand(GetTickCount());	//�ò���ϵͳ���������ڵ�ʱ������ʼ���������

//----------------------------------------��GUI�����Դ����ӡ�----------------------------------------------
	//GUI����ĳ�ʼ��
	g_pGUIMain = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIStart = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUILoad = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);
	g_pGUIOption = new GUIClass(g_pd3dDevice9, g_cxClient, g_cyClient);

	//���GUI�������
	g_pGUIMain->CreateTextFont(_T("΢���ź�"), 36, &g_GUIMainFont);

	//��ӱ���ͼƬ
	if (!g_pGUIMain->AddBackground(TEXT("GUI/main.jpg")))	return E_FAIL;
	if (!g_pGUIStart->AddBackground(TEXT("GUI/startgame.jpg")))	return E_FAIL;
	if (!g_pGUILoad->AddBackground(TEXT("GUI/loadgame.jpg")))	return E_FAIL;
	if (!g_pGUIOption->AddBackground(TEXT("GUI/option.jpg")))	return E_FAIL;


	//---------------------------------�����˵�mainҳ����ص�ҳ�沼�ִ��롿---------------------------------
	// ��Ӿ�̬�ı���ҳ����
	if (!g_pGUIMain->AddStaticText(TEXT("Version �Ұ���V2.0���������"), GUI_STATIC_TEXT,
		D3DCOLOR_RGBA(255, 255, 255, 210), 850, 720, g_GUIMainFont)) return false;

	// ���4����ť���ֱ��ǿ�ʼ��Ϸ��������ȣ�ѡ����˳���Ϸ��ÿ����ť��Ӧ3��ͼ
	if (!g_pGUIMain->AddButton(GUI_BUTTON_START, 585, 340, TEXT("GUI\\startDown.png"),
		TEXT("GUI\\StartUp.png"), TEXT("GUI\\startOver.png"))) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_LOAD, 585, 385, TEXT("GUI\\loadDown.png"),
		TEXT("GUI\\loadUp.png"),TEXT("GUI\\loadOver.png"))) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_OPTION, 585, 430, L"GUI\\optionsDown.png",
		L"GUI\\optionsUp.png", L"GUI\\optionsOver.png")) return false;

	if (!g_pGUIMain->AddButton(GUI_BUTTON_QUIT, 585, 475, L"GUI\\quitDown.png",
		L"GUI\\quitUp.png", L"GUI\\quitOver.png")) return false;


	//------------------------����ʼ����Ϸstartҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����

	if (!g_pGUIStart->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;

	if (!g_pGUIStart->AddButton(GUI_BUTTON_BACK, 300, 200, L"GUI/����ģʽdown.png", L"GUI/����ģʽup.png",
		L"GUI/����ģʽover.png")) return false;

	//------------------------��������Ϸloadҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_pGUILoad->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;


	//------------------------����Ϸ����optionҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_pGUIOption->AddButton(GUI_BUTTON_BACK, 750, 400, L"GUI/backDown.png", L"GUI/backUp.png",
		L"GUI/backOver.png")) return false;
	
	return S_OK;
	
}


//--------------------------------------------��Direct3D_Render��--------------------------------------------
//  �������ڸú����н���Direct3D����Ⱦ
//-----------------------------------------------------------------------------------------------------------
VOID Direct3D_Render(HWND hwnd)
{
	//----------------------------------------------------------------------------------
	//������������
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 100, 200), 1.0f, 0);

	//----------------------------------------------------------------------------------
	//����ʼ������
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->BeginScene();

	//----------------------------------------------------------------------------------
	//����ʽ���ơ�
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

	//��Ⱦ�����ı���
	HelpText_Render(hwnd);

	//----------------------------------------------------------------------------------
	//������������
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->EndScene();

	//----------------------------------------------------------------------------------
	//����ת����ʾ��
	//----------------------------------------------------------------------------------
	g_pd3dDevice9->Present(NULL, NULL, NULL, NULL);
	//Դ��������Ŀ��������򣬵�ǰ���ƵĴ��ڣ���С��������

}

//--------------------------------------------��Game_CleanUp��------------------------------------------------
//  �������ڸú����н���COM��Դ��������Դ������
//------------------------------------------------------------------------------------------------------------
VOID Direct3D_CleanUp()
{
	SAFE_RELEASE(g_pd3dDevice9);			//�ͷ�Direct3D�豸�ӿ�
	SAFE_RELEASE(g_pFont);					//�ͷ�����ӿڶ���
}

//----------------------------------------��HelpText_Render()������-------------------------------------------
// Desc:�����ı���Ϣ�����
//------------------------------------------------------------------------------------------------------------
VOID HelpText_Render(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	//�ڴ������Ͻ����֡��
	int icharCount = swprintf_s(g_str, 20, _T("FPS:%0.3f"), GetFPS());
	g_pFont->DrawText(NULL, g_str, icharCount, &rect, DT_RIGHT | DT_TOP, D3DCOLOR_XRGB(255, 10, 10));

}

//-----------------------------------------------��GUICallBack()������----------------------------------------
// Desc:GUI�ص�����������GUI�����ʶ�����״̬��
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
//-----------------------------------------------��ClipClient()������-----------------------------------------
// Desc:�ú�������������ڿͻ�����
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

//--------------------------------------------��GetFPS()������-----------------------------------------------
//  ����:����ÿ�뻭��ˢ�µ�֡��
//-----------------------------------------------------------------------------------------------------------
float GetFPS()
{
	static float fps = 0;					//ÿ���֡��
	static int	 iframeCount = 0;			//ͼ�λ��Ƶ�֡��
	static DWORD dwlastTime = 0;			//�ϴλ�ȡ��ʱ��
	static DWORD dwcurrentTime;				//��ǰ��ȡ��ʱ��

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
// Caller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <windows.h>
#include "XmlParser.h"
#include <assert.h>

#include "ResManager.h"
#include "UIFrameWnd.h"
//#include "zlib.h"

//#define PNG_DEBUG 3
//#define PNG_VERSION_INFO_ONLY
#include "png.h"

using namespace std;
//using namespace tinyxml2;

extern "C" int CCallLua ();

/*
int example_1()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("SampleWnd no bom.xml");
	//doc.Print()
	int errid = static_cast<int> (doc.ErrorID());
	if (0 != errid)
	{
		//std::cout<<"error: "<<
		doc.PrintError();
		return errid;
	}

	int v0 = 0;
	int v1 = 0;
	XMLElement* rootElement = doc.FirstChildElement();
	XMLElement* windowElement = rootElement->FirstChildElement("window");
	XMLElement* wndAttrElement = windowElement->FirstChildElement("attr");
	//traverse all child node about attr node
	//class CBaseWnd should have the ability of traversing all attribute in map;
	string attrName;
	string attrValue;
	CBaseWnd tmpWnd;
	for( XMLNode* ele = wndAttrElement->FirstChild(); ele; ele = ele->NextSibling())
	{
		attrName = ((XMLElement*)ele)->Value();
		attrValue = ((XMLElement*)ele)->GetText();
		if (CBaseWnd::CheckAttrName(attrName))
		{
			tmpWnd.SetAttr(attrName, attrValue);
		}
	}
	
	//cout<<tmpWnd.GetAttr()
	XMLElement* attrLayeredElement = wndAttrElement->FirstChildElement("layered");
	attrLayeredElement->QueryIntText( &v0 );

	//XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement( "textApproach" );
	//textApproachElement->FirstChildElement( "v" )->QueryIntText( &v1 );

	printf( "Both values are the same: %d and %d\n", v0, v1 );

	return errid;   be able to parse xml,but do not support self-closed label
}*/

void* GreateBaseWndObject()
{
	return (void*)new CBaseWnd();
}

void example_3()
{
	//register class information
	CObjectFactory& factortInstance = CObjectFactory::GetFactoryInstance();
	//factortInstance.RegistClass("CBaseWnd", GreateBaseWndObject);

	CBaseWnd* tmpWnd = (CBaseWnd*)factortInstance.CreateObjectByClassName("CBaseWnd");
	if (NULL == tmpWnd)
	{
		int i = 1;
	}else
	{
		int x= 1;
	}
	
}
void example_2()
{
	XMLFile xmlFile;
	XMLERROR ret = xmlFile.LoadXmlFile(_T("SampleWnd no bom.xml"));
	if (XML_SUCCESS == ret)
	{
		XMLabel* rootObj = xmlFile.GetRootObj();
		cout<<rootObj->GetLabelClassName()<<endl;

		//XMLabel* child = rootObj->
	}
}
void example_4()
{
	ResManager resManager(L"E:\\A iss\\凯子婚礼精选");
	RPicture *pic;
	resManager.GetResPicHandle("texturelist.nine.customRectBkg.3", &pic);
}

void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void read_png_file(char* file_name)
{
	unsigned char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		abort_("[read_png_file] File %s could not be opened for reading", file_name);
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	
	for (y=0; y<height; y++)
	{
		png_uint_32 size = png_get_rowbytes(png_ptr,info_ptr);
		row_pointers[y] = (png_byte*) malloc(size);
	}

	png_read_image(png_ptr, row_pointers);

	fclose(fp);
}

void write_png_file(char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, width, height,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	for (y=0; y<height; y++)
		free(row_pointers[y]);
	free(row_pointers);

	fclose(fp);
}

void process_file(void)
{
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
		abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA ""(lacks the alpha channel)");

	if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
		abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
		PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

	for (y=0; y<height; y++) 
	{
		png_byte* row = row_pointers[y];
		for (x=0; x<width; x++) {
			png_byte* ptr = &(row[x*4]);
			printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
				x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

			/* set red value to 0 and green value to the blue one */
			ptr[0] = 0;
			ptr[1] = ptr[2];
		}
	}
}

void example_5()//test for libpng
{
	//set resource folder,every plugin has it's own unique ResManager object
	ResManager resMgr(L"E:\\code\\ComBase\\trunk\\UIEngine\\docs");
	RPicture* picObj;
	//the first pic's index in piclist is '1' rather than '0'
	//is illegal to pass an piclist id without a index section at the end
	RESERROR resErr = resMgr.GetResPicHandle("texturelist.Nine.btnbkg.4", &picObj);
	
	picObj->WritePngFile(L"E:\\code\\ComBase\\trunk\\UIEngine\\docs\\testout.png");

	//delete picObj;
	//RPicList picObj(L"E:\\code\\ComBase\\trunk\\UIEngine\\docs\\texturelist.png");
	//picObj.ReadPngFile(L"E:\\code\\ComBase\\trunk\\UIEngine\\docs\\你好hover.png");
	//read_png_file("E:\\code\\ComBase\\trunk\\UIEngine\\docs\\texturelist.Nine.btnbkg.png");
	//process_file();
	//write_png_file("E:\\code\\ComBase\\trunk\\UIEngine\\docs\\texture_copy.png");
	int i = 1;
}

void example_6(HINSTANCE hInstance)
{
	if(!SUCCEEDED(CoInitialize(NULL)))
		return;
	
	FrameWnd Demo;
	HRESULT hr = Demo.Initialize(hInstance);
	if(SUCCEEDED(hr))
	{
		Demo.RunMessageLoop();
	}

	CoUninitialize();
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd)
//int _tmain(int argc, _TCHAR* argv[])
{
	//example_1();
	//example_2();
	//example_3();
	
	//example_4();
	//example_5();
	example_6(hInstance);
	return 0;
}



const wchar_t szAppName[] = L"异形窗口2 MoreWindows-(http://blog.csdn.net/MoreWindows)";  

/* 
* 函数名称: GetWindowSize 
* 函数功能: 得到窗口的宽高 
* hwnd      窗口句柄 
* pnWidth   窗口宽 
* pnHeight  窗口高 
*/  
void GetWindowSize(HWND hwnd, int *pnWidth, int *pnHeight);  


/* 
* 函数名称: InitBitmapWindow 
* 函数功能: 位图窗口初始化 
* hinstance 进程实例 
* nWidth    窗口宽 
* nHeight   窗口高 
* nCmdshow  显示方式-与ShowWindow函数的第二个参数相同 
*/  
BOOL InitBitmapWindow(HINSTANCE hinstance, int nWidth, int nHeight, int nCmdshow);  

// 位图窗口消息处理函数  
LRESULT CALLBACK BitmapWindowWndPrco(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParm);  


HBITMAP  g_hBitmap;  
/*int APIENTRY WinMain(HINSTANCE hInstance,  
					 HINSTANCE hPrevInstance,  
					 LPSTR     lpCmdLine,  
					 int       nCmdShow) 
{  
	//先创建一个无背影画刷窗口，  
	//然后在WM_CREATE中并指定透明颜色, 缩放位图后加载至s_hdcMem中.  
	//最后在WM_ERASEBKGND中用s_hdcMem贴图即可  
	g_hBitmap = (HBITMAP)LoadImage(NULL, L"Kitty.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  
	if (g_hBitmap == NULL)  
	{  
		MessageBox(NULL, L"位图加载失败", L"Error", MB_ICONERROR);  
		return 0;  
	}  

	// 设置异形窗口大小  
	BITMAP bm;  
	GetObject(g_hBitmap, sizeof(bm), &bm);  
	int nWindowWidth = bm.bmWidth;  
	int nWindowHeight = bm.bmHeight + 100; //拉高100高度  

	if (!InitBitmapWindow(hInstance, nWindowWidth, nWindowHeight, nCmdShow))  
		return 0;  

	MSG msg;  
	while (GetMessage(&msg, NULL, 0, 0))  
	{  
		TranslateMessage(&msg);  
		DispatchMessage(&msg);  
	}  
	DeleteObject(g_hBitmap);  

	return msg.wParam;  
}  
*/ 

BOOL InitBitmapWindow(HINSTANCE hinstance, int nWidth, int nHeight, int nCmdshow)  
{  
	HWND hwnd;  
	WNDCLASS wndclass;  

	wndclass.style       = CS_VREDRAW | CS_HREDRAW;  
	wndclass.lpfnWndProc = BitmapWindowWndPrco;   
	wndclass.cbClsExtra  = 0;  
	wndclass.cbWndExtra  = 0;  
	wndclass.hInstance   = hinstance;     
	wndclass.hIcon       = LoadIcon(NULL, IDI_APPLICATION);  
	wndclass.hCursor     = LoadCursor(NULL, IDC_ARROW);  
	wndclass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);//窗口背影画刷为空  
	wndclass.lpszMenuName  = NULL;  
	wndclass.lpszClassName = szAppName;  

	if (!RegisterClass(&wndclass))  
	{  
		MessageBox(NULL, L"Program Need Windows NT!", L"Error", MB_ICONERROR);  
		return FALSE;  
	}  

	hwnd = CreateWindowEx(WS_EX_TOPMOST,  
		szAppName,  
		szAppName,   
		WS_POPUP,  
		CW_USEDEFAULT,   
		CW_USEDEFAULT,   
		nWidth,   
		nHeight,  
		NULL,  
		NULL,  
		hinstance,  
		NULL);  
	if (hwnd == NULL)  
		return FALSE;  

	ShowWindow(hwnd, nCmdshow);  
	UpdateWindow(hwnd);  

	return TRUE;  
}  

LRESULT CALLBACK BitmapWindowWndPrco(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParm)  
{  
	static HDC s_hdcMem; //放置缩放后的位图  

	switch (message)  
	{  
	case WM_CREATE:  
		{  
			// 设置分层属性  
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);  
			// 设置透明色   
			SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);   
		}  
		return 0;  
	case WM_KEYDOWN:   
		switch (wParam)  
		{  
		case VK_ESCAPE: //按下Esc键时退出  
			SendMessage(hwnd, WM_DESTROY, 0, 0);  
			return TRUE;  
		}  
		break;  
	case WM_LBUTTONDOWN: //当鼠标左键点击时可以拖曳窗口  
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);   
		return TRUE;  

	case WM_ERASEBKGND: //在窗口背景中直接贴图  
		{  
			HDC hdc = (HDC)wParam;  
			int nWidth, nHeight;  
			GetWindowSize(hwnd, &nWidth, &nHeight);  
			//BitBlt(hdc, 0, 0, nWidth, nHeight, s_hdcMem, 0, 0, SRCCOPY);  
			return TRUE;  
		}  
	case WM_DESTROY:  
		DeleteDC(s_hdcMem);  
		PostQuitMessage(0);  
		return 0;  
	}  
	return DefWindowProc(hwnd, message, wParam, lParm);  
}  


void GetWindowSize(HWND hwnd, int *pnWidth, int *pnHeight)  
{  
	RECT rc;  
	GetWindowRect(hwnd, &rc);  
	*pnWidth = rc.right - rc.left;  
	*pnHeight = rc.bottom - rc.top;  
}  

/*--------------------------------------------------------------
【头文件】：GUIClass.h       Created by Beyond Ray,2014年2月
（描述）：一个封装了控件对象的类和控件处理函数
----------------------------------------------------------------*/

#pragma  once

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

//定义控件类型
#define UGP_GUI_STATIC_TEXT	1
#define UGP_GUI_BUTTON		2

//定义鼠标按键及指针的状态
#define GUI_LBUTTON_DOWN	1
#define GUI_LBUTTON_UP		2
#define GUI_MOUSE_OVER		3

//定义菜单页面切换的宏
#define GUI_BKGROUND_MAIN	1
#define GUI_BKGROUND_START	2
#define GUI_BKGROUND_LOAD	3
#define GUI_BKGROUND_OPTION	4

//具体的控件类型ID
#define GUI_STATIC_TEXT		0
#define GUI_BUTTON_START	1
#define GUI_BUTTON_LOAD		2
#define GUI_BUTTON_OPTION	3
#define GUI_BUTTON_QUIT		4
#define GUI_BUTTON_BACK     5

//定义控件类型ID最大上限编号
#define GUI_TEXT_MAX		0
#define GUI_BUTTON_MAX		5

//灵活顶点格式的结构
typedef struct  
{
	float x, y, z, rhw;			//顶点坐标
	
	float u, v;					//纹理贴图
	//DWORD color;
	
} GUIVERTEX, * LPGUIVERTEX;

#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_TEX1 )

//控件属性结构体
struct GUICONTROL
{
	int type;					//控件的类型（文本、按钮）
	int controlID;				//控件的具体类型ID
	WORD index;					//文本：对字体对象的索引，按钮：对顶点缓存的索引			
	WORD x;						//控件相对于窗口客户区左上角的X位置
	WORD y;						//控件相对于窗口客户区左上角的Y位置		
	WORD width;					//控件的宽度
	WORD height;				//控件的高度
	TCHAR * text;				//文本的内容
	unsigned long color;		//文本的颜色
	LPDIRECT3DTEXTURE9 textureUP, textureDown, textureOver;  //（只相对于按钮）表示鼠标按下，释放，经过时的纹理贴图
};

//【定义一个GUI对象类】
class GUIClass
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice9;		//Direct3D设备对象（COM接口对象）
	LPDIRECT3DTEXTURE9		m_pBackGround;		//控件对象的背景贴图（COM接口对象）
	LPDIRECT3DVERTEXBUFFER9 m_pBkBuffer;		//背景图缓存对象（COM接口对象）

	LPD3DXFONT			  * m_pFont;			//指向字体对象的指针（COM接口对象数组）
	LPDIRECT3DVERTEXBUFFER9 * m_pCtrlBuffer;	//控件按钮缓存对象的指针（COM接口对象数组）
	GUICONTROL			  * m_pControl;			//控件的数组对象

	int		m_iFontNums;		//字体种类的数目
	int     m_iTextNums;		//文本对象的数目
	int		m_iButtonNums;		//按钮对象的数目
	int		m_iControlNums;		//控件对象的数目

	WORD	m_wBackgroundWidth;		//贴图背景的宽度
	WORD	m_wBackgroundHeight;	//贴图背景的高度

	BOOL	m_bIsBKBufferUsed;	//用一个标识来判断是否使用了背景（以缓冲区的创建为标志）

public:
	GUIClass(LPDIRECT3DDEVICE9 pd3dDevice9, WORD Width, WORD Height);	//构造函数		
	~GUIClass(){ CleanUp(); }											//析构函数

public:
	LPDIRECT3DDEVICE9 GetDevice(){ return m_pd3dDevice9; }	//返回设备对象

	int		GetFontNums(){ return m_iFontNums; }			//返回字体种类的数目	
	int		GetTextNums(){ return m_iTextNums; }			//返回文本对象的数目
	int		GetButtonNums(){ return m_iButtonNums; }		//返回按钮对象的数目
	int		GetControlNums(){ return m_iControlNums; }		//返回控件对象的数目

	WORD	GetBackgroundWidth(){ return m_wBackgroundWidth; }		//返回贴图背景的宽度
	WORD	GetBackgroundHeight(){ return m_wBackgroundHeight; }	//返回贴图背景的高度

	VOID	SetBackgroundWidth(WORD Width){ m_wBackgroundWidth = Width; }		//设置贴图背景的宽度
	VOID	SetBackgroundHeight(WORD Height){ m_wBackgroundHeight = Height; }	//设置贴图背景的高度

	LPDIRECT3DTEXTURE9 GetBkTexture(){ return m_pBackGround; }		//返回背景纹理贴图
	LPDIRECT3DVERTEXBUFFER9 GetBkBuffer(){ return m_pBkBuffer; }	//返回背景顶点缓存

	BOOL	IsBKBufferUsed(){ return m_bIsBKBufferUsed; }			//判断是否使用了背景（以缓冲区的创建为标志）

	//用索引分别得到字体，按钮顶点缓存，控件的对象
	LPD3DXFONT	GetFont(WORD Index)
	{
		if (Index >= m_iFontNums) return NULL;
		return m_pFont[Index];
	}

	LPDIRECT3DVERTEXBUFFER9 GetCtrlBuffer(WORD Index)
	{
		if (Index >= m_iButtonNums) return NULL;
		return m_pCtrlBuffer[Index];
	}

	GUICONTROL*	GetControl(WORD Index)
	{ 
		if (Index >= m_iControlNums) return NULL;
		return &m_pControl[Index];
	}

	BOOL AddBackground(TCHAR * pBackgroundName);												//添加背景
	BOOL CreateTextFont(TCHAR * FontName, DWORD SIZE, WORD *Index);								//增加字体
	BOOL AddStaticText(TCHAR * TextContent, UINT ControlID, DWORD Color, WORD x, WORD y, WORD Index);//增加文本
	BOOL AddButton(UINT ControlID, WORD x, WORD y,												//增加按钮
		TCHAR* TextureDown, TCHAR* TextureUp, TCHAR* TextureOver);
	VOID CleanUp();																				//资源的清理	
};

BOOL ProcessGUI(GUIClass * GUI, BOOL LMBDown, WORD MouseX, WORD MouseY,
	void(CALLBACK *GUIProc)(int Id, int State));	//回调函数作形参





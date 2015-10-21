/*--------------------------------------------------------------
��ͷ�ļ�����GUIClass.h       Created by Beyond Ray,2014��2��
����������һ����װ�˿ؼ��������Ϳؼ�������
----------------------------------------------------------------*/

#pragma  once

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

//����ؼ�����
#define UGP_GUI_STATIC_TEXT	1
#define UGP_GUI_BUTTON		2

//������갴����ָ���״̬
#define GUI_LBUTTON_DOWN	1
#define GUI_LBUTTON_UP		2
#define GUI_MOUSE_OVER		3

//����˵�ҳ���л��ĺ�
#define GUI_BKGROUND_MAIN	1
#define GUI_BKGROUND_START	2
#define GUI_BKGROUND_LOAD	3
#define GUI_BKGROUND_OPTION	4

//����Ŀؼ�����ID
#define GUI_STATIC_TEXT		0
#define GUI_BUTTON_START	1
#define GUI_BUTTON_LOAD		2
#define GUI_BUTTON_OPTION	3
#define GUI_BUTTON_QUIT		4
#define GUI_BUTTON_BACK     5

//����ؼ�����ID������ޱ��
#define GUI_TEXT_MAX		0
#define GUI_BUTTON_MAX		5

//�����ʽ�Ľṹ
typedef struct  
{
	float x, y, z, rhw;			//��������
	
	float u, v;					//������ͼ
	//DWORD color;
	
} GUIVERTEX, * LPGUIVERTEX;

#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_TEX1 )

//�ؼ����Խṹ��
struct GUICONTROL
{
	int type;					//�ؼ������ͣ��ı�����ť��
	int controlID;				//�ؼ��ľ�������ID
	WORD index;					//�ı���������������������ť���Զ��㻺�������			
	WORD x;						//�ؼ�����ڴ��ڿͻ������Ͻǵ�Xλ��
	WORD y;						//�ؼ�����ڴ��ڿͻ������Ͻǵ�Yλ��		
	WORD width;					//�ؼ��Ŀ��
	WORD height;				//�ؼ��ĸ߶�
	TCHAR * text;				//�ı�������
	unsigned long color;		//�ı�����ɫ
	LPDIRECT3DTEXTURE9 textureUP, textureDown, textureOver;  //��ֻ����ڰ�ť����ʾ��갴�£��ͷţ�����ʱ��������ͼ
};

//������һ��GUI�����ࡿ
class GUIClass
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice9;		//Direct3D�豸����COM�ӿڶ���
	LPDIRECT3DTEXTURE9		m_pBackGround;		//�ؼ�����ı�����ͼ��COM�ӿڶ���
	LPDIRECT3DVERTEXBUFFER9 m_pBkBuffer;		//����ͼ�������COM�ӿڶ���

	LPD3DXFONT			  * m_pFont;			//ָ����������ָ�루COM�ӿڶ������飩
	LPDIRECT3DVERTEXBUFFER9 * m_pCtrlBuffer;	//�ؼ���ť��������ָ�루COM�ӿڶ������飩
	GUICONTROL			  * m_pControl;			//�ؼ����������

	int		m_iFontNums;		//�����������Ŀ
	int     m_iTextNums;		//�ı��������Ŀ
	int		m_iButtonNums;		//��ť�������Ŀ
	int		m_iControlNums;		//�ؼ��������Ŀ

	WORD	m_wBackgroundWidth;		//��ͼ�����Ŀ��
	WORD	m_wBackgroundHeight;	//��ͼ�����ĸ߶�

	BOOL	m_bIsBKBufferUsed;	//��һ����ʶ���ж��Ƿ�ʹ���˱������Ի������Ĵ���Ϊ��־��

public:
	GUIClass(LPDIRECT3DDEVICE9 pd3dDevice9, WORD Width, WORD Height);	//���캯��		
	~GUIClass(){ CleanUp(); }											//��������

public:
	LPDIRECT3DDEVICE9 GetDevice(){ return m_pd3dDevice9; }	//�����豸����

	int		GetFontNums(){ return m_iFontNums; }			//���������������Ŀ	
	int		GetTextNums(){ return m_iTextNums; }			//�����ı��������Ŀ
	int		GetButtonNums(){ return m_iButtonNums; }		//���ذ�ť�������Ŀ
	int		GetControlNums(){ return m_iControlNums; }		//���ؿؼ��������Ŀ

	WORD	GetBackgroundWidth(){ return m_wBackgroundWidth; }		//������ͼ�����Ŀ��
	WORD	GetBackgroundHeight(){ return m_wBackgroundHeight; }	//������ͼ�����ĸ߶�

	VOID	SetBackgroundWidth(WORD Width){ m_wBackgroundWidth = Width; }		//������ͼ�����Ŀ��
	VOID	SetBackgroundHeight(WORD Height){ m_wBackgroundHeight = Height; }	//������ͼ�����ĸ߶�

	LPDIRECT3DTEXTURE9 GetBkTexture(){ return m_pBackGround; }		//���ر���������ͼ
	LPDIRECT3DVERTEXBUFFER9 GetBkBuffer(){ return m_pBkBuffer; }	//���ر������㻺��

	BOOL	IsBKBufferUsed(){ return m_bIsBKBufferUsed; }			//�ж��Ƿ�ʹ���˱������Ի������Ĵ���Ϊ��־��

	//�������ֱ�õ����壬��ť���㻺�棬�ؼ��Ķ���
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

	BOOL AddBackground(TCHAR * pBackgroundName);												//��ӱ���
	BOOL CreateTextFont(TCHAR * FontName, DWORD SIZE, WORD *Index);								//��������
	BOOL AddStaticText(TCHAR * TextContent, UINT ControlID, DWORD Color, WORD x, WORD y, WORD Index);//�����ı�
	BOOL AddButton(UINT ControlID, WORD x, WORD y,												//���Ӱ�ť
		TCHAR* TextureDown, TCHAR* TextureUp, TCHAR* TextureOver);
	VOID CleanUp();																				//��Դ������	
};

BOOL ProcessGUI(GUIClass * GUI, BOOL LMBDown, WORD MouseX, WORD MouseY,
	void(CALLBACK *GUIProc)(int Id, int State));	//�ص��������β�





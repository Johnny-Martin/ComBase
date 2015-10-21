/*--------------------------------------------------------------
【Cpp文件】：GUIClass.cpp       Created by Beyond Ray,2014年2月
（描述）：一个封装了控件对象的类和控件处理函数的实现
----------------------------------------------------------------*/

#include"GUIClass.h"

//-------------------------------------------【GUIClass()函数】--------------------------------------------------
// Desc:构造函数（初始化）
//---------------------------------------------------------------------------------------------------------------
GUIClass::GUIClass(LPDIRECT3DDEVICE9 pd3dDevice9, WORD Width, WORD Height)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_pBackGround = NULL;
	m_pBkBuffer = NULL;

	m_pControl = NULL;
	m_pCtrlBuffer = NULL;
	m_pFont = NULL;

	m_iFontNums = 0;
	m_iTextNums = 0;
	m_iControlNums = 0;
	m_iButtonNums = 0;

	m_wBackgroundWidth = Width;
	m_wBackgroundHeight = Height;

	m_bIsBKBufferUsed = FALSE;
}

//-------------------------------------------【AddBackground()函数】---------------------------------------------
// Desc:添加GUIClass对象的唯一背景图片
//---------------------------------------------------------------------------------------------------------------
BOOL GUIClass::AddBackground(TCHAR *  pBackgroundName)
{
	if (!pBackgroundName) return FALSE;
	IF(D3DXCreateTextureFromFile(m_pd3dDevice9, pBackgroundName, &m_pBackGround));

	IF(m_pd3dDevice9->CreateVertexBuffer(4 * sizeof(GUIVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_GUI,
		 D3DPOOL_DEFAULT, &m_pBkBuffer, NULL));

	float w = (float)m_wBackgroundWidth;
	float h = (float)m_wBackgroundHeight;

	GUIVERTEX Vertices[] =
	{
		{ 0, h, 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0, 0, 0.0f, 1.0f, 0.0f, 0.0f },
		{ w, h, 0.0f, 1.0f, 1.0f, 1.0f },
		{ w, 0, 0.0f, 1.0f, 1.0f, 0.0f }
	};

	VOID *pVertices = NULL;
	IF(m_pBkBuffer->Lock(0, 0, &pVertices, NULL));
	memcpy(pVertices, Vertices, sizeof(Vertices));
	m_pBkBuffer->Unlock();

	m_bIsBKBufferUsed = true;
	return TRUE;
}

//--------------------------------------------【CreateTextFont()函数】-------------------------------------------
// Desc:创建并增加字体对象
//---------------------------------------------------------------------------------------------------------------
BOOL GUIClass::CreateTextFont(TCHAR * FontName, DWORD SIZE, WORD *Index)
{
	if (!m_pFont)
	{
		m_pFont = new LPD3DXFONT[1];
		if (!m_pFont) return FALSE;
	}
	else
	{
		LPD3DXFONT * pTempFont = new LPD3DXFONT[m_iFontNums + 1];
		if (!pTempFont) return FALSE;

		memset(pTempFont, 0, sizeof(pTempFont));
		memcpy(pTempFont, m_pFont, sizeof(m_pFont));
		SAFE_DELETE_ARRAY(m_pFont);
		m_pFont = pTempFont;
	}
	IF(D3DXCreateFont(m_pd3dDevice9, SIZE, 0, 1000, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, FontName, &m_pFont[m_iFontNums]));

	*Index = m_iFontNums;
	m_iFontNums++;
	return TRUE;
}

//-------------------------------------------【AddText()函数】---------------------------------------------------
// Desc:增加文本控件
//---------------------------------------------------------------------------------------------------------------
BOOL GUIClass::AddStaticText(TCHAR * TextContent,  UINT ControlID, DWORD Color, WORD x, WORD y, WORD Index)
{
	//对形参作检测判断
	if ( ControlID > GUI_TEXT_MAX || Index >= m_iFontNums)
		return FALSE;

	//为m_pControl分配新的内存空间
	if (!m_pControl)
	{
		m_pControl = new GUICONTROL[1];
		if (!m_pControl) return FALSE;
	}
	else
	{
		GUICONTROL * pTempCtrl = new GUICONTROL[m_iControlNums + 1];
		if (!pTempCtrl) return FALSE;

		memset(pTempCtrl, 0, (m_iControlNums + 1) * sizeof(GUICONTROL));
		memcpy(pTempCtrl, m_pControl, m_iControlNums * sizeof(GUICONTROL));
		SAFE_DELETE_ARRAY(m_pControl);
		m_pControl = pTempCtrl;
	}

	//填充新数据的参数
	m_pControl[m_iControlNums].type = UGP_GUI_STATIC_TEXT;
	m_pControl[m_iControlNums].controlID = ControlID;
	m_pControl[m_iControlNums].x = x;
	m_pControl[m_iControlNums].y = y;
	m_pControl[m_iControlNums].index = Index;
	m_pControl[m_iControlNums].color = Color;

	//新文本的复制
	int iChars = lstrlen(TextContent);
	m_pControl[m_iControlNums].text = new TCHAR[iChars + 1];

	if (!m_pControl[m_iControlNums].text) return FALSE;
	lstrcpy(m_pControl[m_iControlNums].text, TextContent);
	m_pControl[m_iControlNums].text[iChars] = '\0';

	//通过描述字段读取并计算宽和高
	D3DXFONT_DESC desc;
	m_pFont[Index]->GetDesc(&desc);
	m_pControl[m_iControlNums].height = desc.Height;
	m_pControl[m_iControlNums].width = desc.Width * iChars;

	//控件数量和文本数量的增加
	m_iControlNums++;
	m_iTextNums++;
	return TRUE;
}

//-------------------------------------------【AddButton()函数】----------------------------------------------
// Desc:增加按钮控件
//------------------------------------------------------------------------------------------------------------
BOOL GUIClass::AddButton(UINT ControlID, WORD x, WORD y, TCHAR* TextureDown, TCHAR* TextureUp, TCHAR* TextureOver)
{
	//对形参作检测判断
	if (ControlID <= GUI_TEXT_MAX || ControlID > GUI_BUTTON_MAX)
		return FALSE;

	//为m_pControl分配新的内存空间
	if (!m_pControl)
	{
		m_pControl = new GUICONTROL[1];
		if (!m_pControl) return FALSE;
	}
	else
	{
		GUICONTROL * pTempCtrl = new GUICONTROL[m_iControlNums + 1];
		if (!pTempCtrl) return FALSE;

		memset(pTempCtrl, 0, (m_iControlNums + 1) * sizeof(GUICONTROL));
		memcpy(pTempCtrl, m_pControl, m_iControlNums * sizeof(GUICONTROL));
		SAFE_DELETE_ARRAY(m_pControl);
		m_pControl = pTempCtrl;
	}

	//填充新数据的参数
	m_pControl[m_iControlNums].type = UGP_GUI_BUTTON;
	m_pControl[m_iControlNums].controlID = ControlID;

	IF(D3DXCreateTextureFromFile(m_pd3dDevice9, TextureDown, &m_pControl[m_iControlNums].textureDown));
	IF(D3DXCreateTextureFromFile(m_pd3dDevice9, TextureUp, &m_pControl[m_iControlNums].textureUP));
	IF(D3DXCreateTextureFromFile(m_pd3dDevice9, TextureOver, &m_pControl[m_iControlNums].textureOver));

	D3DSURFACE_DESC desc;
	m_pControl[m_iControlNums].textureDown->GetLevelDesc(0, &desc);
	m_pControl[m_iControlNums].x = x;
	m_pControl[m_iControlNums].y = y;
	m_pControl[m_iControlNums].height = desc.Height;
	m_pControl[m_iControlNums].width = desc.Width;

	//为m_pCtrlBuffer分配新的内存
	if (!m_pCtrlBuffer)
	{
		m_pCtrlBuffer = new LPDIRECT3DVERTEXBUFFER9[1];
		if (!m_pCtrlBuffer) return FALSE;
	}
	else
	{
		LPDIRECT3DVERTEXBUFFER9 * pTempCtrlBuffer = new LPDIRECT3DVERTEXBUFFER9[m_iButtonNums + 1];
		if (!pTempCtrlBuffer) return FALSE;

		memcpy(pTempCtrlBuffer, m_pCtrlBuffer, m_iButtonNums * sizeof(LPDIRECT3DVERTEXBUFFER9));//指针的赋值
		SAFE_DELETE_ARRAY(m_pCtrlBuffer);
		m_pCtrlBuffer = pTempCtrlBuffer;
	}

	//创建顶点缓冲区
	IF(m_pd3dDevice9->CreateVertexBuffer(4 * sizeof(GUIVERTEX), 0, D3DFVF_GUI, D3DPOOL_MANAGED,
		&m_pCtrlBuffer[m_iButtonNums], NULL));

	GUIVERTEX Vertics[] =
	{
		{ x, y + desc.Height, 0, 1.0f, 0.0f, 1.0f },
		{ x, y, 0, 1.0f, 0.0f, 0.0f },
		{ x + desc.Width, y + desc.Height, 0, 1.0f, 1.0f, 1.0f },
		{ x + desc.Width, y, 0, 1.0f, 1.0f, 0.0f }
	};

	VOID *pVertices = NULL;
	IF(m_pCtrlBuffer[m_iButtonNums]->Lock(0, 0, (void **)&pVertices, NULL));
	memcpy(pVertices, Vertics, sizeof(Vertics));
	m_pCtrlBuffer[m_iButtonNums]->Unlock();

	m_pControl[m_iControlNums].index = m_iButtonNums;

	//增加控件和按钮的数目
	m_iButtonNums++;
	m_iControlNums++;
	return TRUE;
}

//-------------------------------------------【CleanUp()函数】---------------------------------------------------
// Desc:资源的清理
//---------------------------------------------------------------------------------------------------------------
VOID GUIClass::CleanUp()
{
	if (m_bIsBKBufferUsed)
	{
		SAFE_RELEASE(m_pBackGround);
		SAFE_RELEASE(m_pBkBuffer);
	}

	for (int i = 0; i < m_iFontNums; i++)
		SAFE_RELEASE(m_pFont[i]);
	SAFE_DELETE_ARRAY(m_pFont);

	for (int i = 0; i < m_iButtonNums; i++)
	{
		SAFE_RELEASE(m_pCtrlBuffer[i]);
	}
	SAFE_DELETE_ARRAY(m_pCtrlBuffer);

	for (int i = 0; i < m_iControlNums; i++)
	{
		SAFE_RELEASE(m_pControl[i].textureDown);
		SAFE_RELEASE(m_pControl[i].textureUP);
		SAFE_RELEASE(m_pControl[i].textureOver);
		SAFE_DELETE_ARRAY(m_pControl[i].text);
	}
	SAFE_DELETE_ARRAY(m_pControl);
}

//----------------------------------------【ProcessGUI()函数】---------------------------------------------
// Desc:进行GUI的渲染
//---------------------------------------------------------------------------------------------------------
BOOL ProcessGUI(GUIClass * GUI, BOOL LMBDown, WORD MouseX, WORD MouseY, void(CALLBACK *GUIProc)(int Id, int State))
{
	if (!GUI) return FALSE;
	LPDIRECT3DDEVICE9  pd3dDevice9 = GUI->GetDevice();
	if (!pd3dDevice9) return FALSE;

	if (!(GUI->IsBKBufferUsed() && GUI->GetBkBuffer() && GUI->GetBkTexture()))
		return FALSE;

	//渲染GUI背景
	//pd3dDevice9->SetRenderState(D3DRS_LIGHTING, false);
	pd3dDevice9->SetTexture(0, GUI->GetBkTexture());
	pd3dDevice9->SetStreamSource(0, GUI->GetBkBuffer(), 0, sizeof(GUIVERTEX));
	pd3dDevice9->SetFVF(D3DFVF_GUI);
	pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	//用来显示文本的对象
	LPD3DXFONT pFont = NULL;
	RECT rect = { 0, 0, GUI->GetBackgroundWidth(), GUI->GetBackgroundHeight() };

	// 创建一个顶点缓存对象用于按钮的渲染
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
	int iState = GUI_LBUTTON_UP;

	//渲染控件
	GUICONTROL *pGUIControl = NULL;
	for (WORD i = 0; i < GUI->GetControlNums(); i++)
	{
		iState = GUI_LBUTTON_UP;
		pGUIControl = GUI->GetControl(i);
		if (!pGUIControl) continue;

		switch (pGUIControl->type)
		{
		case UGP_GUI_STATIC_TEXT:
			pFont = GUI->GetFont(pGUIControl->index);
			if (!pFont) continue;

			//设置起始位置并输出文本
			rect.left = pGUIControl->x;
			rect.top = pGUIControl->y;
			pFont->DrawText(NULL, pGUIControl->text, -1, &rect, DT_LEFT | DT_SINGLELINE, pGUIControl->color);
			break;

		case UGP_GUI_BUTTON:
			pVertexBuffer = GUI->GetCtrlBuffer(pGUIControl->index);
			if (!pVertexBuffer) continue;

			//开启Alpha混合
			pd3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			pd3dDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pd3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			if (MouseX >= pGUIControl->x && MouseX <= (pGUIControl->x + pGUIControl->width) &&
				MouseY >= pGUIControl->y && MouseY <= (pGUIControl->y + pGUIControl->height))
			{
				if (LMBDown)
					iState = GUI_LBUTTON_DOWN;
				else
					iState = GUI_MOUSE_OVER;
			}
			//开始渲染按钮
			if (iState == GUI_LBUTTON_DOWN) pd3dDevice9->SetTexture(0, pGUIControl->textureDown);
			if (iState == GUI_LBUTTON_UP) pd3dDevice9->SetTexture(0, pGUIControl->textureUP);
			if (iState == GUI_MOUSE_OVER) pd3dDevice9->SetTexture(0, pGUIControl->textureOver);

			pd3dDevice9->SetStreamSource(0, pVertexBuffer, 0, sizeof(GUIVERTEX));
			pd3dDevice9->SetFVF(D3DFVF_GUI);
			pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			//关闭Alpha混合
			pd3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		}
		if (GUIProc) GUIProc(pGUIControl->controlID, iState);
	}
	return TRUE;
}

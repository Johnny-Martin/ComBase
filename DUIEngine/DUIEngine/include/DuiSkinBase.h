#pragma once

#include "duiobject.h"
#include "duiimgbase.h"
#include "duiimgpool.h"
#include "duiref.h"

namespace DuiEngine
{

class DUI_EXP CDuiSkinBase : public CDuiObject,public CDuiRef
{
public:
    CDuiSkinBase():m_pDuiImg(NULL),m_strOwner("")
    {
    }
    virtual ~CDuiSkinBase()
    {
        if(m_pDuiImg) m_pDuiImg->Release();
    }

    void OnFinalRelease()
    {
        delete this;
    }

    void SetOwner(CDuiStringA strOwner)
    {
        m_strOwner=strOwner;
    }

    CDuiStringA GetOwner()
    {
        return m_strOwner;
    }


    virtual void SetImage(CDuiImgBase *pImg)
    {
		if(m_pDuiImg) m_pDuiImg->Release();
        m_pDuiImg=pImg;
		if(m_pDuiImg) m_pDuiImg->AddRef();
    }

    CDuiImgBase * GetImage()
    {
        return m_pDuiImg;
    }

    virtual void Draw(HDC dc, CRect rcDraw, DWORD dwState,BYTE byAlpha=0xFF) = NULL;

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return TRUE;
    }

    virtual void SetWidth(LONG width)
    {
    }

    virtual int GetStates()
    {
        return 1;
    }

    enum
    {
		Frame_Part_TopLeft    = 0x00000001UL,
		Frame_Part_TopCenter  = 0x00000002UL,
        Frame_Part_TopRight   = 0x00000004UL,
		Frame_Part_MidLeft    = 0x00000008UL,
		Frame_Part_MidCenter  = 0x00000010UL,
		Frame_Part_MidRight   = 0x00000020UL,
		Frame_Part_BottomLeft = 0x00000040UL,
		Frame_Part_BottomCenter  = 0x00000080UL,
		Frame_Part_BottomRight= 0x00000100UL,
		
		Frame_Part_Top        = (Frame_Part_TopLeft|Frame_Part_TopCenter|Frame_Part_TopRight),
		Frame_Part_Mid        = (Frame_Part_MidLeft|Frame_Part_MidCenter|Frame_Part_MidRight),
		Frame_Part_Bottom     = (Frame_Part_BottomLeft|Frame_Part_BottomCenter|Frame_Part_BottomRight),
		
		Frame_Part_Left       = (Frame_Part_TopLeft|Frame_Part_MidLeft|Frame_Part_BottomLeft),
		Frame_Part_Center     = (Frame_Part_TopCenter|Frame_Part_MidCenter|Frame_Part_BottomCenter),
		Frame_Part_Right      = (Frame_Part_TopRight|Frame_Part_MidRight|Frame_Part_BottomRight),
		
		Frame_Part_All        = 0x0000FFFFUL,
    };

    static BOOL ExtentBlt(CDuiImgBase *pImgDraw,BOOL bTile,HDC hdc,int x,int y,int nWid,int nHei,int xSrc,int ySrc,int nWidSrc,int nHeiSrc,BYTE byAlpha=0xFF);
    static void FrameDraw(HDC dc, CDuiImgBase *pImgDraw, const CRect &rcSour,const  CRect &rcDraw, CRect rcMargin, COLORREF crBg, UINT uDrawPart ,BOOL bTile,BYTE byAlpha=0xFF);

    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_IMAGE_ATTRIBUTE("src", m_pDuiImg, TRUE)
    DUIWIN_DECLARE_ATTRIBUTES_END()
protected:

    virtual void OnAttributeFinish(pugi::xml_node xmlNode)
    {
        //����ͼƬ�ļ���������������Ƥ����imgparam�ӽڵ���
		pugi::xml_node xmlNode_ImgParam=xmlNode.child("imgparam");
        if(xmlNode_ImgParam) m_pDuiImg->SetAttributes(xmlNode_ImgParam);
    }

    CDuiImgBase *m_pDuiImg;
    CDuiStringA	m_strOwner;
};



}//namespace DuiEngine

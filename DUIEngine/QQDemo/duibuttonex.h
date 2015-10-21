//////////////////////////////////////////////////////////////////////////
//   File Name: duibuttonex.h
// Description: DuiButtonEx
//     Creator: ZhangZhiBin QQ->276883782
//     Version: 2014.01.25 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////
#pragma once


namespace DuiEngine
{

//////////////////////////////////////////////////////////////////////////
// ButtonEx
// Use src attribute specify a resource id
//
// Usage: <buttonex normal-skin="" hover-skin="" down-skin="" disable-skin=""/>
//
class CDuiButtonEx : public DuiEngine::CDuiWindow
{
  DUIOBJ_DECLARE_CLASS_NAME(CDuiButtonEx, "buttonex")
public:
  CDuiButtonEx();

  virtual ~CDuiButtonEx();

  virtual BOOL NeedRedrawWhenStateChange();

  void OnPaint(CDCHandle dc);

  CDuiSkinBase * GetSkinNormal(){return m_pSkinNormal;}
  CDuiSkinBase * GetSkinHover(){return m_pSkinHover;}
  CDuiSkinBase * GetSkinDown(){return m_pSkinDown;}
  CDuiSkinBase * GetSkinDisable() { return m_pSkinDisable; }
protected:
  CDuiSkinBase *m_pSkinNormal;           //��ť��ͨ״̬�µ�Ƥ��
  CDuiSkinBase *m_pSkinHover;            //��ť����״̬�µ�Ƥ��
  CDuiSkinBase *m_pSkinDown;             //��ť����״̬�µ�Ƥ��
  CDuiSkinBase *m_pSkinDisable;          //��ť����״̬�µ�Ƥ��

  DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
  DUIWIN_SKIN_ATTRIBUTE("normal-skin", m_pSkinNormal, TRUE)
  DUIWIN_SKIN_ATTRIBUTE("hover-skin", m_pSkinHover, TRUE)
  DUIWIN_SKIN_ATTRIBUTE("down-skin", m_pSkinDown, TRUE)
  DUIWIN_SKIN_ATTRIBUTE("disable-skin", m_pSkinDisable, TRUE)

  DUIWIN_DECLARE_ATTRIBUTES_END()

  DUIWIN_BEGIN_MSG_MAP()
  MSG_WM_PAINT(OnPaint)
  DUIWIN_END_MSG_MAP()
};

}
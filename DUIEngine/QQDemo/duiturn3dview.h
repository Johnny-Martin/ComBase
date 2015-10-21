//////////////////////////////////////////////////////////////////////////
//   File Name: duiturn3dview.h
// Description: CDuiTurn3dView
//     Creator: ZhangZhiBin QQ->276883782
//     Version: 2014.02.06 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once 

namespace DuiEngine
{
  class CDuiTurn3dView
  {
  public:
    CDuiTurn3dView();
    virtual ~CDuiTurn3dView();

    void SetFrontSide(CDuiStringA strFrontSide);
    void SetBackSide(CDuiStringA strBackSide);
    void SetCartoon(CDuiStringA strCartoon);
    void SetDuiWindow(CDuiWindow* pDuiWindow);
    void SetImage3dView(CDuiStringA str3dView);
    void SetZStep(int nZStep);
    void SetYStep(int nYStep);
    void SetIdEvent(UINT_PTR idEvent);
    void SetElapse(UINT uElapse);
    void Turn(BOOL bTurn2Front);


    void OnTimer(UINT_PTR idEvent);

    void ShowFrame();                     //��ʾ��ת�����е�����һ֡
    BEGIN_MSG_MAP_EX(CDuiTurn3dView)
      MSG_WM_TIMER(OnTimer)
    END_MSG_MAP()
  protected:
    HBITMAP          m_hBitmapBefore;      //��תʱ�������bitmap
    HBITMAP          m_hBitmapAfter;       //��תʱ�ķ����bitmap
    BOOL             m_bTurn2Front;        //�Ƿ�ת������
    
    int              m_nFrameIndex;        //��ת���ڼ�֡
    LPSTR            m_lpDesk;             //ָ���¼�����·��ı�����ָ��
    BITMAPINFOHEADER m_bih;                //�봰�ڼ��ݵ�bitmap��ͼ��ͷ
    int              m_nZStep;             //z���ÿһ������
    int              m_nYStep;             //ÿ�ζ�����Y����תm_nYStep�Ƕ�
    CDuiStringA      m_strFrontSide;       //�����dlg������
    CDuiStringA      m_strBackSide;        //�����dlg������
    CDuiStringA      m_strCartoon;         //������dlg������
    CDuiStringA      m_strImage3dView;     //image3dview���ڴ�������ת���˴�����������
    UINT_PTR         m_idEvent;            //����������ʱ�ӵ�id
    UINT             m_uElapse;            //ʱ�ӵļ��
    CDuiWindow*      m_pDuiWindow;         //������duiwindow
  };
}
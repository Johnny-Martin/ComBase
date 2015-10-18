#ifndef MAINFRAME_H
#define MAINFRAME_H


#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <mmsystem.h>
#include "ximage.h"

class WindowImplBase;
class MainFrame : public WindowImplBase
{
protected:
	virtual tString GetSkinFile();
	virtual tString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const ;
	virtual UINT GetClassStyle() const ;
	virtual LONG GetStyle() const;
	virtual void Notify(TNotifyUI& msg); 
public:
    MainFrame();
	~MainFrame();
public:
	
#if _MSC_VER <=1200
    typedef  WindowImplBase __super;
#endif 
    
    
    void OnFinalMessage(HWND /*hWnd*/);

    void Init(); 

    void OnPrepare();
 
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam); 

	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);  
}; 

#endif //MAINFRAME_H
#ifndef CHATDIALOG_HPP
#define CHATDIALOG_HPP

#include "SkinChangeObserver.hpp"
#include "UIFriends.hpp"
#include "..\DuiLib\UIRichEdit.h"

#ifdef _DEBUG
#import "../bin/GifSmiley_d.dll" named_guids
#else
#import "../bin/GifSmiley.dll" named_guids
#endif

class WindowImplBase;
class ChatDialog : public WindowImplBase, public SkinChangeReceiver
{
public:

	ChatDialog(const tString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info);
	~ChatDialog();

public:

#if _MSC_VER <=1200
    typedef WindowImplBase __super;
#endif 

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam); 

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual BOOL Receive(SkinChangedParam param);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetTextColor(DWORD dwColor);

    void SendMsg();
 
protected:
	virtual tString GetSkinFile();
	virtual tString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const ;
	virtual UINT GetClassStyle() const ;
protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

private:
	void FontStyleChanged();
    BOOL InsertObject(CRichEditUI* pRichEdit);
	int GetObjectPos(CRichEditUI* pRichEdit);
	bool InsertAniSmiley(CRichEditUI* pRichEdit, long hwndHostWindow, BSTR bstrFileName, OLE_COLOR clr, INT cy);
	HRESULT GetNewStorage(LPSTORAGE* ppStg);
	void InsertGif(CRichEditUI* pRichEdit);
	void GetObjectInfo(IRichEditOle *pIRichEditOle);
	void ReleaseAllGif();
private:	
	bool emotion_timer_start_; 

	bool bold_;
	bool italic_;
	bool underline_;
	DWORD text_color_;
	DWORD font_size_;
	tString font_face_name_;

	tString bgimage_;
	DWORD bkcolor_;
	FriendListItemInfo myselft_;
	FriendListItemInfo friend_;
};

#endif // CHARTDIALOG_HPP
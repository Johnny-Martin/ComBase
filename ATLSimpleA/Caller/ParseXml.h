#include "stdafx.h"

class CXmlParser
{
public:
protected:
private:
};

BOOL CheckFileEncoding(LPCWSTR pszFilePath);

typedef void* (*CreateObjectCallBack)(void);

class CObjectFactory//µ¥Àý¹¤³§
{
public:
	virtual ~CObjectFactory(){}
	void* CreateObjectByClassName(string strClassName)
	{
		map<string, CreateObjectCallBack>::const_iterator iter = m_classMap.find(strClassName);
		
		if (m_classMap.end() == iter)
			return NULL;
		else
			return iter->second();
	}
	void RegistClass(string strClassName, CreateObjectCallBack callBackFun)
	{
		m_classMap.insert(std::pair<string, CreateObjectCallBack>(strClassName, callBackFun));
	}
	static CObjectFactory& GetFactoryInstance()
	{
		static CObjectFactory m_factoryInstance;
		return m_factoryInstance;
	}

private:
	CObjectFactory(){}
	std::map<string, CreateObjectCallBack> m_classMap;
};

class CBaseWnd
{
public:
	string GetWndHandleByID(){}
	string strWndID;
	/*static CBaseWnd* CreateBaseWndObject()
	{ 
		return new CBaseWnd();
	}*/
protected:
private:
};

class CModalWnd: public CBaseWnd
{
public:
	void DoModal(){}
	void Destroy(){}
	/*static CModalWnd* CreateModalWndObject()
	{ 
		return new CModalWnd();
	}*/
protected:
private:
};


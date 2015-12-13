#include "stdafx.h"
#include "UIObject.h"

set<string> LayoutObject::m_eventNameSet = LayoutObject::InitEventNameSet();

bool LayoutObject::InitAttrMap()
{
	m_attrMap.insert(pair<string, string>("position", ""));
	m_attrMap.insert(pair<string, string>("left", "0"));
	m_attrMap.insert(pair<string, string>("top", "0"));
	m_attrMap.insert(pair<string, string>("right", "0"));
	m_attrMap.insert(pair<string, string>("bottom", "0"));
	m_attrMap.insert(pair<string, string>("leftexp", ""));
	m_attrMap.insert(pair<string, string>("topexp", ""));
	m_attrMap.insert(pair<string, string>("rightexp", ""));
	m_attrMap.insert(pair<string, string>("bottomexp", ""));
	m_attrMap.insert(pair<string, string>("visible", "1"));
	m_attrMap.insert(pair<string, string>("enable", "1"));
	m_attrMap.insert(pair<string, string>("topmost", "0"));
	m_attrMap.insert(pair<string, string>("layered", "1"));
	m_attrMap.insert(pair<string, string>("appwnd", "1"));
	m_attrMap.insert(pair<string, string>("blur", "0"));
	m_attrMap.insert(pair<string, string>("minenable", "1"));
	m_attrMap.insert(pair<string, string>("maxenable", "1"));
	m_attrMap.insert(pair<string, string>("rootobjectid", ""));
	return true;
}
set<string> LayoutObject::InitEventNameSet()
{
	set<string> eventNameSet;
	eventNameSet.insert("OnCreate");
	eventNameSet.insert("OnShowWnd");
	eventNameSet.insert("OnShowWnd");
	eventNameSet.insert("OnDestory");
	eventNameSet.insert("OnStateChange");
	eventNameSet.insert("OnMove");
	eventNameSet.insert("OnSize");
	eventNameSet.insert("OnVisibleChange");
	eventNameSet.insert("OnEnableChange");
	eventNameSet.insert("OnCreate");

	return eventNameSet;
}
bool LayoutObject::SetAttr(string key, string value)
{
	if (CheckAttrName(key))
	{
		m_attrMap.insert(pair<string, string>(key, value));
		return true;
	}
	return false;
}
bool LayoutObject::GetAttr(string key, string* value)
{
	map<string, string>::iterator iter = m_attrMap.find(key);
	if (m_attrMap.end() != iter)
	{
		*value = iter->second;
		return true;
	}
	return false;
}
bool LayoutObject::CheckAttrName(string strName)
{ 
	return (m_attrMap.end() != m_attrMap.find(strName)) ? true:false;
}
bool LayoutObject::CheckEventName(string strName)
{ 
	return (m_eventMap.end() != m_eventMap.find(strName)) ? true:false;
}


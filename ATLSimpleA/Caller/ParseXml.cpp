#include "stdafx.h"
#include "ParseXml.h"
using namespace std;
////////////////////////////////////////////////////////////////
//UTF-8是一种变长字节编码方式，第一字节高位连续的1的个数代表了该
//字符占得字节数，其余字节皆以10开头
//1字节 0xxxxxxx (ANSI 字符)
//2字节 110xxxxx 10xxxxxx 
//3字节 1110xxxx 10xxxxxx 10xxxxxx 
//........
//6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
//注意: 若一个ANSI编码的文本文件中，全部是英文字符，则其文件编码
//与UTF8 无BOM编码一模一样，这种情况下，也认为是UTF8无BOM编码
BOOL CheckFileEncoding(LPCWSTR pszFilePath)
{//若是UTF8 无 BOM编码，返回文件句柄，避免后面重复openfile
	BOOL ret = ::PathFileExists(pszFilePath);
	if (!ret) return FALSE;

	std::ifstream inFile(pszFilePath, ios::in|ios::binary);
	if (!inFile) return FALSE;
	
	char byteFirst  = 0;
	char byteSecond = 0;
	char byteThird  = 0;
	
	inFile.read(&byteFirst, sizeof(char));
	inFile.read(&byteSecond, sizeof(char));
	inFile.read(&byteThird, sizeof(char));

	if (0xEF == (BYTE)byteFirst && 0xBB == (BYTE)byteSecond && 0xBF == (BYTE)byteThird)
	{//UTF-8 BOM头为文件前三字节：0xEF、0xBB、0xBF
		inFile.close();
		cout<<"UTF-8 带BOM编码"<<endl;
		return FALSE;//UTF8 带 BOM 编码
	}
	
	//将文件扫一遍，整个文件的编码若符合UTF8编码规则，则可认为其为UTF8 无 BOM编码
	inFile.seekg(0);
	char byteTmp = 0;
	BYTE byteCount = 0;
	
	while (!inFile.eof())
	{
		inFile.read(&byteFirst, sizeof(char));
		if ((byteFirst & 0x80) == 0 )
			continue;
		else if ((byteFirst & 0xFC) == 0xFC )
			byteCount = 6;
		else if ((byteFirst & 0xF8) == 0xF8 )
			byteCount = 5;
		else if ((byteFirst & 0xF0) == 0xF0 )
			byteCount = 4;
		else if ((byteFirst & 0xE0) == 0xE0 )
			byteCount = 3;
		else if ((byteFirst & 0xC0) == 0xC0 )
			byteCount = 2;
		else 
		{
			inFile.close();
			return FALSE;
		}
		
		while (--byteCount > 0)
		{
			inFile.read(&byteTmp, sizeof(char));
			if ((byteTmp & 0x80) != 0x80)
			{
				inFile.close();
				return FALSE;
			}
		}
	}

	inFile.close();
	cout<<"UTF-8 无BOM编码"<<endl;
	return TRUE;
}
bool CheckLabelMatch()
{
	return true;
}
bool CompileXml()
{
	//check xml file encoding type, must be UTF-8 without BOM
	//check label match
	return false;
}

set<string> CBaseWnd::m_attrNameSet = CBaseWnd::InitAttrNameSet();
set<string> CBaseWnd::m_eventNameSet = CBaseWnd::InitEventNameSet();

set<string> CBaseWnd::InitAttrNameSet()
{
	m_attrNameSet.insert("left");
	m_attrNameSet.insert("top");
	m_attrNameSet.insert("right");
	m_attrNameSet.insert("bottom");
	m_attrNameSet.insert("leftexp");
	m_attrNameSet.insert("topexp");
	m_attrNameSet.insert("rightexp");
	m_attrNameSet.insert("bottomexp");
	m_attrNameSet.insert("title");
	m_attrNameSet.insert("visible");
	m_attrNameSet.insert("enable");
	m_attrNameSet.insert("topmost");
	m_attrNameSet.insert("layered");
	m_attrNameSet.insert("appwnd");
	m_attrNameSet.insert("blur");
	m_attrNameSet.insert("minenable");
	m_attrNameSet.insert("maxenable");
	m_attrNameSet.insert("rootobjectid");
	
	return m_attrNameSet;
}
bool CBaseWnd::InitAttrMap()
{
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
set<string> CBaseWnd::InitEventNameSet()
{
	m_eventNameSet.insert("OnCreate");
	m_eventNameSet.insert("OnShowWnd");
	m_eventNameSet.insert("OnClose");
	m_eventNameSet.insert("OnDestory");
	m_eventNameSet.insert("OnStateChange");
	m_eventNameSet.insert("OnMove");
	m_eventNameSet.insert("OnSize");
	m_eventNameSet.insert("OnVisibleChange");
	m_eventNameSet.insert("OnEnableChange");
	m_eventNameSet.insert("OnCreate");

	return m_eventNameSet;
}
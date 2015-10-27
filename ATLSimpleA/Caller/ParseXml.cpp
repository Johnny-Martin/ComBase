#include "stdafx.h"
#include "ParseXml.h"
using namespace std;
////////////////////////////////////////////////////////////////
//UTF-8��һ�ֱ䳤�ֽڱ��뷽ʽ����һ�ֽڸ�λ������1�ĸ��������˸�
//�ַ�ռ���ֽ����������ֽڽ���10��ͷ
//1�ֽ� 0xxxxxxx (ANSI �ַ�)
//2�ֽ� 110xxxxx 10xxxxxx 
//3�ֽ� 1110xxxx 10xxxxxx 10xxxxxx 
//........
//6�ֽ� 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
//ע��: ��һ��ANSI������ı��ļ��У�ȫ����Ӣ���ַ��������ļ�����
//��UTF8 ��BOM����һģһ������������£�Ҳ��Ϊ��UTF8��BOM����
BOOL CheckFileEncoding(LPCWSTR pszFilePath)
{//����UTF8 �� BOM���룬�����ļ��������������ظ�openfile
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
	{//UTF-8 BOMͷΪ�ļ�ǰ���ֽڣ�0xEF��0xBB��0xBF
		inFile.close();
		cout<<"UTF-8 ��BOM����"<<endl;
		return FALSE;//UTF8 �� BOM ����
	}
	
	//���ļ�ɨһ�飬�����ļ��ı���������UTF8������������Ϊ��ΪUTF8 �� BOM����
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
	cout<<"UTF-8 ��BOM����"<<endl;
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
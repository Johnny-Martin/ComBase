#include "stdafx.h"
#include "ResManager.h"

RESERROE RImage::LoadResource(LPCWSTR wszResPath)
{
	return RES_SUCCESS;
}
RESERROE RImage::Draw()
{
	return RES_SUCCESS;
}
RESERROE RTexture::LoadResource(LPCWSTR wszResPath)
{
	return RES_SUCCESS;
}
RESERROE RTexture::Draw()
{
	return RES_SUCCESS;
}
RESERROE RPicList::LoadResource(LPCWSTR wszResPath)
{
	//detect the dividing line(RGB: 127,0,127)
	//++++++++++++++++++++++++++++++++++++++++++
	wstring wszFileName;
	wszFileName = ::PathFindFileName(wszResPath);
	std::size_t iBeginPos = wszFileName.find(L"texturelist");
	if (0 == iBeginPos)
	{
	}
	std::size_t iBeginPosEx = wszFileName.find(L"imagelist");
	
	return RES_SUCCESS;
}

wstring ResManager::GetPicPathByID(LPCSTR szResID)
{
	LPWSTR wszPath = new WCHAR[MAX_PATH];

	unsigned int i;
	for (i=0; i<m_wszResPath.length(); ++i)
		wszPath[i] = m_wszResPath[i];
	wszPath[i] = '\0';

	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, szResID, -1, NULL, 0);
	LPWSTR wszMore = new WCHAR[unicodeLen + 1];
	MultiByteToWideChar(CP_ACP, 0, szResID, -1, wszMore, unicodeLen);

	PathAppend(wszPath, wszMore);

	wstring wstrRet = wszPath;
	delete[] wszPath;
	delete[] wszMore;

	wstrRet += L".png";
	return wstrRet;
}

RESERROE ResManager::GetResPicHandle(LPCSTR szResID, RPicture** hRes)
{
	map<string, RPicture*>::iterator iter = m_resID2HandleMap.find(szResID);
	if (m_resID2HandleMap.end() != iter)
	{
		*hRes = iter->second;
		return RES_SUCCESS;
	}

	wstring resFilePath;
	string strResID = szResID;
	//list's strResID must has .index at the end
	std::size_t iBeginPos = strResID.find("texturelist");
	std::size_t iBeginPosEx = strResID.find("imagelist");
	if (0 == iBeginPos || 0 == iBeginPosEx)
	{
		//get the index section
		std::size_t iResIDLen = strlen(szResID);
		unsigned int iLastDotPos = iResIDLen;
		for (; iLastDotPos>0; --iLastDotPos)
		{
			if ('.' == szResID[iLastDotPos])
				break;
		}
		
		bool bLastSectionCheck = true;
		int picIndex = 0;
		for (unsigned int i=iLastDotPos + 1; i<iResIDLen; ++i)
		{
			if (szResID[i] > '9' || szResID[i] < '0')
			{
				bLastSectionCheck = false;
				break;
			}else
			{
				picIndex = picIndex*10 + szResID[i] - '0';
			}
		}

		if (!bLastSectionCheck)
			return RES_ERROR_ILLEGAL_ID;

		string strRealResId;
		for (unsigned int i=0; i<iLastDotPos; ++i)
			strRealResId.append(sizeof(char), szResID[i]);

		resFilePath = GetPicPathByID(strRealResId.c_str());
		if (!::PathFileExists(resFilePath.c_str()))
			return RES_ERROR_FILE_NOT_FOUND;

		RPicList* picListObj = new RPicList(resFilePath.c_str());
		
		//insert each of texture into map
		//++++++++++++++++++++++++++++++++
		m_resID2HandleMap.insert(pair<string, RPicList*>(szResID, picListObj));

		*hRes = picListObj;
		return RES_SUCCESS;
	}

	iBeginPos = strResID.find("texture");
	iBeginPosEx = strResID.find("image");
	if (0 == iBeginPos || 0 == iBeginPosEx)
	{
		resFilePath = GetPicPathByID(szResID);
		if (!::PathFileExists(resFilePath.c_str()))
			return RES_ERROR_FILE_NOT_FOUND;

		RPicture* picObj = NULL;
		if (0 == iBeginPos)
		{
			picObj = new RTexture(resFilePath.c_str());
		}else
		{
			picObj = new RImage(resFilePath.c_str());
		}
		m_resID2HandleMap.insert(pair<string, RPicture*>(szResID, picObj));

		*hRes = picObj;
		return RES_SUCCESS;
	}

	return RES_ERROR_UNKNOWN;
}
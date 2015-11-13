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
	return RES_SUCCESS;
}

wstring ResManager::GetPicPathByID(LPSTR szResID)
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

RESERROE ResManager::GetResPicHandle(LPSTR szResID, RPicture** hRes)
{
	map<string, RPicture*>::iterator iter = m_resID2HandleMap.find(szResID);
	if (m_resID2HandleMap.end() != iter)
	{
		*hRes = iter->second;
		return RES_SUCCESS;
	}

	wstring resFilePath = GetPicPathByID(szResID);
	if (!::PathFileExists(resFilePath.c_str()))
		return RES_ERROR_FILE_NOT_FOUND;
	
	string strResID = szResID;
	std::size_t iBeginPos = strResID.find("texture");
	if (0 == iBeginPos)
	{
		RTexture* textureObj = new RTexture(resFilePath.c_str());
		m_resID2HandleMap.insert(pair<string, RTexture*>(szResID, textureObj));

		*hRes = textureObj;
		return RES_SUCCESS;
	}

	iBeginPos = strResID.find("image");
	if (0 == iBeginPos)
	{
		RImage* imageObj = new RImage(resFilePath.c_str());
		m_resID2HandleMap.insert(pair<string, RImage*>(szResID, imageObj));

		*hRes = imageObj;
		return RES_SUCCESS;
	}

	iBeginPos = strResID.find("texturelist");
	if (0 == iBeginPos)
	{
		RPicList* picListObj = new RPicList(resFilePath.c_str());
		m_resID2HandleMap.insert(pair<string, RPicList*>(szResID, picListObj));

		*hRes = picListObj;
		return RES_SUCCESS;
	}

	iBeginPos = strResID.find("imagelist");
	if (0 == iBeginPos)
	{
		RPicList* picListObj = new RPicList(resFilePath.c_str());
		m_resID2HandleMap.insert(pair<string, RPicList*>(szResID, picListObj));

		*hRes = picListObj;
		return RES_SUCCESS;
	}

	return RES_ERROR_UNKNOWN;
}
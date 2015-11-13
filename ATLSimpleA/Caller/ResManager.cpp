#include "stdafx.h"
#include "ResManager.h"

RESERROE ResManager::GetResPicHandle(LPSTR szResID, RPicture** hRes)
{
	map<string, RPicture*>::iterator iter = m_resID2HandleMap.find(szResID);
	if (m_resID2HandleMap.end() != iter)
	{
		*hRes = iter->second;
		return RES_SUCCESS;
	}

	string strResID = szResID;
	std::size_t iBeginPos = strResID.find("texture");
	if (0 == iBeginPos)
	{
		int i = 1;
	}

	iBeginPos = strResID.find("image");
	if (0 == iBeginPos)
	{
	}

	iBeginPos = strResID.find("texturelist");
	if (0 == iBeginPos)
	{
	}

	iBeginPos = strResID.find("imagelist");
	if (0 == iBeginPos)
	{
	}

	return RES_ERROR_UNKNOWN;
}
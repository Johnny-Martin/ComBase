#include "stdafx.h"
#include "ResManager.h"
#include "png.h"

bool RPicture::IsVerticalLine(unsigned int horizontalPos, const COLORREF lineColor)
{
	for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
	{
		png_byte* row = m_rowPointers[rowIndex];
		png_byte* ptr = &(row[horizontalPos*4]);
		
		COLORREF pixelColor = RGBA(ptr[0], ptr[1], ptr[2], ptr[3]);
		if (lineColor != pixelColor)
			return false;
	}
	return true;
}
RESERROE RPicture::ReadPngFile(LPCWSTR wszFilePath)
{
	int multiByteLen = WideCharToMultiByte(CP_ACP, 0, wszFilePath, -1, NULL, 0, NULL, NULL);
	char* file_name = new char[multiByteLen + 1];
	WideCharToMultiByte(CP_ACP, 0, wszFilePath, -1, file_name, multiByteLen, NULL, NULL);

	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		return RES_ERROR_FILE_NOT_FOUND;

	unsigned char header[8];    // 8 is the maximum size that can be checked
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
		return RES_ERROR_ILLEGAL_FILE_TYPE;


	/* initialize stuff */
	m_pngStructPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!m_pngStructPtr)
		return RES_ERROR_PARSE_FILE_FALIED;

	m_pngInfoPtr = png_create_info_struct(m_pngStructPtr);
	if (!m_pngInfoPtr)
		return RES_ERROR_PARSE_FILE_FALIED;

	if (setjmp(png_jmpbuf(m_pngStructPtr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_init_io(m_pngStructPtr, fp);
	png_set_sig_bytes(m_pngStructPtr, 8);

	png_read_info(m_pngStructPtr, m_pngInfoPtr);

	m_pngWidth = png_get_image_width(m_pngStructPtr, m_pngInfoPtr);
	m_pngHeight = png_get_image_height(m_pngStructPtr, m_pngInfoPtr);
	m_colorType = png_get_color_type(m_pngStructPtr, m_pngInfoPtr);
	m_bitDepth = png_get_bit_depth(m_pngStructPtr, m_pngInfoPtr);

	int number_of_passes = png_set_interlace_handling(m_pngStructPtr);
	png_read_update_info(m_pngStructPtr, m_pngInfoPtr);


	/* read file */
	if (setjmp(png_jmpbuf(m_pngStructPtr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	m_rowPointers = (png_bytep*) malloc(sizeof(png_bytep) * m_pngHeight);
	//m_rowPointers = new png_bytep[(sizeof(png_bytep) * m_pngHeight)];
	
	for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
	{
		png_uint_32 size = png_get_rowbytes(m_pngStructPtr,m_pngInfoPtr);
		m_rowPointers[rowIndex] = (png_byte*) malloc(size);
	}

	png_read_image(m_pngStructPtr, m_rowPointers);

	fclose(fp);
	return RES_SUCCESS;
}
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
//detect the dividing line(RGB: 255,0,255)
RESERROE RTexture::DetectPurpleLine()
{
	//before calling this function,make sure that
	//png file must has been loaded to memory successfully.

	return RES_SUCCESS;
}
RESERROE RTexture::Draw()
{
	return RES_SUCCESS;
}
RESERROE RPicList::LoadResource(LPCWSTR wszResPath)
{
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

//detect the dividing line(RGB: 127,0,127)
//a pictrue list's dividing line must be vertical
RESERROE RPicList::DetectPurpleLine()
{
	//before calling this function,make sure that
	//png file must has been loaded to memory successfully.
	png_byte* pixelDataPtr = NULL;
	for (unsigned int columnIndex=0; columnIndex<m_pngWidth; ++columnIndex)
	{
		pixelDataPtr = &(m_rowPointers[0][columnIndex*4]);
		COLORREF pixelColor = RGBA(pixelDataPtr[0], pixelDataPtr[1], pixelDataPtr[2], pixelDataPtr[3]);
		if (m_purpleLineColor == pixelColor)
		{
			if (IsVerticalLine(columnIndex, m_purpleLineColor))
			{
				m_arrDivideLinePosH.push_back(columnIndex);
			}
		}
	}
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
bool ResManager::CheckPngFileHead(LPWSTR wszFilePath)
{
	//PNG file head: 89 50 4E 47 0D 0A 1A 0A
	BYTE pngFileHead[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
	
	std::ifstream inPngFile(wszFilePath, ios::binary);
	BYTE value = 0;
	for (int i=0; i<sizeof(pngFileHead)/sizeof(BYTE); ++i)
	{
		inPngFile.read((char*)&value, sizeof(BYTE));
		if (value != pngFileHead[i])
			return false;
	}
	return true;
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
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
RESERROR RPicture::ReadPngFile(LPCWSTR wszFilePath)
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
RESERROR RPicture::WritePngFileEx(LPCWSTR wszFilePath, png_bytep *rowPointers, unsigned int width, unsigned int height)
{
	int multiByteLen = WideCharToMultiByte(CP_ACP, 0, wszFilePath, -1, NULL, 0, NULL, NULL);
	char* file_name = new char[multiByteLen + 1];
	WideCharToMultiByte(CP_ACP, 0, wszFilePath, -1, file_name, multiByteLen, NULL, NULL);

	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		return RES_ERROR_FILE_NOT_FOUND;


	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		return RES_ERROR_PARSE_FILE_FALIED;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return RES_ERROR_PARSE_FILE_FALIED;

	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_set_IHDR(png_ptr, info_ptr, width, height,
		m_bitDepth, m_colorType, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	//write png file info to file
	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_write_image(png_ptr, rowPointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_write_end(png_ptr, NULL);

	fclose(fp);
	return RES_SUCCESS;
}
RESERROR RPicture::WritePngFile(LPCWSTR wszFilePath)
{
	return WritePngFileEx(wszFilePath, m_rowPointers, m_pngWidth, m_pngHeight);
}
RESERROR RImage::LoadResource(LPCWSTR wszResPath)
{
	return RES_SUCCESS;
}
RESERROR RImage::Draw()
{
	return RES_SUCCESS;
}
RESERROR RTexture::LoadResource(LPCWSTR wszResPath)
{
	return RES_SUCCESS;
}

//create a texture object with a two-dimensional array, and assign the png width and height
RTexture::RTexture(LPCSTR szResID, png_bytep* rowPointers, unsigned int width, unsigned int height, png_byte bitDepth/* =8 */, png_byte colorType/* =6 */)
{
	SetResID(szResID);
	m_pngStructPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!m_pngStructPtr)
	{
		m_resError = RES_ERROR_PARSE_FILE_FALIED;
		return;
	}
	
	m_pngInfoPtr = png_create_info_struct(m_pngStructPtr);
	if (!m_pngInfoPtr)
	{
		m_resError = RES_ERROR_PARSE_FILE_FALIED;
		return;
	}

	png_set_IHDR(m_pngStructPtr, m_pngInfoPtr, width, height,
					bitDepth, colorType, PNG_INTERLACE_NONE,
						PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	//png_write_info(m_pngStructPtr, m_pngInfoPtr);
	
	m_pngWidth  = width;
	m_pngHeight = height;
	m_bitDepth  = bitDepth;
	m_colorType = colorType;
	m_rowPointers = rowPointers;
}
//detect the dividing line(RGB: 255,0,255)
RESERROR RTexture::DetectPurpleLine()
{
	//before calling this function,make sure that
	//png file must has been loaded to memory successfully.

	return RES_SUCCESS;
}
RESERROR RTexture::Draw()
{
	return RES_SUCCESS;
}
RESERROR RPicList::LoadResource(LPCWSTR wszResPath)
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
RESERROR RPicList::DetectPurpleLine()
{
	//before calling this function,make sure that
	//png file *must* has been loaded to memory successfully.
	png_byte* pixelDataPtr = NULL;
	unsigned int lastLineColumnIndex = 0;
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
RESERROR RPicList::CreatePicFromMem()
{
	if (m_arrDivideLinePosH.size() <= 0)
		return RES_ERROR_UNKNOWN;

	png_bytep* pngDataPtr = NULL;
	vector<unsigned int>::iterator iter = m_arrDivideLinePosH.begin();
	
	//a virtual dividing line before 0 column
	int lastDivideLinePos = -1;
	//a virtual dividing line after last column
	m_arrDivideLinePosH.push_back(m_pngWidth);
	
	for (unsigned int verticalLineIndex = 0; verticalLineIndex < m_arrDivideLinePosH.size(); ++verticalLineIndex)
	{
		unsigned int curDivideLinePos = m_arrDivideLinePosH[verticalLineIndex];
		unsigned int newWidth = curDivideLinePos - lastDivideLinePos - 1;
		pngDataPtr = (png_bytep*) malloc(m_pngHeight * sizeof(png_bytep));
		
		for (unsigned int rowIndex=0; rowIndex < m_pngHeight; ++rowIndex)
		{
			pngDataPtr[rowIndex] = (png_byte*) malloc(newWidth*4);
			
			//copy png pixel data row by row, it takes *four* bytes per pixel
			for (unsigned int columnIndex=0; columnIndex < newWidth; ++columnIndex)
			{
				unsigned int srcPngPixelPos = lastDivideLinePos + columnIndex + 1;
				png_byte* dstPtr = &(pngDataPtr[rowIndex][columnIndex*4]);
				png_byte* srcPtr = &(m_rowPointers[rowIndex][srcPngPixelPos*4]);

				dstPtr[0] = srcPtr[0];//R
				dstPtr[1] = srcPtr[1];//G
				dstPtr[2] = srcPtr[2];//B
				dstPtr[3] = srcPtr[3];//A
			}

		}
		lastDivideLinePos = curDivideLinePos;

		char szIndex[10]={0};
		_itoa(verticalLineIndex+1, szIndex, 10);
		string textureId = m_szResID + '.';
		textureId += szIndex;
		RTexture* pTexture = new RTexture(textureId.c_str(), pngDataPtr, newWidth, m_pngHeight);
		m_picListVector.push_back(pTexture);
		
		//put out to file for checking data
		//wstring outPath = L"E:\\code\\ComBase\\trunk\\UIEngine\\docs\\";
		//outPath.append(sizeof(char), verticalLineIndex + '0');
		//outPath += L".png";
		//WritePngFile(outPath.c_str(), pngDataPtr, newWidth, m_pngHeight);

		///* cleanup heap allocation */
		//for (unsigned int y=0; y<m_pngHeight; y++)
		//{
		//	png_byte* tmpPtr = pngDataPtr[y];
		//	free(tmpPtr);
		//}
		//free(pngDataPtr);

	}

	m_arrDivideLinePosH.pop_back();
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
unsigned int ResManager::GetIndexFromPicListId(LPCSTR szPicListID)
{
	unsigned int picIndex = 0;
	//get the index section
	std::size_t iPicListIDLen = strlen(szPicListID);
	
	unsigned int iLastDotPos = iPicListIDLen;
	for (; iLastDotPos>0; --iLastDotPos)
	{
		if ('.' == szPicListID[iLastDotPos])
			break;
	}

	for (unsigned int i=iLastDotPos + 1; i<iPicListIDLen; ++i)
	{
		if (szPicListID[i] > '9' || szPicListID[i] < '0')
		{
			return 0;
		}else
		{
			picIndex = picIndex*10 + szPicListID[i] - '0';
		}
	}

	return picIndex;
}
string ResManager::GetRealIdFromPicListId(LPCSTR szPicListID)
{
	//get the index section
	unsigned int iLastDotPos = strlen(szPicListID);
	for (; iLastDotPos>0; --iLastDotPos)
	{
		if ('.' == szPicListID[iLastDotPos])
			break;
	}
	
	string strRealResId;
	for (unsigned int i=0; i<iLastDotPos; ++i)
		strRealResId.append(sizeof(char), szPicListID[i]);

	return strRealResId;
}
RESERROR ResManager::GetResPicHandle(LPCSTR szResID, RPicture** hRes)
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
		unsigned int resIndex = GetIndexFromPicListId(szResID);
		if (resIndex <= 0)
			return RES_ERROR_ILLEGAL_ID;

		string strRealResId = GetRealIdFromPicListId(szResID);
		iter = m_resID2HandleMap.find(strRealResId);
		//piclist has been created
		if (m_resID2HandleMap.end() != iter)
		{
			RPicList* picListObjPointer = dynamic_cast<RPicList*>(iter->second);
			*hRes = picListObjPointer->GetPicByIndex(resIndex - 1);
			return (NULL == (*hRes)) ? RES_ERROR_ILLEGAL_ID : RES_SUCCESS;
		}

		resFilePath = GetPicPathByID(strRealResId.c_str());
		if (!::PathFileExists(resFilePath.c_str()))
			return RES_ERROR_FILE_NOT_FOUND;

		RPicList* picListObj = new RPicList(resFilePath.c_str(), strRealResId.c_str());
		
		//insert each of texture into map
		//++++++++++++++++++++++++++++++++
		m_resID2HandleMap.insert(pair<string, RPicList*>(szResID, picListObj));

		*hRes = picListObj->GetPicByIndex(resIndex - 1);
		return (NULL == (*hRes)) ? RES_ERROR_ILLEGAL_ID : RES_SUCCESS;
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
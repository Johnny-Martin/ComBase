#include "stdafx.h"
#include "ResManager.h"
#include "png.h"

bool RPicture::IsVerticalLine(unsigned int horizontalPos, const COLORREF lineColor)
{
	unsigned int bytesPerPixel = m_pixelDepth/8;
	for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
	{
		png_byte* row = m_rowPointers[rowIndex];
		png_byte* ptr = &(row[horizontalPos*bytesPerPixel]);
		
		COLORREF pixelColor = RGB(ptr[0], ptr[1], ptr[2]);
		if (lineColor != pixelColor)
			return false;
	}
	return true;
}
bool RPicture::IsHorizontalLine(unsigned int horizontalPos, const COLORREF lineColor)
{
	png_byte* row = m_rowPointers[horizontalPos];
	unsigned int bytesPerPixel = m_pixelDepth/8;
	for (unsigned int columnIndex=0; columnIndex<m_pngWidth; ++columnIndex)
	{
		png_byte* ptr = &(row[columnIndex*bytesPerPixel]);

		COLORREF pixelColor = RGB(ptr[0], ptr[1], ptr[2]);
		if (lineColor != pixelColor)
			return false;
	}
	return true;
}
//Only support 24 and 32 bits pixel depth png file
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
	
	if (png_get_color_type(m_pngStructPtr, m_pngInfoPtr) != PNG_COLOR_TYPE_RGB
		&& png_get_color_type(m_pngStructPtr, m_pngInfoPtr) != PNG_COLOR_TYPE_RGBA)
		return RES_ERROR_ILLEGAL_PNG_FILE;

	m_pngWidth   = png_get_image_width(m_pngStructPtr, m_pngInfoPtr);
	m_pngHeight  = png_get_image_height(m_pngStructPtr, m_pngInfoPtr);
	m_colorType  = png_get_color_type(m_pngStructPtr, m_pngInfoPtr);
	m_bitDepth   = png_get_bit_depth(m_pngStructPtr, m_pngInfoPtr);
	m_pixelDepth = m_pngInfoPtr->pixel_depth;

	int number_of_passes = png_set_interlace_handling(m_pngStructPtr);
	png_read_update_info(m_pngStructPtr, m_pngInfoPtr);


	/* read file */
	if (setjmp(png_jmpbuf(m_pngStructPtr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	m_rowPointers = (png_bytep*) malloc(sizeof(png_bytep) * m_pngHeight);
	//m_rowPointers = new png_bytep[(sizeof(png_bytep) * m_pngHeight)];
	
	//ID2D1HwndRenderTarget::CreateBitmap only support continuous png pixel data in memory
	//allocate a continuous memory for m_rowPointers so that Image can return m_rowPointers
	//directly in GetPngPixelArray
	png_uint_32 rowSize = png_get_rowbytes(m_pngStructPtr,m_pngInfoPtr);
	png_byte* pngPixelData = (png_byte*) malloc(rowSize * m_pngHeight);
	for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
	{
		png_byte *rowHead = (png_byte*)((int)pngPixelData + rowIndex * rowSize);
		m_rowPointers[rowIndex] = (png_byte*) rowHead;
	}

	//Old memory allocation
	//for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
	//{
	//	png_uint_32 size = png_get_rowbytes(m_pngStructPtr,m_pngInfoPtr);
	//	m_rowPointers[rowIndex] = (png_byte*) malloc(size);
	//}
	png_read_image(m_pngStructPtr, m_rowPointers);

	fclose(fp);
	return RES_SUCCESS;
}
//this function inspect the png data and png info that build function "CreatePicByData" 
//if we can create a png file in disk with "m_rowPointers" and "m_pngInfoPtr"
//the data must be definitely correct!
RESERROR RPicture::WritePngFile(LPCWSTR wszFilePath)
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

	png_infop info_ptr = png_create_info_struct(png_ptr);//m_pngInfoPtr;
	if (!info_ptr)
		return RES_ERROR_PARSE_FILE_FALIED;

	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_set_IHDR(png_ptr, info_ptr, m_pngWidth, m_pngHeight,
		m_bitDepth, m_colorType, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	//write png file info to file
	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_write_image(png_ptr, m_rowPointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		return RES_ERROR_PARSE_FILE_FALIED;

	png_write_end(png_ptr, NULL);

	fclose(fp);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	return RES_SUCCESS;
}

RESERROR RPicture::CreatePicByData(LPCSTR szResID, png_bytep* rowPointers, unsigned int width, unsigned int height, png_byte bitDepth, png_byte colorType)
{
	m_pngStructPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!m_pngStructPtr)
	{
		//m_resError = RES_ERROR_PARSE_FILE_FALIED;
		return RES_ERROR_PARSE_FILE_FALIED;
	}

	m_pngInfoPtr = png_create_info_struct(m_pngStructPtr);
	if (!m_pngInfoPtr)
	{
		//m_resError = RES_ERROR_PARSE_FILE_FALIED;
		return RES_ERROR_PARSE_FILE_FALIED;
	}

	png_set_IHDR(m_pngStructPtr, m_pngInfoPtr, width, height,
		bitDepth, colorType, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	//png_write_info(m_pngStructPtr, m_pngInfoPtr);

	m_pngWidth    = width;
	m_pngHeight   = height;
	m_bitDepth    = bitDepth;
	m_colorType   = colorType;
	m_rowPointers = rowPointers;
	m_pixelDepth  = m_pngInfoPtr->pixel_depth;
	//m_pngInfoPtr->pixel_depth = m_pixelDepth;

	return RES_SUCCESS;
}
RESERROR RImage::LoadResource(LPCWSTR wszResPath)
{
	return RES_SUCCESS;
}
RESERROR RImage::Draw(ID2D1HwndRenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom)
{
	return RES_SUCCESS;
}

RESERROR RTexture::LoadResource(LPCWSTR wszResPath)
{
	return RES_SUCCESS;
}


//detect the dividing line(RGB: 255,0,255)
RESERROR RTexture::DetectVerticalLine()
{
	//before calling this function,make sure that
	//png file must has been loaded to memory successfully.
	png_byte* pixelDataPtr = NULL;
	unsigned int bytesPerPixel = m_pixelDepth/8;

	for (unsigned int columnIndex=0; columnIndex<m_pngWidth; ++columnIndex)
	{
		pixelDataPtr = &(m_rowPointers[0][columnIndex*bytesPerPixel]);
		COLORREF pixelColor = RGB(pixelDataPtr[0], pixelDataPtr[1], pixelDataPtr[2]);
		if (m_purpleLineColor == pixelColor)
		{
			if (IsVerticalLine(columnIndex, m_purpleLineColor))
			{
				m_arrVerticalLinePos.push_back(columnIndex);
			}
		}

	}
	return RES_SUCCESS;
}
RESERROR RTexture::DetectHorizontalLine()
{
	//before calling this function,make sure that
	//png file must has been loaded to memory successfully.
	png_byte* pixelDataPtr = NULL;
	unsigned int bytesPerPixel = m_pixelDepth/8;
	for (unsigned int rowIndex=0; rowIndex<m_pngHeight; ++rowIndex)
	{
		pixelDataPtr = &(m_rowPointers[rowIndex][0]);
		COLORREF pixelColor = RGB(pixelDataPtr[0], pixelDataPtr[1], pixelDataPtr[2]);
		if (m_purpleLineColor == pixelColor)
		{
			if (IsHorizontalLine(rowIndex, m_purpleLineColor))
			{
				m_arrHorizontalLinePos.push_back(rowIndex);
			}
		}

	}
	return RES_SUCCESS;
}
RESERROR RTexture::ProcessTexture()
{
	if (THREE_V == m_textureType)
	{
		m_resError = DetectVerticalLine();
		if (RES_SUCCESS == m_resError)
		{

		}
	}else if (NINE == m_textureType)
	{
		DetectVerticalLine();
		DetectHorizontalLine();
	}else if (THREE_H == m_textureType)
	{
		DetectHorizontalLine();
	}else
		abort();
	return RES_SUCCESS;
}
RESERROR RTexture::CreateD2D1Bitmap(ID2D1HwndRenderTarget* pRenderTarget)
{
	if (THREE_V == m_textureType)
	{

	}else if (NINE == m_textureType)
	{
		int lastBlockXEnd = -1;
		int lastBlockYEnd = -1;
		m_arrVerticalLinePos.push_back(m_pngWidth);
		m_arrHorizontalLinePos.push_back(m_pngHeight);
		for (int i=0; i<9; ++i)
		{
			if (i%3 == 0)
				lastBlockXEnd = -1;
			else
				lastBlockXEnd = m_arrVerticalLinePos[i%3 - 1];

			if (i/3 == 0)
				lastBlockYEnd = -1;
			else
				lastBlockYEnd = m_arrHorizontalLinePos[i/3 - 1];

			UINT curBlockWidth  = m_arrVerticalLinePos[i%3] - lastBlockXEnd - 1;
			UINT curBlockHeight = m_arrHorizontalLinePos[i/3] - lastBlockYEnd - 1;

			UINT pngRowSize = m_pngWidth*m_pixelDepth/8;
			UINT curBlockRowSize = curBlockWidth*m_pixelDepth/8;
			png_byte* curBlockData = (png_byte*) malloc(curBlockHeight*curBlockRowSize);
			png_byte* pngData = m_rowPointers[0];

			for (UINT rowIndex=0; rowIndex<curBlockHeight; ++rowIndex)
				for (UINT columnIndex=0; columnIndex<curBlockRowSize; ++columnIndex)
				{
					UINT offset = (lastBlockXEnd + 1)*m_pixelDepth/8 + (rowIndex + lastBlockYEnd + 1)*pngRowSize + columnIndex;
					curBlockData[rowIndex*curBlockRowSize + columnIndex] = pngData[offset];
				}
			
			D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
			D2D1_BITMAP_PROPERTIES properties = {pixelFormat, 96.0, 96.0};
			ID2D1Bitmap *pBitmap = NULL;
			HRESULT hr = pRenderTarget->CreateBitmap(D2D1::SizeU(curBlockWidth, curBlockHeight), (void *)curBlockData, curBlockRowSize , properties, &pBitmap);
			if (SUCCEEDED(hr))
			{
				m_arrD2D1Bitmap.push_back(pBitmap);
			}
			//delete curBlockData
			free(curBlockData);
		}

		m_arrVerticalLinePos.pop_back();
		m_arrHorizontalLinePos.pop_back();

	}else if (THREE_H == m_textureType)
	{
	}else
		abort();
	return RES_SUCCESS;
}
RESERROR RTexture::Draw(ID2D1HwndRenderTarget* pRenderTarget, UINT left, UINT top, UINT right, UINT bottom)
{
	if (m_arrD2D1Bitmap.size() == 0)
	{
		CreateD2D1Bitmap(pRenderTarget);
	}
	ID2D1Bitmap *pBitmap = NULL; //DXGI_FORMAT_R8G8B8A8_UNORM DXGI_FORMAT_B8G8R8A8_UNORM
	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);//D2D1_ALPHA_MODE_IGNORE
	D2D1_BITMAP_PROPERTIES properties = {pixelFormat, 96.0, 96.0};
	
	UINT pitch = m_pngWidth * m_pixelDepth / 8;
	//HRESULT hr = pRenderTarget->CreateBitmap(D2D1::SizeU( m_pngWidth, m_pngHeight), (void *)m_rowPointers[0], pitch , properties, &pBitmap);

	//if (SUCCEEDED(hr))
	{
		D2D1_RECT_F dstRect = D2D1::RectF(left ,top, right, bottom);
		if (NINE == m_textureType)
		{
			pBitmap		   = m_arrD2D1Bitmap[0];
			dstRect.right  = left + m_arrVerticalLinePos[0];
			dstRect.bottom = top + m_arrHorizontalLinePos[0];
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[1];
			dstRect.left   = dstRect.right;
			dstRect.right  = right - (m_pngWidth - m_arrVerticalLinePos[1]);
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[2];
			dstRect.left   = dstRect.right;
			dstRect.right  = right;
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[3];
			dstRect.left   = left;
			dstRect.top	   = dstRect.bottom;
			dstRect.right  = left + m_arrVerticalLinePos[0];
			dstRect.bottom = bottom - (m_pngHeight - m_arrHorizontalLinePos[1]);
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[4];
			dstRect.left   = dstRect.right;
			dstRect.right  = right - (m_pngWidth - m_arrVerticalLinePos[1]);
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[5];
			dstRect.left   = dstRect.right;
			dstRect.right  = right;
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[6];
			dstRect.left   = left;
			dstRect.right  = left + m_arrVerticalLinePos[0];
			dstRect.top    = dstRect.bottom;
			dstRect.bottom = bottom;
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap		   = m_arrD2D1Bitmap[7];
			dstRect.left   = dstRect.right;
			dstRect.right  = right - (m_pngWidth - m_arrVerticalLinePos[1]);
			pRenderTarget->DrawBitmap(pBitmap, dstRect);

			pBitmap = m_arrD2D1Bitmap[8];
			dstRect.left   = dstRect.right;
			dstRect.right  = right;
			pRenderTarget->DrawBitmap(pBitmap, dstRect);
		}
		//D2D1_RECT_F dstRect = D2D1::RectF(left ,top, left + m_pngWidth, top + m_pngHeight);
	
	}
	
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
RESERROR RPicList::DetectVerticalLine()
{
	//before calling this function,make sure that
	//png file *must* has been loaded to memory successfully.
	png_byte* pixelDataPtr = NULL;
	unsigned int bytesPerPixel = m_pixelDepth/8;

	for (unsigned int columnIndex=0; columnIndex<m_pngWidth; ++columnIndex)
	{
		pixelDataPtr = &(m_rowPointers[0][columnIndex*bytesPerPixel]);
		COLORREF pixelColor = RGB(pixelDataPtr[0], pixelDataPtr[1], pixelDataPtr[2]);
		if (m_purpleLineColor == pixelColor)
		{
			if (IsVerticalLine(columnIndex, m_purpleLineColor))
			{
				m_arrVerticalLinePos.push_back(columnIndex);
			}
		}

	}
	return RES_SUCCESS;
}
RESERROR RPicList::CreatePicFromMem()
{
	if (m_arrVerticalLinePos.size() <= 0)
		return RES_ERROR_UNKNOWN;

	png_bytep* pngDataPtr = NULL;
	vector<unsigned int>::iterator iter = m_arrVerticalLinePos.begin();
	
	//a virtual dividing line before 0 column
	int lastDivideLinePos = -1;
	//a virtual dividing line after last column
	m_arrVerticalLinePos.push_back(m_pngWidth);
	
	for (unsigned int verticalLineIndex = 0; verticalLineIndex < m_arrVerticalLinePos.size(); ++verticalLineIndex)
	{
		unsigned int curDivideLinePos = m_arrVerticalLinePos[verticalLineIndex];
		unsigned int newWidth = curDivideLinePos - lastDivideLinePos - 1;
		pngDataPtr = (png_bytep*) malloc(m_pngHeight * sizeof(png_bytep));
		
		unsigned int bytesPerPixel = m_pixelDepth/8;
		for (unsigned int rowIndex=0; rowIndex < m_pngHeight; ++rowIndex)
		{
			pngDataPtr[rowIndex] = (png_byte*) malloc(newWidth*bytesPerPixel);
			
			//copy png pixel data row by row, it takes *four* bytes per pixel
			for (unsigned int columnIndex=0; columnIndex < newWidth; ++columnIndex)
			{
				unsigned int srcPngPixelPos = lastDivideLinePos + columnIndex + 1;
				png_byte* dstPtr = &(pngDataPtr[rowIndex][columnIndex*bytesPerPixel]);
				png_byte* srcPtr = &(m_rowPointers[rowIndex][srcPngPixelPos*bytesPerPixel]);

				for (unsigned int i=0; i<bytesPerPixel; ++i)
					dstPtr[i] = srcPtr[i];
			}

		}
		lastDivideLinePos = curDivideLinePos;

		char szIndex[10]={0};
		_itoa(verticalLineIndex+1, szIndex, 10);
		string textureId = m_szResID + '.';
		textureId += szIndex;
		RPicture* pPicObj = NULL;
		if (TEXTURELIST == m_picListType)
			pPicObj = new RTexture(textureId.c_str(), pngDataPtr, newWidth, m_pngHeight, m_bitDepth, m_colorType);
		else
			pPicObj = new RImage(textureId.c_str(), pngDataPtr, newWidth, m_pngHeight, m_bitDepth, m_colorType);
		
		m_picListVector.push_back(pPicObj);
	}

	m_arrVerticalLinePos.pop_back();
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
		RPicture* pObj = *hRes;

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
			picObj = new RTexture(resFilePath.c_str(), szResID);
		}else
		{
			picObj = new RImage(resFilePath.c_str(), szResID);
		}
		m_resID2HandleMap.insert(pair<string, RPicture*>(szResID, picObj));

		*hRes = picObj;
		return RES_SUCCESS;
	}

	return RES_ERROR_UNKNOWN;
}
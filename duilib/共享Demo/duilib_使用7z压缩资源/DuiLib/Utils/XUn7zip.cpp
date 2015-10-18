////////////////////////////////////////////////////////////////////////////////
//      
//      File for XUn7zip.cpp
//      
//      File      : XUn7zip.cpp
//      Version   : 1.0
//      Comment   : 解压缩7z文件
//      
//      Create at : 2013-07-26
//      Create by : 东
//		Motify by : 
//      
//		Comment:	数据全部解压缩放入vector,
//					每次均查询vector,
//					并非进入7z文件查询
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <vector>

#include "7zip/7z.h"
#include "7zip/7zAlloc.h"
#include "7zip/7zCrc.h"
#include "7zip/7zFile.h"
#include "7zip/7zString.h"
#include "scoped_ptr.h"

__inline LPSTR GetAnsiFileName(LPCWSTR lpwstr)
{
	LPSTR lpResult = NULL;

	size_t nLen = wcslen(lpwstr);

	DWORD dwNum = WideCharToMultiByte(CP_ACP, 0, lpwstr, (DWORD)nLen, NULL, 0, NULL, NULL);

	lpResult = new CHAR[dwNum + 1];
	if ( lpResult != NULL )
	{
		lpResult[0] = 0;
		WideCharToMultiByte(CP_ACP, 0, lpwstr, (DWORD)nLen, lpResult, dwNum, NULL, NULL);
		lpResult[dwNum] = 0;
	}

	return lpResult;
}

__inline LPWSTR GetUnicodeFileName(LPCSTR lpstr)
{
	LPWSTR lpResult = NULL;

	size_t nLen = strlen(lpstr);

	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, lpstr, (DWORD)nLen, NULL, 0);

	lpResult = new WCHAR[dwNum + 1];
	if ( lpResult != NULL )
	{
		lpResult[0] = 0;
		MultiByteToWideChar(CP_ACP, 0, lpstr, (DWORD)nLen, lpResult, dwNum);
		lpResult[dwNum] = 0;
	}
	return lpResult;
}

__inline BOOL IsSameUInt16(const UInt16 *one,const UInt16 *two,UInt32 len)
{

	for (size_t i = 0 ; i<len ; i++)
	{
		if(one[i] != two[i])
			return FALSE;
	}

	return TRUE;
}

class TUn7zip
{
public:
	BOOL Open7zU(const char *filepath);
	BOOL Open7zU(const wchar_t *filepath);
	BOOL Open7zU(void *z, unsigned int len);
	SRes Get7zItem(UInt32 index,UInt64 *size);
	SRes Find7zItem(const UInt16 *name,UInt32 lenght,UInt32 *index,UInt64 *size);
	SRes Un7zItem(UInt32 index,void *dst);
	SRes Close7zU();

	size_t GetItemsCount();
private:
	typedef struct
	{
		UInt16 *name;
		UInt32 lenght;
		UInt32 index;
		Byte   *buf;
		UInt64 size;
	} HandleData;

	void InsertItem(HandleData* item);

	std::vector<HandleData*> vItems;
};

void TUn7zip::InsertItem(HandleData* item)
{
	vItems.push_back(item);
}

size_t TUn7zip::GetItemsCount()
{
	return vItems.size();
}

BOOL TUn7zip::Open7zU(void* z, unsigned int len)
{
	CStringInStream archiveStream;
	CLookToRead lookStream;
	CSzArEx db;
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;

	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;

	//初始化
	String_Construct(&archiveStream.file);
	String_Open(&archiveStream.file,z,&len);

	StringInStream_CreateVTable(&archiveStream);
	LookToRead_CreateVTable(&lookStream, False);

	lookStream.realStream = &archiveStream.s;
	LookToRead_Init(&lookStream);

	CrcGenerateTable();

	BOOL bRes = FALSE;
	SzArEx_Init(&db);
	SRes res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
	if (res != SZ_OK)
	{
		this->Close7zU();
	}
	else
	{
		UInt32 blockIndex = 0xFFFFFFFF;
		Byte *outBuffer = 0;
		size_t outBufferSize = 0;

		size_t offset = 0;
		size_t outSizeProcessed = 0;
		const CSzFileItem *f = NULL;

		size_t len;

		for (UInt32 i = 0; i < db.db.NumFiles; i++)
		{

			offset = 0;
			outSizeProcessed = 0;
			f = db.db.Files + i;

			if (f->IsDir ) continue;

			len = SzArEx_GetFileNameUtf16(&db, i, NULL);

			HandleData* hd = new HandleData;
			hd->index = i;
			hd->size = f->Size;
			hd->lenght = len;
			hd->name = (UInt16 *)SzAlloc(NULL, len * sizeof(hd->name[0]));
			hd->buf = 0;

			SzArEx_GetFileNameUtf16(&db, i, hd->name);

			res = SzArEx_Extract(&db, &lookStream.s, i,
				&blockIndex, &outBuffer, &outBufferSize,
				&offset, &outSizeProcessed,
				&allocImp, &allocTempImp);
			if (res != SZ_OK)
			{
				bRes = FALSE;
				break;
			}

			hd->buf = (Byte *)SzAlloc(NULL, outSizeProcessed * sizeof(hd->buf[0]));
			::memcpy(hd->buf,outBuffer + offset, outSizeProcessed);

			this->InsertItem(hd);
		}
		IAlloc_Free(&allocImp, outBuffer);
		bRes = TRUE;
	}

	SzArEx_Free(&db, &allocImp);
	String_Close(&archiveStream.file);

	return bRes;
}

BOOL TUn7zip::Open7zU(const wchar_t *filepath)
{
	BOOL bRes = False;
	const char* sz_filepath = NULL;
	sz_filepath = GetAnsiFileName(filepath);
	if(sz_filepath != NULL)
	{
		bRes = this->Open7zU(sz_filepath);
		delete []sz_filepath;
		sz_filepath = NULL;
	}
	return bRes;
}

BOOL TUn7zip::Open7zU(const char* filepath)
{
	CFileInStream archiveStream;
	CLookToRead lookStream;
	CSzArEx db;
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;

	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;

	BOOL bRes = False;

	if (!InFile_Open(&archiveStream.file, filepath))
	{
		FileInStream_CreateVTable(&archiveStream);
		LookToRead_CreateVTable(&lookStream, False);

		lookStream.realStream = &archiveStream.s;
		LookToRead_Init(&lookStream);

		CrcGenerateTable();

		SzArEx_Init(&db);
		SRes res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
		if (res == SZ_OK)
		{
			UInt32 blockIndex = 0xFFFFFFFF;
			Byte *outBuffer = 0;
			size_t outBufferSize = 0;
			const CSzFileItem *f = NULL;
			size_t offset = 0;
			size_t outSizeProcessed = 0;
			size_t len;

			HandleData* hd = NULL;

			for (UInt32 i = 0; i < db.db.NumFiles; i++)
			{

				offset = 0;
				outSizeProcessed = 0;
				f = db.db.Files + i;

				if (f->IsDir ) continue;

				len = SzArEx_GetFileNameUtf16(&db, i, NULL);

				hd = new HandleData;
				hd->index = i;
				hd->size = f->Size;
				hd->lenght = len;
				hd->name = (UInt16 *)SzAlloc(NULL, len * sizeof(hd->name[0]));
				hd->buf = 0;

				SzArEx_GetFileNameUtf16(&db, i, hd->name);

				res = SzArEx_Extract(&db, &lookStream.s, i,
					&blockIndex, &outBuffer, &outBufferSize,
					&offset, &outSizeProcessed,
					&allocImp, &allocTempImp);
				if (res != SZ_OK)
				{
					bRes = FALSE;
					break;
				}

				hd->buf = (Byte *)SzAlloc(NULL, outSizeProcessed * sizeof(hd->buf[0]));
				::memcpy(hd->buf,outBuffer + offset, outSizeProcessed);

				this->InsertItem(hd);
			}
			IAlloc_Free(&allocImp, outBuffer);
		}

		SzArEx_Free(&db, &allocImp);
		File_Close(&archiveStream.file);
		bRes = TRUE;
	}

	return bRes;
}

SRes TUn7zip::Close7zU()
{
	return SZ_OK;
}

SRes TUn7zip::Get7zItem(UInt32 index,UInt64 *size)
{
	SRes res = SZ_ERROR_READ;
	for (std::vector<HandleData*>::iterator it = vItems.begin(); it != vItems.end() ; it ++)
	{
		HandleData* hd = (*it);

		if(hd->index == index )
		{
			*size = hd->size;
			res = SZ_OK;
			break;
		}
	}

	return res;
}

SRes TUn7zip::Find7zItem(const UInt16 *name,UInt32 lenght,UInt32 *index,UInt64 *size)
{
	SRes res = SZ_ERROR_READ;
	for (std::vector<HandleData*>::iterator it = vItems.begin(); it != vItems.end() ; it ++)
	{
		HandleData* hd = (*it);
		if(hd->lenght == lenght && IsSameUInt16(hd->name,name,lenght))
		{
			*index = hd->index;
			*size = hd->size;

			res = SZ_OK;
			break;
		}
	}

	return res;
}

SRes TUn7zip::Un7zItem(UInt32 index,void *dst)
{
	SRes res = SZ_ERROR_READ;
	for (std::vector<HandleData*>::iterator it = vItems.begin(); it != vItems.end() ; it ++)
	{
		HandleData* hd = (*it);

		if(hd->index == index)
		{
			memcpy(dst,hd->buf,(size_t)hd->size);
			res = SZ_OK;
			break;
		}
	}

	return res;
}

typedef struct
{
	public_base::scoped_ptr<TUn7zip> un7z;
} TUn7zipHandleData;

HANDLE Open7zipU(const wchar_t *z, HANDLE h7z = NULL)
{
	TUn7zipHandleData *han = NULL;
	if(h7z == NULL)
	{
		han = new TUn7zipHandleData;
	}
	else
	{
		han = static_cast<TUn7zipHandleData*>(h7z);
	}

	han->un7z.reset(new TUn7zip);

	TUn7zip* u7z = han->un7z.get();

	if(!u7z->Open7zU(z))
	{
		delete han;
		return 0;
	}

	return (HANDLE)han;
}

HANDLE Open7zipU(const char *z, HANDLE h7z = NULL)
{
	TUn7zipHandleData *han = NULL;
	if(h7z == NULL)
	{
		han = new TUn7zipHandleData;
	}
	else
	{
		han = static_cast<TUn7zipHandleData*>(h7z);
	}

	han->un7z.reset(new TUn7zip);

	TUn7zip* u7z = han->un7z.get();

	if(!u7z->Open7zU(z))
	{
		delete han;
		return 0;
	}

	return (HANDLE)han;
}

HANDLE Open7zipU(void *z, unsigned int l, HANDLE h7z = NULL)
{
	TUn7zipHandleData *han = NULL;
	if(h7z == NULL)
	{
		han = new TUn7zipHandleData;
	}
	else
	{
		han = static_cast<TUn7zipHandleData*>(h7z);
	}

	han->un7z.reset(new TUn7zip);

	TUn7zip* u7z = han->un7z.get();

	if(!u7z->Open7zU(z,l))
	{
		delete han;
		return 0;
	}

	return (HANDLE)han;
}

unsigned long Get7zipItem(HANDLE h7z, int index, unsigned __int64* size)
{ 
	if(h7z == NULL)
		return 0x00010000;

	TUn7zipHandleData *han = static_cast<TUn7zipHandleData*>(h7z);

	TUn7zip* u7z = han->un7z.get();

	if(u7z->Get7zItem(index,size) != SZ_OK)
		return 0x00010000;

	return 0;
}

unsigned long Get7zipCount(HANDLE h7z,size_t* size)
{
	if(h7z == NULL)
		return 0x00010000;

	TUn7zipHandleData *han = static_cast<TUn7zipHandleData*>(h7z);

	TUn7zip* u7z = han->un7z.get();

	*size = u7z->GetItemsCount();

	return 0;
}

unsigned long Find7zipItemW(HANDLE h7z, const WCHAR *name, size_t *index, unsigned __int64 *size)
{ 
	if(h7z == NULL || name == NULL)
		return 0x00010000;

	TUn7zipHandleData *han = static_cast<TUn7zipHandleData*>(h7z);

	TUn7zip* u7z = han->un7z.get();

	const UInt16* u16 = reinterpret_cast<const UInt16*>(name);
	UInt32 len = wcslen(name) + 1; //缺少'\0' 补位

	if(u7z->Find7zItem(u16,len,(UInt32*)index,(UInt64*)size) != SZ_OK)
		return 0x00010000;

	return 0;
}

unsigned long Find7zipItemA(HANDLE h7z, const CHAR *name, size_t *index, unsigned __int64 *size)
{
	LPWSTR wname = NULL;
	if(h7z == NULL || name == NULL || (wname =GetUnicodeFileName(name)) == NULL)
		return 0x00010000;

	TUn7zipHandleData *han = static_cast<TUn7zipHandleData*>(h7z);

	TUn7zip* u7z = han->un7z.get();

	const UInt16* u16 = reinterpret_cast<const UInt16*>(wname);
	UInt32 len = wcslen(wname) + 1; //缺少'\0' 补位

	if(u7z->Find7zItem(u16,len,(UInt32*)index,(UInt64*)size) != SZ_OK)
	{
		delete []wname;
		return 0x00010000;
	}

	delete []wname;
	return 0;
}

unsigned long Un7zipItem(HANDLE h7z, int index, void *dst)
{ 
	if(h7z == NULL)
		return 0x00010000;

	TUn7zipHandleData *han = static_cast<TUn7zipHandleData*>(h7z);
	TUn7zip* u7z = han->un7z.get();

	if(u7z->Un7zItem(index,dst) != SZ_OK)
		return 0x00010000;

	return 0;
}

unsigned long Close7zipU(HANDLE h7z)
{
	if(h7z == NULL)
		return 0x00010000;

	TUn7zipHandleData *han = static_cast<TUn7zipHandleData*>(h7z);

	TUn7zip* u7z = han->un7z.get();

	u7z->Close7zU();

	delete han;

	return 0;
}

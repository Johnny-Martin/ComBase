// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <OleIdl.h>
#include <ShellAPI.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <exdisp.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "define.h"
#include "GlobalSet.h"

#include "../duilib11x/UIlib.h"

#ifdef _DEBUG
#pragma comment(lib, "Duilib11xUD.lib")
#else
#pragma comment(lib, "Duilib11xU.lib")
#endif

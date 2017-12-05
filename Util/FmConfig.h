/********************************************************************
created:	2012-03-25
author:		Fish (于国平)
summary:	设置
*********************************************************************/
#pragma once

#include "FmPlatform.h"

/// standard library
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
using namespace std;

#include <algorithm>
#include <functional>
#include <limits>
#include <stdarg.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#if FM_COMPILER == FM_COMPILER_MSVC
#pragma comment(lib, "winmm.lib" )
#pragma comment(lib, "ws2_32.lib" )
#include <assert.h>
#pragma warning(disable : 4996)
#endif

extern "C" {
#include <sys/stat.h>
}

#if FM_PLATFORM == FM_PLATFORM_LINUX
extern "C" {
#include <unistd.h>
#include <dlfcn.h>
}
#endif

// 断言宏
#ifdef _DEBUG
	#if FM_PLATFORM == FM_PLATFORM_WIN32
		#ifdef UNICODE
			#define Assert(_Expression)\
				if( !(_Expression) )\
			{\
				printf( "ASSERT:%s", #_Expression );\
				printf( " (%s:%d)\n", __FILE__, __LINE__ );\
				char buf[1024*4] = { NULL };\
				printf( buf, "ASSERT:%s (%s:%d)\n", #_Expression, __FILE__, __LINE__ );\
				WCHAR wszBuf[1024*4] = {NULL};\
				MultiByteToWideChar(CP_ACP, 0, buf, -1, wszBuf, sizeof(wszBuf));\
				OutputDebugStringW( wszBuf );\
			}
		#else
			#define Assert(_Expression)\
				if( !(_Expression) )\
			{\
				printf( "ASSERT:%s", #_Expression );\
				printf( " (%s:%d)\n", __FILE__, __LINE__ );\
				char buf[1024*4] = { NULL };\
				printf( buf, "ASSERT:%s (%s:%d)\n", #_Expression, __FILE__, __LINE__ );\
				OutputDebugString( buf );\
			}
		#endif //#ifdef UNICODE
	#else
		#define Assert(_Expression)\
			if( !(_Expression) )\
		{\
			printf( "ASSERT:%s", #_Expression );\
			printf( " (%s:%d)\n", __FILE__, __LINE__ );\
		}
	#endif // // #if FM_PLATFORM == FM_PLATFORM_WIN32
#else
	#define Assert(_Expression) ((void)0)
#endif // #ifdef _DEBUG

// 删除指针
#if !defined(FM_SAFE_DELETE)
#define FM_SAFE_DELETE(p) if((p)) { delete (p); (p)=0; }
#endif

// 删除数组
#if !defined(FM_SAFE_DELETE_ARRAY)
#define FM_SAFE_DELETE_ARRAY(p) if((p)) { delete [](p); (p)=0; }
#endif

// 删除v容器,如vector,list,set
#if !defined(FM_SAFE_DELETE_STL_V)
#define FM_SAFE_DELETE_STL_V( STL_V_CONTAINER, container ) \
	for( STL_V_CONTAINER::iterator itForDelete=(container).begin(); itForDelete!=(container).end(); ++itForDelete )\
	{\
		if( *itForDelete != NULL )\
		{\
			delete *itForDelete;\
		}\
	}\
	(container).clear()
#endif

// 删除k,v容器,如map,unordered_map
#if !defined(FM_SAFE_DELETE_STL_KV)
#define FM_SAFE_DELETE_STL_KV( STL_KV_CONTAINER, container ) \
	for( STL_KV_CONTAINER::iterator itForDelete=(container).begin(); itForDelete!=(container).end(); ++itForDelete )\
	{\
		if( itForDelete->second != NULL )\
		{\
			delete itForDelete->second;\
		}\
	}\
	(container).clear()
#endif

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


// 单件模式
#define SINGLETON_MODE( ClassName ) \
	static ClassName& GetInstance() \
	{\
		static ClassName s_##ClassName;\
		return s_##ClassName;\
	}

// namespace macro
#define NS_FM_BEGIN		namespace Freeman {
#define NS_FM_END		}
#define USING_NS_FM     using namespace Freeman


#define FmUtil_EXPORTS


/********************************************************************
created:	2012-03-25
author:		Fish (于国平)
summary:	平台相关的宏定义
*********************************************************************/
#pragma once

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#endif

// win32 header
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
# if !defined(_WIN32_WINNT)
#  define _WIN32_WINNT 0x0500
#endif
# if !defined(NOMINMAX)
#  define NOMINMAX
#endif
#  include <windows.h>
#else
#  include <string.h>
#  define MAX_PATH 260
#endif

// current platform and compiler
#define FM_PLATFORM_WIN32	1
#define FM_PLATFORM_LINUX	2
#define FM_PLATFORM_ANDROID	3
#define FM_PLATFORM_IOS		4
#define FM_PLATFORM_MAC		5

#if ( defined( WIN32 ) || defined( _WIN32 ) )
#  define FM_PLATFORM FM_PLATFORM_WIN32
#endif

// ios
#if (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
#ifndef FM_PLATFORM
    #define FM_PLATFORM	FM_PLATFORM_IOS
#endif
#endif

// android
#if defined(ANDROID)
#ifndef FM_PLATFORM
    #define FM_PLATFORM	FM_PLATFORM_ANDROID
#endif
#endif

// linux
#if defined(LINUX) || defined(__LINUX__) || defined( __unix )
#ifndef FM_PLATFORM
    #define FM_PLATFORM FM_PLATFORM_LINUX
#endif
    //# pragma message( "FM_PLATFORM_LINUX" )
#endif

// mac
#if defined(TARGET_OS_MAC)
#ifndef FM_PLATFORM
    #define FM_PLATFORM	FM_PLATFORM_MAC
#endif
#endif

#if ! FM_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif



// compiler
#define FM_COMPILER_MSVC		1
#define FM_COMPILER_GNU			2
#define FM_COMPILER_BORLAND		3
#define FM_COMPILER_WINSCW		4
#define FM_COMPILER_GCCE		5
// todo: xcode


#ifdef _MSC_VER
#  define FM_COMPILER FM_COMPILER_MSVC
#  define FM_COMPILER_VERSION _MSC_VER
#elif defined( __GNUC__ )
#  define FM_COMPILER FM_COMPILER_GNU
#  define FM_COMPILER_VERSION (((__GNUC__)*100) + \
	(__GNUC_MINOR__*10) + \
	__GNUC_PATCHLEVEL__)
	//# pragma message( "FM_COMPILER_GNU" )
#elif defined( __BORLANDC__ )
#  define FM_COMPILER FM_COMPILER_BORLAND
#  define FM_COMPILER_VERSION __BCPLUSPLUS__
#elif defined( __WINSCW__ )
#  define FM_COMPILER FM_COMPILER_WINSCW
#  define FM_COMPILER_VERSION _MSC_VER
#elif defined( __GCCE__ )
#  define FM_COMPILER FM_COMPILER_GCCE
#  define FM_COMPILER_VERSION _MSC_VER
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

// 32位还是64位
#define FM_ARCHITECTURE_32 1
#define FM_ARCHITECTURE_64 2

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define FM_ARCH_TYPE FM_ARCHITECTURE_32
#else
#   define FM_ARCH_TYPE FM_ARCHITECTURE_64
#endif


typedef unsigned int	uint32;
typedef unsigned short	uint16;
typedef unsigned char	uint8;
typedef int				int32;
typedef short			int16;
typedef char			int8;
typedef	unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned char	uchar;

#if FM_COMPILER == FM_COMPILER_MSVC
typedef unsigned __int64 uint64;
typedef __int64 int64;
#else
typedef unsigned long long uint64;
typedef long long int64;
#endif

// hashmap hash_set
#if (FM_COMPILER == FM_COMPILER_GNU )
	#if FM_COMPILER_VERSION >= 430
		#include <tr1/unordered_map>
		#include <tr1/unordered_set>
		#define UNORDERED_MAP std::tr1::unordered_map
		#define UNORDERED_SET std::tr1::unordered_set
	#else
		#include <ext/hash_map>
		#include <ext/hash_set>

		using __gnu_cxx::hash_map;
		using __gnu_cxx::hash_set;

		namespace __gnu_cxx
		{
			template<> struct hash<uint64>
			{
				size_t operator()(const uint64 &__x) const { return (size_t)__x; }
			};

			template<> struct hash<std::string>
			{
				size_t operator()( const std::string& x ) const
				{
						return hash< const char* >()( x.c_str() );
				}
			};

			template<typename T> struct hash<T *>
			{
				size_t operator()(T * const &__x) const { return (size_t)__x; }
			};

		};
		using namespace __gnu_cxx;
		#define UNORDERED_MAP hash_map
		#define UNORDERED_SET hash_set
		# pragma message( "#define UNORDERED_MAP hash_map" )
	#endif
#elif (FM_COMPILER == FM_COMPILER_MSVC)
	#if FM_COMPILER_VERSION >= 1600  // VC++ 10.0
		#include <unordered_map>
		#include <unordered_set>
		#define UNORDERED_MAP unordered_map
		#define UNORDERED_SET unordered_set
	#else
		#include <hash_set>
		#include <hash_map>
		#if _MSC_VER >= 1300
			using stdext::hash_map;
			using stdext::hash_set;
			using namespace stdext;
		#endif
		#define UNORDERED_MAP hash_map
		#define UNORDERED_SET hash_set
	#endif
#else
	#include <hash_set>
	#include <hash_map>
	#define UNORDERED_MAP hash_map
	#define UNORDERED_SET hash_set
#endif

#if FM_COMPILER == FM_COMPILER_MSVC
// 去掉烦人的_s编译警告
#define _CRT_SECURE_NO_DEPRECATE 1
#else
#define stricmp strcasecmp
#endif

// undef min max
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// 字节序
#define FM_ENDIAN_LITTLE	1
#define FM_ENDIAN_BIG		2

#ifdef FM_CONFIG_BIG_ENDIAN
#    define FM_ENDIAN FM_ENDIAN_BIG
#else
#    define FM_ENDIAN FM_ENDIAN_LITTLE
#endif


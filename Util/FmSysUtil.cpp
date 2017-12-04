#include "FmSysUtil.h"

#if FM_PLATFORM == FM_PLATFORM_WIN32
#include <windows.h>
#endif

NS_FM_BEGIN

void SysUtil::Sleep(uint ms)
{
#if FM_PLATFORM == FM_PLATFORM_WIN32
	::Sleep( ms  );
#else
	usleep( ms * 1000 );
#endif
}

uint64 SysUtil::GetMSTime()
{
#if FM_PLATFORM == FM_PLATFORM_WIN32
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	struct tm tm;
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	time_t clock = mktime(&tm);
	return (uint64)clock*1000 + (uint64)wtm.wMilliseconds;
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((uint64)tv.tv_sec * 1000) + ((uint64)tv.tv_usec / 1000);
#endif
}

time_t SysUtil::GetTime()
{
	return time(nullptr);
}

NS_FM_END
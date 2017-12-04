/********************************************************************
created:	2012-04-12
author:		Fish (于国平)
summary:	系统常用函数
*********************************************************************/
#pragma once

#include "FmConfig.h"

NS_FM_BEGIN

class FmUtil_EXPORTS SysUtil
{
public:
	// sleep
	static void Sleep( uint ms );

	// 1970年1月1日到现在的毫秒数
	static uint64 GetMSTime();

	// 获取系统时间(秒) time(NULL)
	static  time_t GetTime();

};

NS_FM_END

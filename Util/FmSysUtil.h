/********************************************************************
created:	2012-04-12
author:		Fish (�ڹ�ƽ)
summary:	ϵͳ���ú���
*********************************************************************/
#pragma once

#include "FmConfig.h"

NS_FM_BEGIN

class FmUtil_EXPORTS SysUtil
{
public:
	// sleep
	static void Sleep( uint ms );

	// 1970��1��1�յ����ڵĺ�����
	static uint64 GetMSTime();

	// ��ȡϵͳʱ��(��) time(NULL)
	static  time_t GetTime();

};

NS_FM_END

/********************************************************************
created:	2017-11-29
author:		Fish
summary:	Redis»ù´¡Àà
*********************************************************************/
#pragma once

#include <Redis/hiredis/hiredis.h>
#include "Util/FmConfig.h"

NS_FM_BEGIN

class RedisClient
{
protected:
	redisContext* m_RedisContext = nullptr;
	string m_HostIP;
	int m_HostPort = 6379;
public:
	RedisClient(const string& ip, int port);
	~RedisClient();

	const string GetHostIP() { return m_HostIP; }
	int GetHostPort() { return m_HostPort; }
	redisContext* GetRedisContext() { return m_RedisContext; }

	bool Connect();

	void Shutdown();

	void Ping();
};

NS_FM_END
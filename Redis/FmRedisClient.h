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
	string m_Password;
	int m_DbIndex = 0;
public:
	RedisClient(const string& ip, int port, const string& password, int dbIndex);
	~RedisClient();

	const string GetHostIP() { return m_HostIP; }
	int GetHostPort() { return m_HostPort; }
	int GetDbIndex() { return m_DbIndex; }

	redisContext* GetRedisContext() { return m_RedisContext; }

	bool Connect();

	void Shutdown();

	void Ping();

	bool Auth();

	bool SelectDbIndex();
};

NS_FM_END
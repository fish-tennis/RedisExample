#include "FmRedisClient.h"

NS_FM_BEGIN

RedisClient::RedisClient(const string& ip, int port)
{
	m_HostIP = ip;
	m_HostPort = port;
}

RedisClient::~RedisClient()
{
}

bool RedisClient::Connect()
{
	//struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	//m_RedisContext = redisConnectWithTimeout(m_HostIP.c_str(), m_HostPort, timeout);
	m_RedisContext = redisConnect(m_HostIP.c_str(), m_HostPort);
	redisContext* ctx = (redisContext*)m_RedisContext;
	if (m_RedisContext == nullptr || ctx->err)
	{
		if (m_RedisContext)
		{
			printf("BeginConnect error: %s\n", ctx->errstr);
		}
		else
		{
			printf("BeginConnect error: can't allocate redis context\n");
		}
		redisFree(ctx);
		m_RedisContext = nullptr;
		return false;
	}
	return true;
}

void RedisClient::Shutdown()
{
	if (m_RedisContext != nullptr)
	{
		redisFree((redisContext*)m_RedisContext);
		m_RedisContext = nullptr;
	}
}

void RedisClient::Ping()
{

}

NS_FM_END

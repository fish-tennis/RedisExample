#include "FmRedisClient.h"

NS_FM_BEGIN

RedisClient::RedisClient(const string& ip, int port, const string& password, int dbIndex)
{
	m_HostIP = ip;
	m_HostPort = port;
	m_Password = password;
	m_DbIndex = dbIndex;
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
	if (!m_Password.empty())
	{
		if (!Auth())
		{
			redisFree(ctx);
			m_RedisContext = nullptr;
			printf("auto error\n");
			return false;
		}
	}
	if (!SelectDbIndex())
	{
		redisFree(ctx);
		m_RedisContext = nullptr;
		printf("select error: index=%d\n", m_DbIndex);
		return false;
	}
	return true;
}

bool RedisClient::Auth()
{
	if (m_Password.empty())
		return true;
	bool authSucc = false;
	if (redisAppendCommand((redisContext*)m_RedisContext, "auth %s", m_Password.c_str()) == REDIS_OK)
	{
		redisReply* reply = nullptr;
		redisGetReply((redisContext*)m_RedisContext, (void**)(&reply));
		if (reply != nullptr)
		{
			if (stricmp("ok", reply->str) == 0)
				authSucc = true;
			freeReplyObject(reply);
		}
	}
	return authSucc;
}

bool RedisClient::SelectDbIndex()
{
	bool selectSucc = false;
	if (redisAppendCommand((redisContext*)m_RedisContext, "select %d", m_DbIndex) == REDIS_OK)
	{
		redisReply* reply = nullptr;
		redisGetReply((redisContext*)m_RedisContext, (void**)(&reply));
		if (reply != nullptr)
		{
			if (stricmp("ok", reply->str) == 0)
				selectSucc = true;
			freeReplyObject(reply);
		}
	}
	return selectSucc;
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
	if (redisAppendCommand((redisContext*)m_RedisContext, "ping") == REDIS_OK)
	{
		redisReply* reply = nullptr;
		redisGetReply((redisContext*)m_RedisContext, (void**)(&reply));
		if (reply != nullptr)
		{
			freeReplyObject(reply);
		}
	}
}

NS_FM_END

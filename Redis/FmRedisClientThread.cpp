#include "FmRedisClientThread.h"
#include <Util/FmLog.h>
#include "Util/FmSysUtil.h"

NS_FM_BEGIN

RedisClientThread::RedisClientThread()
{
	m_RequestQueue = new folly::ProducerConsumerQueue<RedisRequest*>(1024);
	m_ResultQueue = new folly::ProducerConsumerQueue<RedisResult*>(1024);
}

RedisClientThread::~RedisClientThread()
{
	delete m_RequestQueue;
	delete m_ResultQueue;
}

bool RedisClientThread::Init(const string& ip, int port, const string& password, int dbIndex)
{
	m_RedisClient = new RedisClient(ip, port, password, dbIndex);
	m_IsRunning = true;
	m_Thread = new std::thread(std::bind(&RedisClientThread::Run, this));
	return true;
}

void RedisClientThread::Shutdown()
{
	m_IsRunning = false;
	if (m_Thread)
	{
		m_Thread->join();
		FM_SAFE_DELETE(m_Thread);
	}
}

bool RedisClientThread::PushRequest(RedisRequest* redisRequest, RedisResultCallback resultCallback)
{
	if (redisRequest == nullptr)
	{
		Assert(false);
		return false;
	}
	redisRequest->SetSerialId(++m_CallbackSerialId);
	if (!m_RequestQueue->write(redisRequest)) // 队列满了会返回失败,丢弃该请求
	{
		DeleteRequest(redisRequest);
		return false;
	}
	if(resultCallback != nullptr )
		m_RedisResultCallbacks[m_CallbackSerialId] = resultCallback;
	return true;
}

RedisResult* RedisClientThread::PopResult()
{
	RedisResult* ret = nullptr;
	m_ResultQueue->read(ret);
	return ret;
}

void RedisClientThread::Run()
{
	FmLog("RedisClient::Run Begin");

	FmLog("Connect Phase Begin");
	while (m_IsRunning)
	{
		if (m_RedisClient->Connect())
		{
			FmLog("RedisClient Connect OK %s:%d", m_RedisClient->GetHostIP().c_str(), m_RedisClient->GetHostPort());
			break;
		}
		else
		{
			SysUtil::Sleep(1000); // reconnect after sleep
		}
	}
	FmLog("Connect Phase End");

	FmLog("Command Phase Begin");
	uint64 lastPingMS = SysUtil::GetMSTime();
	bool isReconnecting = false;
	RedisRequest* redisRequest = nullptr;
	while (m_IsRunning)
	{
		// Check Reconnect
		if (isReconnecting)
		{
			if (m_RedisClient->Connect())
			{
				FmLog("RedisClient Reconnect OK %s:%d", m_RedisClient->GetHostIP().c_str(), m_RedisClient->GetHostPort());
				isReconnecting = false;
			}
			else
			{
				SysUtil::Sleep(1000); // reconnect after sleep
				continue;
			}
		}

		if (redisRequest == nullptr)
		{
			m_RequestQueue->read(redisRequest);
		}
		if (redisRequest != nullptr)
		{
			bool isDisconnected = false;
			bool processSucc = OnProcessRequest(redisRequest, isDisconnected);
			if (!processSucc)
			{
				// todo:命令执行失败了,怎么处理?
			}
			DeleteRequest(redisRequest);
			redisRequest = nullptr;
			// 处理失败,可能掉线了
			if (isDisconnected)
			{
				isReconnecting = true; // 开始重连
#ifdef _DEBUG
				FmLog("RedisClient Reconnecting");
#endif
				continue;
			}
			lastPingMS = SysUtil::GetMSTime();
		}
		else
		{
			// 20秒没执行命令 则自己ping一下
			uint64 msTime = SysUtil::GetMSTime();
			if (msTime - lastPingMS > 1000 * 20)
			{
				m_RedisClient->Ping();
				lastPingMS = msTime;
			}
			SysUtil::Sleep(1);
		}
	}
	FmLog("Command Phase End");

	m_RedisClient->Shutdown();
	FmLog("RedisClient::Run End");
}

bool RedisClientThread::OnProcessRequest(RedisRequest* redisRequest, bool& isDisconnected)
{
	bool hasError = false;
	for (list<RedisCommand*>::iterator it=redisRequest->GetCommands().begin(); it!=redisRequest->GetCommands().end(); ++it)
	{
		RedisCommand* command = *it;
		// 普通命令
		if (command->m_BinaryData.GetOffset() == 0)
		{
			if (redisAppendCommand(m_RedisClient->GetRedisContext(), command->m_Command.c_str()) == REDIS_ERR)
			{
				hasError = true;
				break;
			}
		}
		// 二进制命令
		else
		{
			// 命令后面追加二进制数据
			command->m_Command += " %b";
			if (redisAppendCommand(m_RedisClient->GetRedisContext(), command->m_Command.c_str(), &(command->m_BinaryData.GetBuffer()[0]), command->m_BinaryData.GetOffset()) == REDIS_ERR)
			{
				hasError = true;
				break;
			}
		}
	}

	RedisResult* result = new RedisResult();
	result->SetCallbackSerialId(redisRequest->GetSerialId());
	result->GetCallbackArgs().SetBuffer(redisRequest->GetCallbackArgs().GetBuffer());
	// 错误处理
	if (hasError)
	{
		// cleanup all append commands
		redisCleanupAppendCommands(m_RedisClient->GetRedisContext());
	}
	else
	{
		for (list<RedisCommand*>::iterator it = redisRequest->GetCommands().begin(); it != redisRequest->GetCommands().end(); ++it)
		{
			redisReply* reply = nullptr;
			if (redisGetReply(m_RedisClient->GetRedisContext(), (void**)(&reply)) == REDIS_ERR
				|| (reply != nullptr && reply->type == REDIS_REPLY_ERROR)
				)
			{
				hasError = true;
				// 判断断线
				if (reply == nullptr && m_RedisClient->GetRedisContext()->err == REDIS_ERR_IO)
				{
					isDisconnected = true;
				}
				break; // 如果有多个命令执行,但是只有部分执行成功,怎么处理?
			}
			if (reply != nullptr)
				result->GetReplys().push_back(reply);
		}
	}

	while (!m_ResultQueue->write(result))
	{
		// todo:超时设置
	}
	return !hasError;
}

void RedisClientThread::Update()
{
	vector<RedisResult*> results;
	RedisResult* ret = nullptr;
	while (m_ResultQueue->read(ret))
	{
		results.push_back(ret);
	}
	for (size_t i = 0; i < results.size(); ++i)
	{
		RedisResult* result = results[i];
		OnProcessResult(result);
		DeleteResult(result);
	}
}

bool RedisClientThread::OnProcessResult(RedisResult* redisResult)
{
	RedisResultCallbackMap::iterator it = m_RedisResultCallbacks.find(redisResult->GetCallbackSerialId());
	if (it == m_RedisResultCallbacks.end())
	{
		FmLog("OnProcessResult NotFindCallback serialId=%llu", redisResult->GetCallbackSerialId());
		return false;
	}
	RedisResultCallback callback = it->second;
	if(callback)
		callback(redisResult);
	m_RedisResultCallbacks.erase(redisResult->GetCallbackSerialId());
	return true;
}

void RedisClientThread::DeleteRequest(RedisRequest* redisRequest)
{
	delete redisRequest;
}

void RedisClientThread::DeleteResult(RedisResult* redisResult)
{
	delete redisResult;
}

NS_FM_END

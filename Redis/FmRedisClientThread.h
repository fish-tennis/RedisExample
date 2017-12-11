/********************************************************************
created:	2017-11-29
author:		Fish
summary:	RedisClient放在一个线程中执行
*********************************************************************/
#pragma once

#include <Util/FmConfig.h>
#include <Util/FmStream.h>
#include <thread>
#include <Redis/hiredis/hiredis.h>
#include "FmRedisClient.h"
#include "FmRedisData.h"
#include "Util/ProducerConsumerQueue.h"

NS_FM_BEGIN

class RedisClientThread
{
protected:
	std::thread* m_Thread = nullptr;		// 处理线程
	volatile bool m_IsRunning = false;		// 线程执行状态
	RedisClient* m_RedisClient = nullptr;	// 暂时一个线程对应一个RedisClient

	folly::ProducerConsumerQueue<RedisRequest*>* m_RequestQueue; // 请求队列
	folly::ProducerConsumerQueue<RedisResult*>* m_ResultQueue;	 // 结果队列

	typedef UNORDERED_MAP< uint64, RedisResultCallback > RedisResultCallbackMap;
	RedisResultCallbackMap m_RedisResultCallbacks; // 回调hash表
	uint64 m_CallbackSerialId = 0;
public:
	RedisClientThread();
	~RedisClientThread();

	bool Init(const string& ip, int port, const string& password, int dbIndex);

	void Shutdown();

	// 提交一个redis请求
	// 注意:RedisRequest必须是new出来的,因为内部会delete redisRequest
	bool PushRequest(RedisRequest* redisRequest, RedisResultCallback resultCallback);

	void Update();

protected:
	void Run();

	// 取出一个redis结果
	RedisResult* PopResult();

	bool OnProcessRequest(RedisRequest* redisRequest, bool& isDisconnected);

	bool OnProcessResult(RedisResult* redisResult);

	void DeleteRequest(RedisRequest* redisRequest);
	void DeleteResult(RedisResult* redisResult);
};

NS_FM_END
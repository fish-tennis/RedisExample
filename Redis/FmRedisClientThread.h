/********************************************************************
created:	2017-11-29
author:		Fish
summary:	RedisClient����һ���߳���ִ��
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
	std::thread* m_Thread = nullptr;		// �����߳�
	volatile bool m_IsRunning = false;		// �߳�ִ��״̬
	RedisClient* m_RedisClient = nullptr;	// ��ʱһ���̶߳�Ӧһ��RedisClient

	folly::ProducerConsumerQueue<RedisRequest*>* m_RequestQueue; // �������
	folly::ProducerConsumerQueue<RedisResult*>* m_ResultQueue;	 // �������

	typedef UNORDERED_MAP< uint64, RedisResultCallback > RedisResultCallbackMap;
	RedisResultCallbackMap m_RedisResultCallbacks; // �ص�hash��
	uint64 m_CallbackSerialId = 0;
public:
	RedisClientThread();
	~RedisClientThread();

	bool Init(const string& ip, int port, const string& password, int dbIndex);

	void Shutdown();

	// �ύһ��redis����
	// ע��:RedisRequest������new������,��Ϊ�ڲ���delete redisRequest
	bool PushRequest(RedisRequest* redisRequest, RedisResultCallback resultCallback);

	void Update();

protected:
	void Run();

	// ȡ��һ��redis���
	RedisResult* PopResult();

	bool OnProcessRequest(RedisRequest* redisRequest, bool& isDisconnected);

	bool OnProcessResult(RedisResult* redisResult);

	void DeleteRequest(RedisRequest* redisRequest);
	void DeleteResult(RedisResult* redisResult);
};

NS_FM_END
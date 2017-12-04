/********************************************************************
created:	2017-11-29
author:		Fish
summary:	RedisClient����һ���߳���ִ��
*********************************************************************/
#pragma once

#include <Util/FmConfig.h>
#include <Util/FmStream.h>
#include <Redis/hiredis/hiredis.h>

NS_FM_BEGIN

class RedisCommand
{
public:
	string m_Command;				// �Ƕ���������ֱ�����������
	StrStream m_BinaryData;			// ����������,������װ: (m_Command %b), m_BinaryData.Buff, m_BinaryData.Len
};

// һ��redis��������,����ͬʱ���Ͷ������
class RedisRequest
{
protected:
	uint64 m_SerialId = 0;
	StrStream m_CallbackArgs;	// �ص����� Ҳ������չ��void* customData
	list<RedisCommand*> m_Commands;
public:
	RedisRequest();
	~RedisRequest();

	// �ص�����
	StrStream& GetCallbackArgs() { return m_CallbackArgs; }
	uint64 GetSerialId() { return m_SerialId; }
	void SetSerialId(uint64 serialId) { m_SerialId = serialId; }

	// ׷��һ������
	RedisCommand* AppendCommand(const string& command );

	list<RedisCommand*>& GetCommands() { return m_Commands; }
};


// һ��redis�������,����ͬʱ��ȡ�������ķ��ؽ��
class RedisResult
{
protected:
	uint64 m_CallbackSerialId = 0;
	StrStream m_CallbackArgs;	   // �ص����� Ҳ������չ��void* customData
	list<redisReply*> m_Replys;
public:
	RedisResult() {}
	~RedisResult();

	// �ص�����
	StrStream& GetCallbackArgs() { return m_CallbackArgs; }
	uint64 GetCallbackSerialId() { return m_CallbackSerialId; }
	void SetCallbackSerialId(uint64 serialId) { m_CallbackSerialId = serialId; }

	list<redisReply*>& GetReplys() { return m_Replys; }
};

// �ص�����
typedef std::function<void(RedisResult* redisResult)> RedisResultCallback;

NS_FM_END
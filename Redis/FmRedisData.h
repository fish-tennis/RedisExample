/********************************************************************
created:	2017-11-29
author:		Fish
summary:	RedisClient放在一个线程中执行
*********************************************************************/
#pragma once

#include <Util/FmConfig.h>
#include <Util/FmStream.h>
#include <Redis/hiredis/hiredis.h>

NS_FM_BEGIN

class RedisCommand
{
public:
	string m_Command;				// 非二进制数据直接用命令完成
	StrStream m_BinaryData;			// 二进制数据,命令组装: (m_Command %b), m_BinaryData.Buff, m_BinaryData.Len
};

// 一次redis操作请求,可以同时发送多个命令
class RedisRequest
{
protected:
	uint64 m_SerialId = 0;
	StrStream m_CallbackArgs;	// 回调数据 也可以扩展出void* customData
	list<RedisCommand*> m_Commands;
public:
	RedisRequest();
	~RedisRequest();

	// 回调数据
	StrStream& GetCallbackArgs() { return m_CallbackArgs; }
	uint64 GetSerialId() { return m_SerialId; }
	void SetSerialId(uint64 serialId) { m_SerialId = serialId; }

	// 追加一条命令
	RedisCommand* AppendCommand(const string& command );

	list<RedisCommand*>& GetCommands() { return m_Commands; }
};


// 一次redis操作结果,可以同时收取多个命令的返回结果
class RedisResult
{
protected:
	uint64 m_CallbackSerialId = 0;
	StrStream m_CallbackArgs;	   // 回调数据 也可以扩展出void* customData
	list<redisReply*> m_Replys;
public:
	RedisResult() {}
	~RedisResult();

	// 回调数据
	StrStream& GetCallbackArgs() { return m_CallbackArgs; }
	uint64 GetCallbackSerialId() { return m_CallbackSerialId; }
	void SetCallbackSerialId(uint64 serialId) { m_CallbackSerialId = serialId; }

	list<redisReply*>& GetReplys() { return m_Replys; }
};

// 回调函数
typedef std::function<void(RedisResult* redisResult)> RedisResultCallback;

NS_FM_END
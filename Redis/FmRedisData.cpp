#include "FmRedisData.h"

NS_FM_BEGIN

RedisRequest::RedisRequest()
{
}

RedisRequest::~RedisRequest()
{
	for (list<RedisCommand*>::iterator it=m_Commands.begin(); it!=m_Commands.end(); ++it)
	{
		delete *it;
	}
	m_Commands.clear();
}

RedisCommand* RedisRequest::AppendCommand(const string& command)
{
	RedisCommand* cmd = new RedisCommand();
	cmd->m_Command = command;
	m_Commands.push_back(cmd);
	return cmd;
}

RedisResult::~RedisResult()
{
	for (list<redisReply*>::iterator it=m_Replys.begin(); it!=m_Replys.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			freeReplyObject((*it));
		}
	}
	m_Replys.clear();
}

NS_FM_END

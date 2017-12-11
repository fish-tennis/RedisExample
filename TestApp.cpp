#include "TestApp.h"
#include "Util/FmLog.h"

#include "TestObject.h"

NS_FM_BEGIN

TestApp::TestApp()
{
}

TestApp::~TestApp()
{

}

// testapp 127.0.0.1 6379 password dbindex
bool TestApp::OnInit( int argc, char **argv )
{
	string host = "127.0.0.1";
	int port = 6379;
	string password = "";
	int dbIndex = 0;
	if (argc > 1)
	{
		host = argv[1];
	}
	if (argc > 2)
	{
		port = atoi(argv[2]);
	}
	if (argc > 3)
	{
		password = argv[3];
	}
	if (argc > 4)
	{
		port = atoi(argv[4]);
	}
	m_RedisClientThread.Init(host, port, password, dbIndex);
	return true;
}

bool TestApp::OnRunning()
{
	m_RedisClientThread.Update();
	return true;
}

bool TestApp::OnShutdown()
{
	FmLog( "OnShutdown begin" );
	m_RedisClientThread.Shutdown();
	FmLog( "OnShutdown end" );
	return true;
}

void TestApp::OnCommand( const std::string& command )
{
	if( command == "quit" )
	{
		SetRunning(false);
		FmLog("Shutdown ...\n");
	}
	else if (command == "test")
	{
		// 普通的hset
		RedisRequest* redisRequest = new RedisRequest();
		redisRequest->AppendCommand("hset testhashtable key1 value1");
		redisRequest->AppendCommand("hset testhashtable key2 value2"); // 可以多个命令放在一个RedisRequest里
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			// GetReplys().size() = AppendCommand的命令数
			if (redisResult->GetReplys().size() == 2)
			{
				for (list<redisReply*>::iterator it=redisResult->GetReplys().begin(); it!=redisResult->GetReplys().end(); ++it)
				{
					redisReply* reply = *it;
					FmLog("hset testhashtable type=%d str=%s int=%llu", reply->type, reply->str, reply->integer);
				}
			}
			else
			{
				FmLog("hset testhashtable key1 value1 Error");
			}
		});

		// hset 二进制数据
		TestObject testObj;
		testObj.m_Int = 123;
		testObj.m_Name = "TestName";
		testObj.m_Map[101] = 2;
		testObj.m_Map[203] = 3;
		redisRequest = new RedisRequest();
		RedisCommand* command = redisRequest->AppendCommand("hset testhashtable binaryKey");
		command->m_BinaryData << testObj;
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			if (!redisResult->GetReplys().empty())
			{
				FmLog("hset testhashtable binaryKey type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
			}
			else
			{
				FmLog("hset testhashtable binaryKey Error");
			}
		});

		// 普通的hget
		redisRequest = new RedisRequest();
		redisRequest->AppendCommand("hget testhashtable key1");
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			if (!redisResult->GetReplys().empty())
			{
				FmLog("hget testhashtable key1 type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
			}
			else
			{
				FmLog("hset testhashtable key1 Error");
			}
		});

		// hget二进制数据
		uint64 callbackData = 1357911;
		redisRequest = new RedisRequest();
		redisRequest->AppendCommand("hget testhashtable binaryKey");
		redisRequest->GetCallbackArgs() << callbackData;
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			uint64 retCallbackData = 0;
			redisResult->GetCallbackArgs() >> retCallbackData;
			if (!redisResult->GetReplys().empty())
			{
				FmLog("hget testhashtable binaryKey type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
				TestObject newTestObj;
				MemStream stream((uint8*)redisResult->GetReplys().front()->str, (size_t)redisResult->GetReplys().front()->len);
				stream >> newTestObj;
				FmLog("newTestObj int=%d Name=%s MapSize=%d retCallbackData=%llu",
					newTestObj.m_Int, newTestObj.m_Name.c_str(), (int)newTestObj.m_Map.size(), retCallbackData);
			}
			else
			{
				FmLog("hget testhashtable binaryKey Error");
			}
		});
	}

	// hashlistinit和hashlistcheck模拟了一个实际项目中的需求
	// 在hash表里缓存一些临时数据,并定时对hash表进行清理(删除最早缓存的数据)
	// 通过一个list来记录插入到hash表中的key的顺序

	// hashlistinit用来创建一个有110个key的hash表
	else if (command == "hashlistinit")
	{
		RedisRequest* initRequest = new RedisRequest();
		initRequest->AppendCommand("del testhash");
		initRequest->AppendCommand("del testlist");
		m_RedisClientThread.PushRequest(initRequest, nullptr);

		char strCmd[256] = {};
		for ( int i=0; i<110; ++i )
		{
			uint64 key = i+1;
			RedisRequest* redisRequest = new RedisRequest();
			sprintf(strCmd, "hset testhash %llu %d", key, 100+i+1);
			redisRequest->AppendCommand(strCmd);
			sprintf(strCmd, "lpush testlist %llu", key);
			redisRequest->AppendCommand(strCmd);
			m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
				//FmLog("hset testhashtable key1 value1 type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
			});
		}
	}
	// hashlistcheck用来检查hash表是否超过大小并删除旧key
	else if (command == "hashlistcheck")
	{
		static const int maxHashSize = 100;
		char strCmd[256] = {};
		RedisRequest* redisRequest = new RedisRequest();
		sprintf(strCmd, "lrange testlist %d -1", maxHashSize);
		redisRequest->AppendCommand(strCmd);
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			if (!redisResult->GetReplys().empty() && redisResult->GetReplys().front() != nullptr)
			{
				redisReply* reply = redisResult->GetReplys().front();
				if (reply->elements > 0)
				{
					// 批量删除hash表中多的项
					string delCmd = "hdel testhash";
					for (size_t i = 0; i < reply->elements; ++i)
					{
						redisReply* elementReply = reply->element[i];
						delCmd += " ";
						delCmd += elementReply->str;
						FmLog("lrange %d type=%d str=%s int=%llu", (int)i, elementReply->type, elementReply->str, elementReply->integer);
					}
					RedisRequest* delRequest = new RedisRequest();
					delRequest->AppendCommand(delCmd);
					FmLog("delCmd:%s", delCmd.c_str());
					TestApp::GetInstance().m_RedisClientThread.PushRequest(delRequest, [](RedisResult* delResult) {
						if (!delResult->GetReplys().empty())
						{
							FmLog("hdel testhash type=%d str=%s int=%llu", delResult->GetReplys().front()->type, delResult->GetReplys().front()->str, delResult->GetReplys().front()->integer);
						}
						else
						{
							FmLog("hdel testhash Error");
						}
					});
					// list清除
					delRequest = new RedisRequest();
					char trimCmd[256] = {};
					sprintf(trimCmd, "ltrim testlist 0 %d", maxHashSize-1); // 只保留[0,maxHashSize)的key
					delRequest->AppendCommand(trimCmd);
					TestApp::GetInstance().m_RedisClientThread.PushRequest(delRequest, [](RedisResult* delResult) {
						if (!delResult->GetReplys().empty())
						{
							FmLog("ltrim testlist type=%d str=%s int=%llu", delResult->GetReplys().front()->type, delResult->GetReplys().front()->str, delResult->GetReplys().front()->integer);
						}
						else
						{
							FmLog("ltrim testlist Error");
						}
					});
				}
			}
		});
	}
	else
	{
		FmLog("Error command");
	}
}

NS_FM_END
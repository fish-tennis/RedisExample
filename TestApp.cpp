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

bool TestApp::OnInit( int argc, char **argv )
{
	m_RedisClientThread.Init("10.8.230.19", 6379);
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
		// 普通的set
		RedisRequest* redisRequest = new RedisRequest();
		redisRequest->AppendCommand("hset testhashtable key1 value1");
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			FmLog("hset testhashtable key1 value1 type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
		});

		// set 二进制数据
		TestObject testObj;
		testObj.m_DeckIndex = 123;
		testObj.m_DeckName = "TestDeckName";
		testObj.m_HeroCardId = 1104;
		testObj.m_MainCards[101] = 2;
		testObj.m_MainCards[203] = 3;
		redisRequest = new RedisRequest();
		RedisCommand* command = redisRequest->AppendCommand("hset testhashtable binaryKey");
		command->m_BinaryData << testObj;
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			FmLog("hset testhashtable binaryKey type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
		});

		// 普通的get
		redisRequest = new RedisRequest();
		redisRequest->AppendCommand("hget testhashtable key1");
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			FmLog("hget testhashtable key1 type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
		});

		// get二进制数据
		uint64 callbackData = 1357911;
		redisRequest = new RedisRequest();
		redisRequest->AppendCommand("hget testhashtable binaryKey");
		redisRequest->GetCallbackArgs() << callbackData;
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			FmLog("hget testhashtable binaryKey type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
			TestObject newTestObj;
			MemStream stream((uint8*)redisResult->GetReplys().front()->str, (size_t)redisResult->GetReplys().front()->len);
			stream >> newTestObj;
			uint64 retCallbackData = 0;
			redisResult->GetCallbackArgs() >> retCallbackData;
			FmLog("newTestObj DeckIndex=%d DeckName=%s HeroCardId=%d MapSize=%d retCallbackData=%llu",
				newTestObj.m_DeckIndex, newTestObj.m_DeckName.c_str(), newTestObj.m_HeroCardId, (int)newTestObj.m_MainCards.size(), retCallbackData);
		});
	}
	// hash和list共同维护一份数据
	// list用来控制hash里面的key的数量,删除旧的key
	else if (command == "hashlist")
	{
		char strCmd[256] = {};
		//for ( int i=0; i<100; ++i )
		//{
		//	uint64 key = i+1;
		//	RedisRequest* redisRequest = new RedisRequest();
		//	sprintf(strCmd, "hset testhash %llu %d", key, 100+i+1);
		//	redisRequest->AppendCommand(strCmd);
		//	sprintf(strCmd, "lpush testlist %llu", key);
		//	redisRequest->AppendCommand(strCmd);
		//	redisRequest->GetCallbackArgs() << key;
		//	m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
		//		//FmLog("hset testhashtable key1 value1 type=%d str=%s int=%llu", redisResult->GetReplys().front()->type, redisResult->GetReplys().front()->str, redisResult->GetReplys().front()->integer);
		//	});
		//}

		static const int maxHashSize = 80;
		RedisRequest* redisRequest = new RedisRequest();
		sprintf(strCmd, "lrange testlist %d -1", maxHashSize);
		redisRequest->AppendCommand(strCmd);
		m_RedisClientThread.PushRequest(redisRequest, [](RedisResult* redisResult) {
			if (!redisResult->GetReplys().empty() && redisResult->GetReplys().front() != nullptr)
			{
				redisReply* reply = redisResult->GetReplys().front();
				if (reply->elements > 0)
				{
					// 删除hash表中多的项
					char delCmd[1024] = {};
					sprintf(delCmd, "hdel testhash");
					for (size_t i = 0; i < reply->elements; ++i)
					{
						redisReply* elementReply = reply->element[i];
						sprintf(delCmd, " %s %s", delCmd, elementReply->str);
						FmLog("lrange %d type=%d str=%s int=%llu", (int)i, elementReply->type, elementReply->str, elementReply->integer);
					}
					RedisRequest* delRequest = new RedisRequest();
					delRequest->AppendCommand(delCmd);
					TestApp::GetInstance().m_RedisClientThread.PushRequest(delRequest, [](RedisResult* delResult) {
						FmLog("hdel testhash type=%d str=%s int=%llu", delResult->GetReplys().front()->type, delResult->GetReplys().front()->str, delResult->GetReplys().front()->integer);
					});
					// list清除
					delRequest = new RedisRequest();
					sprintf(delCmd, "ltrim testlist 0 %d", maxHashSize-1);
					delRequest->AppendCommand(delCmd);
					TestApp::GetInstance().m_RedisClientThread.PushRequest(delRequest, [](RedisResult* delResult) {
						FmLog("ltrim testlist type=%d str=%s int=%llu", delResult->GetReplys().front()->type, delResult->GetReplys().front()->str, delResult->GetReplys().front()->integer);
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
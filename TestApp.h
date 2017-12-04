/********************************************************************
created:	2017-11-29
author:		Fish (于国平)
summary:	测试程序框架
*********************************************************************/
#pragma once

#include "Util/FmBaseApp.h"

#include "Redis/FmRedisClientThread.h"

NS_FM_BEGIN

class TestApp : public BaseApp
{
protected:
	RedisClientThread m_RedisClientThread;
public:
	TestApp();
	~TestApp();

	SINGLETON_MODE( TestApp );

	// 初始化
	virtual bool OnInit( int argc, char **argv );

	// 运行
	virtual bool OnRunning();

	// 关闭
	virtual bool OnShutdown();

	// 响应控制台命令
	virtual void OnCommand( const std::string& command );
};

NS_FM_END
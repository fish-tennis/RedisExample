/********************************************************************
author:		Fish (于国平)
summary:	应用程序基本框架
*********************************************************************/
#pragma once

#include "FmConfig.h"
#include <thread>
#include "ProducerConsumerQueue.h"

NS_FM_BEGIN

class FmUtil_EXPORTS BaseApp
{
protected:
	volatile bool m_IsRunning = false;			// 是否运行状态
	volatile bool m_IsCommandRunning = false;	// 是否接受输入指令

	folly::ProducerConsumerQueue<string>* m_Commands; // 输入的命令

	static BaseApp*		s_BaseApp;
public:
	BaseApp();
	virtual ~BaseApp();

	bool IsRunning() { return m_IsRunning; }
	void SetRunning( bool isRunning ) { m_IsRunning = isRunning; }

	static BaseApp* GetTheApp() { return s_BaseApp; }

	// 初始化
	virtual bool Init( int argc, char **argv );
	// 父类继承该初始化接口
	virtual bool OnInit( int argc, char **argv ) = 0;

	// 运行
	virtual bool Run();
	// 父类继承该运行接口
	virtual bool OnRunning() = 0;

	// 关闭
	virtual bool Shutdown();
	// 父类继承该关闭接口
	virtual bool OnShutdown() = 0;

	// 输入命令线程
	void InputCommandThread();

	// 父类继承该命令接口
	virtual void OnCommand( const std::string& command );

	// 初始化守护进程(linux)
	static bool InitDaemon( int argc, char **argv );

	// 信号回调(linux)
	static void SignalCallback( int signal );
};

NS_FM_END

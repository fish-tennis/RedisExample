#include "FmBaseApp.h"
#include "FmLog.h"
#include "FmSysUtil.h"

#if FM_PLATFORM == FM_PLATFORM_LINUX
#include <signal.h>
#include <sys/param.h>
#endif

NS_FM_BEGIN

BaseApp* BaseApp::s_BaseApp = nullptr;

BaseApp::BaseApp()
{
	Assert( !s_BaseApp );
	m_Commands = new folly::ProducerConsumerQueue<string>(32);
}

BaseApp::~BaseApp()
{
	delete m_Commands;
}

bool BaseApp::Init( int argc, char **argv )
{
	if( argc == 2 )
	{
	}
	return OnInit( argc, argv );
}

bool BaseApp::Run()
{
	m_IsRunning = true;

	// 开一个线程来获取用户的输入命令
	std::thread inputCommandThread(std::bind(&BaseApp::InputCommandThread, this));
	inputCommandThread.detach();

	while( m_IsRunning )
	{

		string curCmd;
		if(m_Commands->read(curCmd))
		{
			FmLog("OnCommand:%s", curCmd.c_str());
			OnCommand(curCmd);
		}

		if( !m_IsRunning )
			break;
		if( !OnRunning() )
		{
			break;
		}
		SysUtil::Sleep(1);
	}

	return true;
}

bool BaseApp::Shutdown()
{
	m_IsRunning = false;
	bool result = OnShutdown();
	return result;
}

void BaseApp::InputCommandThread()
{
	FmLog("InputCommandThread Begin");
	char command[256] = { NULL };
	m_IsCommandRunning = true;
	while( m_IsRunning )
	{
		if( m_IsCommandRunning )
		{
			cin >> command;
			// 先停止接受输入,防止线程无法终止
			// 逻辑部分可以再把m_IsCommandRunning设置为true
			if( strcmp( command, "quit" ) == 0 )
			{
				m_IsCommandRunning = false;
			}
			m_Commands->write(command);
		}
		SysUtil::Sleep(100);
	}
	FmLog("InputCommandThread End");
}

void BaseApp::OnCommand( const std::string& command )
{
	if( command == "quit" )
	{
		SetRunning( false );
		FmLog( "Shutdown ...\n");
	}
}

bool BaseApp::InitDaemon( int argc, char **argv )
{
	return false;
}

void BaseApp::SignalCallback( int signal )
{
#if FM_PLATFORM == FM_PLATFORM_LINUX
	//FmLog( "SignalCallback signal=%d", signal );
	if( signal == SIGUSR1 || signal == SIGKILL )
	{
		BaseApp* app = BaseApp::GetTheApp();
		if( app )
		{
			app->m_Commands->write("quit");
			//FmLog( "SignalCallback push cmd quit" );
		}
	}
#endif
}

NS_FM_END

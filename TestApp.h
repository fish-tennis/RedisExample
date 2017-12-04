/********************************************************************
created:	2017-11-29
author:		Fish (�ڹ�ƽ)
summary:	���Գ�����
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

	// ��ʼ��
	virtual bool OnInit( int argc, char **argv );

	// ����
	virtual bool OnRunning();

	// �ر�
	virtual bool OnShutdown();

	// ��Ӧ����̨����
	virtual void OnCommand( const std::string& command );
};

NS_FM_END
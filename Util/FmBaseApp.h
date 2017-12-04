/********************************************************************
author:		Fish (�ڹ�ƽ)
summary:	Ӧ�ó���������
*********************************************************************/
#pragma once

#include "FmConfig.h"
#include <thread>
#include "ProducerConsumerQueue.h"

NS_FM_BEGIN

class FmUtil_EXPORTS BaseApp
{
protected:
	volatile bool m_IsRunning = false;			// �Ƿ�����״̬
	volatile bool m_IsCommandRunning = false;	// �Ƿ��������ָ��

	folly::ProducerConsumerQueue<string>* m_Commands; // ���������

	static BaseApp*		s_BaseApp;
public:
	BaseApp();
	virtual ~BaseApp();

	bool IsRunning() { return m_IsRunning; }
	void SetRunning( bool isRunning ) { m_IsRunning = isRunning; }

	static BaseApp* GetTheApp() { return s_BaseApp; }

	// ��ʼ��
	virtual bool Init( int argc, char **argv );
	// ����̳иó�ʼ���ӿ�
	virtual bool OnInit( int argc, char **argv ) = 0;

	// ����
	virtual bool Run();
	// ����̳и����нӿ�
	virtual bool OnRunning() = 0;

	// �ر�
	virtual bool Shutdown();
	// ����̳иùرսӿ�
	virtual bool OnShutdown() = 0;

	// ���������߳�
	void InputCommandThread();

	// ����̳и�����ӿ�
	virtual void OnCommand( const std::string& command );

	// ��ʼ���ػ�����(linux)
	static bool InitDaemon( int argc, char **argv );

	// �źŻص�(linux)
	static void SignalCallback( int signal );
};

NS_FM_END

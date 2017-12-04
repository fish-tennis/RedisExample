#include "TestApp.h"
#include "Util/FmLog.h"
using namespace Freeman;

int main(int argc, char **argv)
{
#ifdef _WIN32
	//! Windows netword DLL init
	WORD version = MAKEWORD(2, 2);
	WSADATA data;

	if (WSAStartup(version, &data) != 0) {
		std::cerr << "WSAStartup() failure" << std::endl;
		return -1;
	}
#endif

	TestApp& app = TestApp::GetInstance();
	if (!app.Init(argc, argv))
	{
		return 0;
	}
	app.Run();
	app.Shutdown();
	return 0;
}
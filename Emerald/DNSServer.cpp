//DNS Server
#if 1
#include "Emerald.h"
#include <map>
#include <iostream>

using namespace std;

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = true;					//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	EEDNSServer server("127.0.0.1", 53);

	while (EERun())
	{
		server.Process();
	}

	EEShutdown();
	return 0;
}







#endif
//Send recorder by UDP(Server)
#if 0
#include "Emerald.h"

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

	EEUDPServer server("192.168.229.1", 23333);

	sockaddr_in addr;
	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		std::string data;
		if (server.Recv(&addr, data))
		{
			server.Send(&addr,data);
		}

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
// TCP Demo
#if 0
#include "../Emerald.h"

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

	EEFont word1(FLOAT3(100.0f, 100.0f, 0.0f), EEColor::YELLOW, "send:");
	EEFont word2(FLOAT3(100.0f, 200.0f, 0.0f), EEColor::YELLOW, "recv:");

	EETCPServer server("192.168.229.1", 23333);
	EETCPClient client("192.168.229.1", 23333);


	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		server.Process();
		std::string buf;
		std::cin >> buf;
		server.Send(buf);
		word1.AddText(buf.c_str());
		std::string asd;
		client.Recv(asd);
		word2.AddText(asd.c_str());
		word1.Process();
		word2.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
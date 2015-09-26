// Thread Demo
#if 0
#include "../Emerald.h"

using namespace std;

void test()
{
	try
	{
		while (true)
		{
			std::cout << "asd";
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
	catch (boost::thread_interrupted&)
	{
		return;
	}
}

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

	std::cout << boost::this_thread::get_id() << std::endl;
	std::cout << boost::thread::hardware_concurrency() << std::endl;
	boost::thread *temp = new boost::thread(boost::bind(&test));
	temp->interrupt();
	//temp.detach();
	//temp.join();

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
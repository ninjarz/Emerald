//Points Demo
#if 1
#include "Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = false;					//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	std::vector<FLOAT2> pointsData;
	for (float i = 0; i < 800; ++i)
		pointsData.push_back(FLOAT2(i, 200.f));
	EEPoints2D points(pointsData);
	points.SetColor(EEColor::RED);


	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		EEShowFPSInTitle(L"- -");

		points.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
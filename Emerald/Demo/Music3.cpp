// Music Demo
#if 0
#include "../Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEMusic music;
	music.AsyncLoadMusic("Music/zhntn.mp3");
	music.SetFrequencyRatio(1.f);
	music.Start();

	//music.Open("Music/zhntn.mp3");
	//music.Play();
	//music.Play(0.5, 0.1, 2);




	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = true;					//是开启抗锯齿
	desc.isVsync = true;				//是否垂直同步
	EEInitialize(desc);

	float tdWidth = 1.f;
	EEScene scene;
	std::vector<EEQuad2D> tdQuads(512);
	for (auto& tdQuad : tdQuads)
	{
		tdQuad.SetPositionY((float)desc.height / 2);
		tdQuad.SetWidth(tdWidth);
		tdQuad.SetColor(EEColor::RED);
		scene.AddObject(&tdQuad);
	}
	int loopPos = 0;

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		auto samples = music.GetCurrentLeftSamplesDouble(512);
		if (samples.size() == 512)
		{
			std::vector<std::complex<double>> fd(512);
			EEFFT(samples, fd, 9);
			for (unsigned int i = 1; i < samples.size(); ++i)
			{
				tdQuads[loopPos].SetPositionX(desc.width - scene.GetPositionX());
				tdQuads[loopPos].SetHeight(sqrt(fd[i].real() * fd[i].real() + fd[i].imag() * fd[i].imag()) / 8000);
				scene.SetPositionX(scene.GetPositionX() - tdWidth);

				loopPos = ++loopPos % tdQuads.size();
			}
		}
		scene.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
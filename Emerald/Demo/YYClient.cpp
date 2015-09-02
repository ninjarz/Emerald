#if 0
#include "Emerald.h"
#include <string>

using namespace std;

EEUDPClient g_client("fe80::11ea:39a6:7ae1:187c%38", 23333);
bool g_flag(false);
string g_name;
void ConfirmName()
{
	g_flag = true;
}

void CALLBACK RecorderInProc(HWAVEIN _waveIn, UINT _msg, DWORD_PTR _instance, DWORD_PTR _param1, DWORD_PTR _param2)
{
	LPWAVEHDR waveInHdr = (LPWAVEHDR)_param1;
	if (WIM_DATA == _msg)
	{
		LPWAVEHDR waveInHdr = (LPWAVEHDR)_param1;

		char code[60];
		EEWaveCoder::WaveEncode(waveInHdr->lpData, EE_RECORDER_FRAME_SIZE, code, NULL);
		std::string data = "<name>";
		data.append(g_name);
		data.append("<name><wave>");
		data.append(code, 60);
		data.append("<wave>");

		g_client.Send(data);

		waveInAddBuffer(_waveIn, (LPWAVEHDR)_param1, sizeof(WAVEHDR));
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

	//order 10, time 0 - +∞
	EETexture bgTex(L"Texture\\主界面\\主界面背景.jpg");
	EEScene *mainScene = new EEScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	mainScene->SetLocalZOrder(10.0f);

	EETexture button1Tex(L"Texture/主界面/模式标签/自由模式.png");
	EEButton *button1 = new EEButton(EE_BUTTON_SCALE, Rect_Float(320.f, 280.f, 370.f, 330.f), 1.3f, 0.2f, 0.2f, button1Tex, (DWORD_PTR)ConfirmName);
	button1->SetLocalZOrder(5.f);
	mainScene->AddObject(button1);

	EELineEditer *lineEditer = new EELineEditer(Rect_Float(200.f, 300.f, 300.f, 315.f), EEColor::WHITE, EEColor::BLACK);
	lineEditer->SetLocalZOrder(3.f);
	mainScene->AddObject(lineEditer);
	while (EERun())
	{
		EEBeginScene(EEColor::WHITE);

		EEProcess(mainScene);
		button1->Process();
		if (g_flag)
		{
			g_name = lineEditer->GetText();
			cout << g_name;
			break;
		}

		EEEndScene();
	}
	if (!g_flag)
		return 0;
	mainScene->RemoveObject(button1);
	mainScene->RemoveObject(lineEditer);



	//order 9, time 1 - +∞
	EETexture round1Tex(L"Texture\\主界面\\圆形图案1.png");
	EEQuad2D *round1Quad = new EEQuad2D(Rect_Float(70, 75, 370, 375), round1Tex);
	round1Quad->SetLocalZOrder(9.f);
	round1Quad->SetAlpha(0.0f);
	EEFade(round1Quad, 1.0f, 1.0f, 1.0f);
	EERotateYX(round1Quad, 16.0f, 2 * EE_2PI, 1.0f, true);
	mainScene->AddObject(round1Quad);

	//order 9, time 2 - +∞
	EETexture round2Tex(L"Texture\\主界面\\圆形图案2（滤色）.png");
	EEQuad2D *round2Quad = new EEQuad2D(Rect_Float(0, 5, 440, 445), round2Tex);
	round2Quad->SetLocalZOrder(9.f);
	round2Quad->SetAlpha(0.0f);
	EEFade(round2Quad, 1.0f, 1.0f, 2.0f);
	EERotateYX(round2Quad, 16.0f, -2 * EE_2PI, 2.0f, true);
	mainScene->AddObject(round2Quad);

	//order 8, time 3.5 - +∞
	EETexture road1Tex(L"Texture\\主界面\\线路形图案1（滤色）.png");
	EEQuad2D *road1Quad = new EEQuad2D(Rect_Float(300, 65, 500, 385), road1Tex);
	road1Quad->SetLocalZOrder(8.f);
	road1Quad->SetAlpha(0.0f);
	EEFade(road1Quad, 1.0f, 1.0f, 3.5f);
	//mainScene->AddObject(road1Quad);

	//order 8, time 3.0 - +∞
	EETexture road2Tex(L"Texture\\主界面\\线路形图案2（滤色）.png");
	EEQuad2D *road2Quad = new EEQuad2D(Rect_Float(40, 45, 600, 405), road2Tex);
	road2Quad->SetLocalZOrder(8.f);
	road2Quad->SetAlpha(0.0f);
	EEFade(road2Quad, 1.0f, 1.0f, 3.0f);
	//mainScene->AddObject(road2Quad);

	//order 7, time 3.5 - +∞
	EETexture TopTex(L"Texture\\主界面\\上框.png");
	EEQuad2D *TopQuad = new EEQuad2D(Rect_Float(0, 0, (float)EEGetWidth(), 25.f), TopTex);
	TopQuad->SetLocalZOrder(7.f);
	TopQuad->SetAlpha(0.0f);
	EEFade(TopQuad, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(TopQuad);

	//order 7, time 3.5 - +∞
	EETexture BottonTex(L"Texture\\主界面\\下框.png");
	EEQuad2D *BottomQuad = new EEQuad2D(Rect_Float(0, (float)EEGetHeight() - 25.f, (float)EEGetWidth(), (float)EEGetHeight()), BottonTex);
	BottomQuad->SetLocalZOrder(7.f);
	BottomQuad->SetAlpha(0.0f);
	EEFade(BottomQuad, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(BottomQuad);

	EETexture particleTex(L"Texture/主界面/随机上升的音符效果（线性减淡）.png");
	EEParticleInfo info;
	info.amount = 70;
	//life
	info.durationTime = 4.f;
	info.deltaDurationTime = 3.f;
	//position
	info.position = FLOAT3(800.f, 400.f, 9.0);
	info.deltaPosition = FLOAT3(50.f, 50.f, 0.0);
	info.width = 50;
	info.height = 50;
	info.direction = FLOAT3(-40.0, -30.0f, 0.0);
	info.deltaDirection = FLOAT3(20.0, 30.0f, 0.0);
	//color
	info.beginColor = EEColor(1.0f, 1.0f, 1.0f, 1.0f);
	info.deltaBeginColor = EEColor(0.2f, 0.2f, 0.0f, 0.0f);
	info.endColor = EEColor(0.8f, 0.8f, 1.0f, 0.0f);
	info.deltaEndColor = EEColor(0.26f, 0.26f, 0.0f, 0.0f);
	//scale
	info.beginScale = 1.0f;
	info.deltaBeginScale = 0.2f;
	info.endScale = 4.0f;
	info.deltaEndScale = 0.0f;
	//texture
	info.texture = particleTex;
	EEParticleEmitter *particle = new EEParticleEmitter(info);
	mainScene->AddObject(particle);

	EERecorder recorder((DWORD_PTR)RecorderInProc);
	recorder.Start();
	std::map<string, EEMusic*> voices;
	while (EERun())
	{
		EEBeginScene(EEColor::WHITE);

		EEProcess(mainScene);
		std::string data;
		int headpos = 0;
		int tailpos = 0;
		if (g_client.Recv(data))
		{
			std::string name;
			if ((headpos = data.find("<name>", headpos)) != -1)
			{
				headpos += 6;
				if ((tailpos = data.find("<name>", headpos)) != -1)
				{
					name.assign(data, headpos, tailpos - headpos);
				}
			}
			std::string wave;
			if ((headpos = data.find("<wave>", headpos)) != -1)
			{
				headpos += 6;
				if ((tailpos = data.find("<wave>", headpos)) != -1)
				{
					wave.assign(data, headpos, tailpos - headpos);
				}
			}

			std::map<string, EEMusic*>::iterator it = voices.find(name);
			if (it == voices.end())
			{
				EEMusic *voice = new EEMusic(recorder.GetFormat());
				voice->Play();
				voices.insert(std::pair<string, EEMusic*>(name, voice));
				char code[960];
				EEWaveCoder::WaveDecode(&wave[0], 60, code, NULL);
				voice->AddBuffer(code, 960);
			}
			else
			{
				char code[960];
				EEWaveCoder::WaveDecode(&wave[0], 60, code, NULL);
				it->second->AddBuffer(code, 960);
			}
		}

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
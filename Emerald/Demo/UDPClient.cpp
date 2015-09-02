//Send recorder by UDP(Client)
#if 0
#include "Emerald.h"

using namespace std;

EEUDPClient g_client("fe80::11ea:39a6:7ae1:187c%38", 23333);
bool flag(false);
string g_name;
void ConfirmName()
{
	flag = true;
}
//EEUDPClient g_client("fe80::f43b:e20:d94e:a87c%10", 23333);
//EEUDPClient g_client("2001:da8:215:c5c4:f43b:e20:d94e:a87c", 23333);


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

	EETexture bgTex(L"Texture\\主界面\\主界面背景.jpg");
	EELineEditer *lineEditer = new EELineEditer(Rect_Float(200.f, 300.f, 300.f, 330.f), bgTex, EEColor::BLACK);
	EETexture buttonTex(L"Texture/主界面/模式标签/生涯模式.png");
	EEButton *button1 = new EEButton(EE_BUTTON_SCALE, Rect_Float(40.f, 380.f, 100.f, 440.f), 1.3f, 0.2f, 0.2f, buttonTex, (DWORD_PTR)ConfirmName);
	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		lineEditer->Process();
		button1->Process();
		if (flag)
		{
			g_name = lineEditer->GetText();
			break;
		}

		EEEndScene();
	}


	EERecorder recorder((DWORD_PTR)RecorderInProc);
	recorder.Start();
	std::map<string, EEMusic*> voices;
	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

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
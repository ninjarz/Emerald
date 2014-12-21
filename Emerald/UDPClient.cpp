//Send recorder by UDP(Client)
#if 0
#include "Emerald.h"

EEUDPClient g_client("192.168.229.1", 23333);

void CALLBACK RecorderInProc(HWAVEIN _waveIn, UINT _msg, DWORD_PTR _instance, DWORD_PTR _param1, DWORD_PTR _param2)
{
	LPWAVEHDR waveInHdr = (LPWAVEHDR)_param1;
	if (WIM_DATA == _msg)
	{
		LPWAVEHDR waveInHdr = (LPWAVEHDR)_param1;

		char code[60];
		EEWaveCoder::WaveEncode(waveInHdr->lpData, EE_RECORDER_FRAME_SIZE, code, NULL);
		std::string data = "a";
		data.append(code, 60);

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

	EERecorder recorder((DWORD_PTR)RecorderInProc);
	recorder.Start();

	std::map<char, EEMusic*> voices;

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		std::string recv;
		if (g_client.Recv(recv))
		{
			std::map<char, EEMusic*>::iterator it = voices.find(recv[0]);
			if (it == voices.end())
			{
				EEMusic *voice = new EEMusic(recorder.GetFormat());
				voice->Play();
				voices.insert(std::pair<char, EEMusic*>(recv[0], voice));
				char code[960];
				EEWaveCoder::WaveDecode(&recv[1], 60, code, NULL);
				voice->AddBuffer(code, 960);
			}
			else
			{
				char code[960];
				EEWaveCoder::WaveDecode(&recv[1], 60, code, NULL);
				it->second->AddBuffer(code, 960);
			}
		}

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif
//AnimationEditer Demo
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

	EEAnimationEmitter *emitter = new EEAnimationEmitter();
	std::vector<EEAnimationFrame*> target = emitter->GetAnimation()->GetFrames();

	EETexture button1Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/自由模式.png");
	EEButton addItem(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.03f, (float)EEGetHeight() * 0.8489f, (float)EEGetWidth() * 0.1075f, (float)EEGetHeight() * 0.9856f), 1.3f, 0.2f, 0.2f, button1Tex);
	EEComboBox *combo = new EEComboBox(Rect_Float(100.f, 300.f, 200.f, 320.f), EEColor::WHITE, EEColor::BLACK);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		addItem.Process();
		combo->Process();
		if (EEIsMouseInput())
		{
			EEGetMouse();
			emitter->Emit(FLOAT3(EEGetMousePosition(), 0.0f));
		}
		emitter->Process();

		EEEndScene();
	}
	SAFE_DELETE(emitter);

	EEShutdown();
	return 0;
}

#endif
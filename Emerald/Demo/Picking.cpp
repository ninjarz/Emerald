//Picking Demo
#if 0
#include "../Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEAxisAlignedBox qwe;

	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = true;					//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	EETexture tex(L"Texture\\etc\\xmyrz.jpg");
	EETexture tex0(L"Texture/主界面/主界面背景.jpg");
	EETexture tex1(L"Texture/主界面/模式标签/生涯模式.png");
	EEBox box(FLOAT3(50.0f, 20.0f, 10.0f), 10.0f, 10.f, 10.f, tex);
	EESphere sphere(20.f, 100, 100, tex1);
	sphere.SetPosition(FLOAT3(-50.0f, 20.0f, 10.0f));
	EEGrid grid(200.f, 200.f, 100, 100);
	grid.SetColor(EEColor::WHITE);
	grid.SetPosition(FLOAT3(0.f, -40.0f, 10.0f));
	EECylinder cylinder(10.f, 20.f, 20.f, 50, 50, tex0);
	cylinder.SetPosition(FLOAT3(0.0f, -20.0f, 10.0f));
	EETexture tex2(L"Mesh/model.jpg");
	EEPolyhedron polyhedron(L"Mesh/model.txt", tex2);
	polyhedron.SetPosition(FLOAT3(0.0f, 0.0f, 10.0f));

	FLOAT3 position = EEGetCameraPosition();
	FLOAT3 look = EEGetCameraLook();

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		//EEShowFPSInTitle(L"- -");

		EECameraProcess(EE_CAMERA_FIRST);
		box.Process();
		sphere.Process();
		grid.Process();
		cylinder.Process();
		polyhedron.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;

	/*EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = true;					//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	EETexture tex(L"Texture/etc/xmyrz.jpg");
	EETexture cat(L"Texture/etc/cat.jpg");
	EETexture tex0(L"Texture/主界面/主界面背景.jpg");
	EETexture tex1(L"Texture/主界面/模式标签/生涯模式.png");

	EEGrid grid(200.f, 200.f, 100, 100, tex);
	grid.SetPosition(FLOAT3(0.f, -40.0f, 0.0f));
	EEBox back(FLOAT3(0.f, 60.0f, 50.f), 200.0f, 200.f, 2.f, cat);
	EEBox box(FLOAT3(-25.0f, -30.0f, -25.0f), 3.0f, 20.f, 3.f);
	box.SetColor(EEColor::WHITE);
	EECylinder cylinder(5.f, 10.f, 10.f, 50, 50);
	cylinder.SetColor(EEColor::WHITE);
	cylinder.SetPosition(FLOAT3(-25.0f, -20.0f, -25.0f));
	EESphere sphere(5.f, 100, 100);
	sphere.SetColor(EEColor::WHITE);
	sphere.SetPosition(FLOAT3(50.0f, -35.0f, -10.0f));

	FLOAT3 position = EEGetCameraPosition();
	FLOAT3 look = EEGetCameraLook();

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		//EEShowFPSInTitle(L"- -");

		EECameraProcess(EE_CAMERA_FIRST);
		back.Process();
		box.Process();
		sphere.Process();
		grid.Process();
		cylinder.Process();

		EEEndScene();
	}*/
}

#endif
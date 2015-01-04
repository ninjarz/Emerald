#pragma once
#ifndef _EE_CORE_H_
#define _EE_CORE_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "EEHelper.h"
#include "EEMath.h"
#include "EECamera.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	class EEThreadSystem;
	class EESystem;
	class EEInput;
	class EETimer;
	class EED3D;
	class EEShaderState;

	//EEDesc
	//----------------------------------------------------------------------------------------------------
	struct EEDesc
	{
		LPCWSTR     applicationName;  //窗口名称
		bool        isFullScreen;     //是否全屏
		int         width;            //窗口宽度
		int         height;           //窗口高度
		bool        isSSAA;           //是开启抗锯齿
		bool        isVsync;          //是否垂直同步
	};

	//EECore
	//----------------------------------------------------------------------------------------------------
	class EECore
	{
		//EECore
	public:
		static EECore *s_EECore;

	public:
		inline EECore()
		{
		}

		inline EECore(const EECore&)
		{
		}

		inline ~EECore()
		{
		}

		bool Initialize_All(const EEDesc& _EEDesc);
		void Shutdown();
		int Run();

	private:
		MSG m_msg;

		//EECore_ThreadSystem
	public:
		EEThreadSystem* GetEEThreadSystem() const;

		bool AddThread(boost::thread* _thread);
		bool RemoveThread(boost::thread* _thread);
		void JoinAll();

	private:
		EEThreadSystem* m_EEThreadSystem;

		//EECore_System
	public:
		EESystem* GetEESystem() const;

		bool SetHInstance(HINSTANCE _para);
		bool SetHWnd(HWND _para);
		bool SetApplicationName(LPCWSTR _para);
		bool SetIsFullScreen(bool _para);
		bool SetWidth(int _para);
		bool SetHeight(int _para);
		bool SetRect(const RECT& _para);
		bool SetInsideCentre(const INT2& _para);
		bool SetOutsideCentre(const INT2& _para);

		HINSTANCE GetHInstance() const;
		HWND GetHWnd() const;
		LPCWSTR GetApplicationName() const;
		bool GetIsFullScreen() const;
		int GetWidth() const;
		int GetHeight() const;
		RECT GetRect() const;
		INT2 GetInsideCentre() const;
		INT2 GetOutsideCentre() const;

	private:
		EESystem *m_EESystem;

		//EECore_Input
	public:
		EEInput* GetEEInput();
		LRESULT (EEInput::*GetInputPro())(HWND, UINT, WPARAM, LPARAM);

		bool IsKeyDown(UINT);
		UINT GetKey();
		bool IsKeyInput();
		Point GetMousePosition() const;
		int GetMouseDeltaX() const;
		int GetMouseDeltaY() const;
		int GetMouseDeltaM() const;
		void ClearMouseDeltaX();
		void ClearMouseDeltaY();
		void ClearMouseDeltaXY();
		void ClearMouseDeltaM();

	private:
		EEInput *m_EEInput;
		LRESULT (EEInput::*m_inputPro)(HWND, UINT, WPARAM, LPARAM);

		//EECore_Timer
	public:
		EETimer* GetEETimer();

		void Start();
		void Stop();
		void Reset();
		void Tick();
		double GetTotalTime() const;
		double GetDeltaTime() const;
		void ShowFPSInTitle();
		void ShowFPSInTitle(LPCWSTR _title);

	private:
		EETimer *m_EETimer;

		//EECore_D3D
	public:
		EED3D* GetEED3D();

		bool SetDevice(ID3D11Device* _para);
		bool SetDeviceeContext(ID3D11DeviceContext* _para);
		bool SetScreenNear(float _para);
		bool SetScreenDeep(float _para);
		bool SetIsSSAA(bool _para);
		bool SetIsVsync(bool _para);

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		float GetScreenNear() const;
		float GetScreenDeep() const;
		bool GetIsSSAA() const;
		bool GetIsVsync() const;

	private:
		EED3D *m_EED3D;

		//EECore_CameraSystem
	public:
		EECameraSystem* GetEECameraSystem();

		bool MapCameraBuffer();
		EEHCamera CreateCamera(const EECameraDesc& _desc);
		bool DeleteCamera(EEHCamera _camera);
		void ClearCamera();
		bool SetCamera(EEHCamera _camera);
		EEHCamera GetCamera();
		const MATRIX& GetViewMatrix();
		const MATRIX& GetProjectionMatrix();
		const MATRIX& GetOrthoLHMatrix();

	private:
		EECameraSystem *m_EECameraSystem;

		//EECore_ShaderState
	public:
		EEShaderState* GetEEShaderState();

	private:
		EEShaderState *m_EEShaderState;

		//EECore_GUI
	public:
		boost::shared_mutex* GetControlMutex();

	private:
		boost::shared_mutex	*m_control_mutex;
	};

	//EECore_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEInitialize(const EEDesc& _EEDesc = { L"Emerald", false, 800, 450, true, false });
	void EEShutdown();
	int EERun();
}

#endif
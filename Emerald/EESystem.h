#pragma once
#ifndef _EE_SYSTEM_H_
#define _EE_SYSTEM_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESystem
	//----------------------------------------------------------------------------------------------------
	class EESystem
	{
	public:
		inline EESystem()
		{
		}

		inline EESystem(const EESystem&)
		{
		}

		inline ~EESystem()
		{
		}

		bool Initialize(LPCWSTR _applicationName, bool _isFullScreen, int _width, int _height);
		void Shutdown();

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
		void InitWindows();
		void ShutdownWindows();

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	private:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		LPCWSTR m_applicationName;
		bool m_isFullScreen;
		int m_width;
		int m_height;
		RECT m_rect;
		INT2 m_insideCentre;
		INT2 m_outsideCentre;
	};

	//EESystem_APIs
	//----------------------------------------------------------------------------------------------------
	HINSTANCE EEGetHInstance();
	HWND EEGetHWnd();
	LPCWSTR EEGetApplicationName();
	bool EEGetIsFullScreen();
	int EEGetWidth();
	int EEGetHeight();
	RECT EEGetRect();
	INT2 EEGetInsideCentre();
	INT2 EEGetOutsideCentre();
}

#endif
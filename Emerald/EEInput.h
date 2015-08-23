#pragma once
#ifndef _EE_INPUT_H_
#define _EE_INPUT_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WindowsX.h>
#include <queue>
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEInput
	//----------------------------------------------------------------------------------------------------
	class EEInput
	{
		//EEInput
	public:
		inline EEInput()
		{
		}

		inline EEInput(const EEInput&)
		{
		}

		inline virtual ~EEInput()
		{
			Shutdown();
		}

		bool Initialize();
		void Shutdown();
		LRESULT MessageHandler(HWND, UINT, WPARAM, LPARAM);

		//EEInput_Keyboard
	public:
		bool IsKeyDown(UINT);
		UINT PeekKey();
		UINT GetKey();
		bool IsKeyInput();
		void ClearKey();

	private:
		void KeyDown(UINT);
		void KeyUp(UINT);

	private:
		bool m_keys[256];
		std::queue<UINT> m_keyInput;

		//EEInput_Mouse
	public:
		inline Point GetMousePosition() const	{ return m_mousePos; }
		inline int GetMouseDeltaX() const		{ return m_mouseDeltaX; }
		inline int GetMouseDeltaY() const		{ return m_mouseDeltaY; }
		inline int GetMouseDeltaM() const		{ return m_mouseDeltaM; }
		inline void ClearMouseDeltaX()			{ m_mouseDeltaX = 0; }
		inline void ClearMouseDeltaY()			{ m_mouseDeltaY = 0; }
		inline void ClearMouseDeltaXY()			{ m_mouseDeltaX = 0; m_mouseDeltaY = 0; }
		inline void ClearMouseDeltaM()			{ m_mouseDeltaM = 0; }
		UINT GetMouse();
		bool IsMouseInput();

	private:
		void MouseDown(WPARAM, int, int);
		void MouseUp(WPARAM, int, int);
		void MouseWheel(WPARAM, int, int);
		void OnMouseMove(WPARAM, int, int);

	private:
		//normal
		Point m_mousePos;
		int m_mouseDeltaX;
		int m_mouseDeltaY;
		int m_mouseDeltaM;
		std::queue<UINT> m_mouseInput;
		/*
		//with the left button held
		Point m_mouseLeftPos;
		int m_mouseLeftDeltaX;
		int m_mouseLeftDeltaY;
		int m_mouseLeftDeltaM;
		//with the right button held
		Point m_mouseRightPos;
		int m_mouseRightDeltaX;
		int m_mouseRightDeltaY;
		int m_mouseRightDeltaM;
		*/
		int m_mouseDeltaPerLine; //‘› ±√ª”√
	};

	//EEInput_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyDown(UINT _para);
	UINT EEPeekKey();
	UINT EEGetKey();
	bool EEIsKeyInput();
	void EEClearKey();

	Point EEGetMousePosition();
	int EEGetMouseDeltaX();
	int EEGetMouseDeltaY();
	int EEGetMouseDeltaM();
	void EEClearMouseDeltaX();
	void EEClearMouseDeltaY();
	void EEClearMouseDeltaXY();
	void EEClearMouseDeltaM();
	UINT EEGetMouse();
	bool EEIsMouseInput();
}

#endif
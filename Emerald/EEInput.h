#pragma once
#ifndef _EE_INPUT_H_
#define _EE_INPUT_H_

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

		inline ~EEInput()
		{
		}

		bool Initialize();
		void Shutdown();
		LRESULT MessageHandler(HWND, UINT, WPARAM, LPARAM);

		//EEInput_Keyboard
	public:
		bool IsKeyDown(UINT);
		UINT GetKey();
		bool IsKeyInput();

	private:
		void KeyDown(UINT);
		void KeyUp(UINT);

	private:
		bool m_keys[256];
		std::queue<UINT> m_keyInput;

		//EEInput_Mouse
	public:
		inline Point GetMousePosition() const	{ return m_pos; }
		inline int GetOnDeltaX() const			{ return m_onDeltaX; }
		inline int GetOnDeltaY() const			{ return m_onDeltaY; }
		inline void ClearOnDeltaX()				{ m_onDeltaX = 0; }
		inline void ClearOnDeltaY()				{ m_onDeltaY = 0; }
		inline void ClearOnDeltaXY()			{ m_onDeltaX = 0; m_onDeltaY = 0; }

	private:
		void MouseDown(WPARAM, int, int);
		void MouseUp(WPARAM, int, int);
		void MouseWheel(WPARAM, int, int);
		void OnMouseMove(WPARAM, int, int);

	private:
		Point m_pos;
		int m_onDeltaX;
		int m_onDeltaY;
		int m_deltaM;
		int m_deltaPerLine;//‘› ±√ª”√
	};

	//EEInput_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyDown(UINT _para);
	UINT EEGetKey();
	bool EEIsKeyInput();
	Point EEGetMousePosition();
	int EEGetOnDeltaX();
	int EEGetOnDeltaY();
	void EEClearOnDeltaX();
	void EEClearOnDeltaY();
	void EEClearOnDeltaXY();
}

#endif
#pragma once
#ifndef _EE_INPUT_H_
#define _EE_INPUT_H_

#include <windows.h>
#include <WindowsX.h>
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
	private:
		void KeyDown(unsigned int);
		void KeyUp(unsigned int);

	public:
		bool IsKeyDown(UINT);

	private:
		bool m_keys[256];

		//EEInput_Mouse
	private:
		void MouseDown(WPARAM, int, int);
		void MouseUp(WPARAM, int, int);
		void MouseWheel(WPARAM, int, int);
		void OnMouseMove(WPARAM, int, int);

	public:
		inline Point GetMousePosition() const	{ return m_pos; }
		inline int GetOnDeltaX() const			{ return m_onDeltaX; }
		inline int GetOnDeltaY() const			{ return m_onDeltaY; }
		inline void ClearOnDeltaX()				{ m_onDeltaX = 0; }
		inline void ClearOnDeltaY()				{ m_onDeltaY = 0; }
		inline void ClearOnDeltaXY()			{ m_onDeltaX = 0; m_onDeltaY = 0; }

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
	Point EEGetMousePosition();
	int EEGetOnDeltaX();
	int EEGetOnDeltaY();
	void EEClearOnDeltaX();
	void EEClearOnDeltaY();
	void EEClearOnDeltaXY();
}

#endif
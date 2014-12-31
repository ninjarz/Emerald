#include "EEInput.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEInput
	//----------------------------------------------------------------------------------------------------
	bool EEInput::Initialize()
	{
		//initialize keyboard
		ZeroMemory(m_keys, sizeof(m_keys));

		//initialize mouse
		m_mousePos = Point(0, 0);
		m_mouseDeltaX = 0;
		m_mouseDeltaY = 0;
		m_mouseDeltaM = 0;
		ULONG scrollLines;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &scrollLines, 0);
		if (scrollLines)
			m_mouseDeltaPerLine = WHEEL_DELTA / scrollLines;
		else
			m_mouseDeltaPerLine = 0;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::Shutdown()
	{
	}

	//----------------------------------------------------------------------------------------------------
	LRESULT EEInput::MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
	{
		switch (_umsg)
		{
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			MouseDown(_wparam, GET_X_LPARAM(_lparam), GET_Y_LPARAM(_lparam));
			return 0;

		case WM_LBUTTONUP:
			MouseUp(VK_LBUTTON, GET_X_LPARAM(_lparam), GET_Y_LPARAM(_lparam));
			return 0;
		case WM_MBUTTONUP:
			MouseUp(VK_MBUTTON, GET_X_LPARAM(_lparam), GET_Y_LPARAM(_lparam));
			return 0;
		case WM_RBUTTONUP:
			MouseUp(VK_RBUTTON, GET_X_LPARAM(_lparam), GET_Y_LPARAM(_lparam));
			return 0;

			//case WM_SETTINGCHANGE:

		case WM_MOUSEWHEEL:
			MouseWheel(_wparam, GET_X_LPARAM(_lparam), GET_Y_LPARAM(_lparam));
			return 0;
		case WM_MOUSEMOVE:
			OnMouseMove(_wparam, GET_X_LPARAM(_lparam), GET_Y_LPARAM(_lparam));
			return 0;

		case WM_KEYDOWN:
			KeyDown((unsigned int)_wparam);
			return 0;

		case WM_KEYUP:
			KeyUp((unsigned int)_wparam);
			return 0;

		case WM_CHAR:
			m_keyInput.push((unsigned int)_wparam);
			return 0;

		default:
			return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
		}
		//return 0;
	}

	//EEInput_Keyboard
	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsKeyDown(UINT _key)
	{
		return m_keys[_key];
	}

	//----------------------------------------------------------------------------------------------------
	UINT EEInput::GetKey()
	{
		while (m_keyInput.empty())
		{
			//may be I should just handle the message, but the timer too
			EECore::s_EECore->Run();
		}

		UINT key = m_keyInput.front();
		m_keyInput.pop();
		return key;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsKeyInput()
	{
		return !m_keyInput.empty();
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::KeyDown(UINT input)
	{
		m_keys[input] = true;

		return;
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::KeyUp(UINT _input)
	{
		m_keys[_input] = false;

		return;
	}

	//EEInput_Mouse
	//----------------------------------------------------------------------------------------------------
	void EEInput::MouseDown(WPARAM _btnState, int _x, int _y)
	{
		SetCapture(EECore::s_EECore->GetHWnd());
		KeyDown((unsigned int)_btnState);
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::MouseUp(WPARAM _btnState, int _x, int _y)
	{
		ReleaseCapture();
		KeyUp((unsigned int)_btnState);
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::MouseWheel(WPARAM _btnState, int _x, int _y)
	{
		m_mouseDeltaM += (short)HIWORD(_btnState);
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::OnMouseMove(WPARAM _btnState, int _x, int _y)
	{
		m_mouseDeltaX += _x - m_mousePos.x;
		m_mouseDeltaY += _y - m_mousePos.y;
		m_mousePos.x = _x;
		m_mousePos.y = _y;
	}

	//EEInput_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyDown(UINT para) { return EECore::s_EECore->GetEEInput()->IsKeyDown(para); }

	//----------------------------------------------------------------------------------------------------
	UINT EEGetKey() { return EECore::s_EECore->GetEEInput()->GetKey(); }

	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyInput() { return EECore::s_EECore->GetEEInput()->IsKeyInput(); }

	//----------------------------------------------------------------------------------------------------
	Point EEGetMousePosition() { return EECore::s_EECore->GetEEInput()->GetMousePosition(); }

	//----------------------------------------------------------------------------------------------------
	int EEGetMouseDeltaX() { return EECore::s_EECore->GetEEInput()->GetMouseDeltaX(); }

	//----------------------------------------------------------------------------------------------------
	int EEGetMouseDeltaY() { return EECore::s_EECore->GetEEInput()->GetMouseDeltaY(); }

	//----------------------------------------------------------------------------------------------------
	int EEGetMouseDeltaM() { return EECore::s_EECore->GetEEInput()->GetMouseDeltaM(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearMouseDeltaX() { return EECore::s_EECore->GetEEInput()->ClearMouseDeltaX(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearMouseDeltaY() { return EECore::s_EECore->GetEEInput()->ClearMouseDeltaY(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearMouseDeltaXY() { return EECore::s_EECore->GetEEInput()->ClearMouseDeltaXY(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearMouseDeltaM() { return EECore::s_EECore->GetEEInput()->ClearMouseDeltaM(); }
}
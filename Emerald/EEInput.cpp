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
			m_keyInput.push_back((unsigned int)_wparam);
			//printf("%c %d\n", _wparam, m_keyInput.size());
			return 0;
		case WM_IME_CHAR:
			m_keyInput.push_back((unsigned int)_wparam);
			return 0;

		default:
			return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
		}
		//return 0;
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::Clear()
	{
		ClearKey();
		ClearKeyHit();
		ClearMouse();
		ClearMouseDeltaXY();
		ClearMouseDeltaM();
	}

	//EEInput_Keyboard
	//----------------------------------------------------------------------------------------------------
	int EEInput::PeekKey(UINT _key)
	{
		int i = 0;
		for (auto it = m_keyInput.begin(); it != m_keyInput.end(); ++it, ++i)
		{
			if (*it == _key)
				return i;
		}

		return  -1;
	}

	//----------------------------------------------------------------------------------------------------
	UINT EEInput::GetKey()
	{
		while (m_keyInput.empty())
		{
			// todo
			EECore::s_EECore->Run();
		}

		UINT key = m_keyInput.front();
		m_keyInput.pop_front();
		return key;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::GetKey(UINT _key)
	{
		for (auto it = m_keyInput.begin(); it != m_keyInput.end(); ++it)
		{
			if (*it == _key)
			{
				m_keyInput.erase(it);
				return true;
			}
		}

		return  false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsKeyInput()
	{
		return !m_keyInput.empty();
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::ClearKey()
	{
		m_keyInput.clear();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsKeyDown(UINT _key)
	{
		return m_keys[_key];
	}

	//----------------------------------------------------------------------------------------------------
	UINT EEInput::GetKeyHit()
	{
		while (m_keyHitInput.empty())
		{
			//  May be I should just handle the message, but running the timer
			EECore::s_EECore->Run();
		}

		UINT key = m_keyHitInput.front();
		m_keyHitInput.pop_front();
		return key;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::GetKeyHit(UINT _key)
	{
		for (auto it = m_keyHitInput.begin(); it != m_keyHitInput.end(); ++it)
		{
			if (*it == _key)
			{
				m_keyHitInput.erase(it);
				return true;
			}
		}

		return  false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsKeyHitInput()
	{
		return !m_keyHitInput.empty();
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::ClearKeyHit()
	{
		m_keyHitInput.clear();
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::KeyDown(UINT input)
	{
		if (!m_keys[input])
			m_keyHitInput.push_back(input);
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
	UINT EEInput::GetMouse()
	{
		while (m_mouseInput.empty())
		{
			EECore::s_EECore->Run();
		}

		UINT mouse = m_mouseInput.front();
		m_mouseInput.pop_front();
		return mouse;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsMouseInput()
	{
		return !m_mouseInput.empty();
	}

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
		m_mouseInput.push_back((unsigned int)_btnState);
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

	//----------------------------------------------------------------------------------------------------
	void EEInput::ClearMouse()
	{
		m_mouseInput.clear();
	}

	//EEInput_APIs
	//----------------------------------------------------------------------------------------------------
	void EEClearInput() { return EECore::s_EECore->GetEEInput()->Clear(); }

	//----------------------------------------------------------------------------------------------------
	int EEPeekKey(UINT _key) { return EECore::s_EECore->GetEEInput()->PeekKey(_key); }

	//----------------------------------------------------------------------------------------------------
	UINT EEGetKey() { return EECore::s_EECore->GetEEInput()->GetKey(); }

	//----------------------------------------------------------------------------------------------------
	bool EEGetKey(UINT _key) { return EECore::s_EECore->GetEEInput()->GetKey(_key); }

	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyInput() { return EECore::s_EECore->GetEEInput()->IsKeyInput(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearKey() { return EECore::s_EECore->GetEEInput()->ClearKey(); }

	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyDown(UINT para) { return EECore::s_EECore->GetEEInput()->IsKeyDown(para); }

	//----------------------------------------------------------------------------------------------------
	UINT EEGetKeyHit() { return EECore::s_EECore->GetEEInput()->GetKeyHit(); }

	//----------------------------------------------------------------------------------------------------
	bool EEGetKeyHit(UINT _key) { return EECore::s_EECore->GetEEInput()->GetKeyHit(_key); }

	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyHitInput() { return EECore::s_EECore->GetEEInput()->IsKeyHitInput(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearKeyHit() { return EECore::s_EECore->GetEEInput()->ClearKeyHit(); }

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

	//----------------------------------------------------------------------------------------------------
	UINT EEGetMouse() { return EECore::s_EECore->GetEEInput()->GetMouse(); }

	//----------------------------------------------------------------------------------------------------
	bool EEIsMouseInput() { return EECore::s_EECore->GetEEInput()->IsMouseInput(); }
}
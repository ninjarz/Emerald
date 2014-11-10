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
		m_pos = Point(0, 0);
		m_onDeltaX = 0;
		m_onDeltaY = 0;
		m_deltaM = 0;
		ULONG scrollLines;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &scrollLines, 0);
		if (scrollLines)
			m_deltaPerLine = WHEEL_DELTA / scrollLines;
		else
			m_deltaPerLine = 0;

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

		default:
			return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
		}
		//return 0;
	}

	//EEInput_Keyboard
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

	//----------------------------------------------------------------------------------------------------
	bool EEInput::IsKeyDown(UINT _key)
	{
		return m_keys[_key];
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
		m_deltaM += (short)HIWORD(_btnState);
	}

	//----------------------------------------------------------------------------------------------------
	void EEInput::OnMouseMove(WPARAM _btnState, int _x, int _y)
	{
		m_onDeltaX += _x - m_pos.x;
		m_onDeltaY += _y - m_pos.y;
		m_pos.x = _x;
		m_pos.y = _y;
	}

	//EEInput_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEIsKeyDown(UINT para) { return EECore::s_EECore->GetEEInput()->IsKeyDown(para); }

	//----------------------------------------------------------------------------------------------------
	Point EEGetMousePosition() { return EECore::s_EECore->GetEEInput()->GetMousePosition(); }

	//----------------------------------------------------------------------------------------------------
	int EEGetOnDeltaX() { return EECore::s_EECore->GetEEInput()->GetOnDeltaX(); }

	//----------------------------------------------------------------------------------------------------
	int EEGetOnDeltaY() { return EECore::s_EECore->GetEEInput()->GetOnDeltaY(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearOnDeltaX() { return EECore::s_EECore->GetEEInput()->ClearOnDeltaX(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearOnDeltaY() { return EECore::s_EECore->GetEEInput()->ClearOnDeltaY(); }

	//----------------------------------------------------------------------------------------------------
	void EEClearOnDeltaXY() { return EECore::s_EECore->GetEEInput()->ClearOnDeltaXY(); }
}
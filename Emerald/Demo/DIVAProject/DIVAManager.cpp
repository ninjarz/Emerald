#include "DIVAManager.h"
#include "DIVAMenu.h"
#include "DIVAFreeMode.h"


//----------------------------------------------------------------------------------------------------
DIVAManager::DIVAManager()
	:
	m_state(DIVA_FREE_MODE)
{
	Initialize();
}

//----------------------------------------------------------------------------------------------------
DIVAManager::~DIVAManager()
{

}

//----------------------------------------------------------------------------------------------------
bool DIVAManager::Process()
{
	EEDesc desc;
	desc.applicationName = L"Emerald";
	desc.isFullScreen = false;
	desc.width = 800;
	desc.height = 450;
	desc.isSSAA = false;
	desc.isVsync = true;
	EEInitialize(desc);

	while (m_state != DIVA_END)
	{
		switch (m_state)
		{
		case DIVA_MENU:
		{
			DIVAMenu menu;
			m_state = menu.Process();
			break;
		}
		case DIVA_FREE_MODE:
		{
			DIVAFreeMode freeMode;
			m_state = freeMode.Process();
			break;
		}
		default:
			m_state = DIVA_END;
			break;
		}
	}

	EEShutdown();
	return 0;
}

//----------------------------------------------------------------------------------------------------
bool DIVAManager::Initialize()
{

	return true;
}
#include "DIVAFreeMode.h"


DIVAFreeMode::DIVAFreeMode()
	:
	m_scene()
{
	Initialize();
}

DIVAFreeMode::~DIVAFreeMode()
{

}

DIVAState DIVAFreeMode::Process()
{
	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		EEProcess(&m_scene);

		EEEndScene();
	}

	return DIVA_END;
}

bool DIVAFreeMode::Initialize()
{
	

	return true;
}
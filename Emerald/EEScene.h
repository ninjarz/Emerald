#pragma once
#ifndef _EE_SCENE_H_
#define _EE_SCENE_H_

#include <vector>
#include <list>
#include "EEGeometry.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEScne
	//----------------------------------------------------------------------------------------------------
	class EEScene : public EEObject
	{
	public:
		EEScene();
		EEScene(const EEScene &_scene);
		virtual ~EEScene();

		virtual bool Update();
		virtual bool Render();
		void AddObject(EEObject* _object);
		void RemoveObject(EEObject* _object);
		void SubObject();

	protected:
		//controls
		std::vector<EEObject*> m_objects;
	};
}


#endif
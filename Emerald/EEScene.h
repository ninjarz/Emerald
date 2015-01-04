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
	class EEScene : public EEQuad2D
	{
	public:
		EEScene(const Rect_Float &_rect, const EETexture& _tex);
		EEScene(const Rect_Float &_rect, ID3D11ShaderResourceView *_tex);
		EEScene(const EEScene &_scene);
		~EEScene();

		bool Update();
		bool Render();
		void AddObject(EEObject* _object);
		void RemoveObject(EEObject* _object);

	protected:
		//controls
		std::vector<EEObject*> m_objects;
	};
}


#endif
#pragma once
#ifndef _EE_CYLINDER_H_
#define _EE_CYLINDER_H_

#include "EEObject3D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EECylinderVertex
	//----------------------------------------------------------------------------------------------------
	struct EECylinderVertex
	{
		FLOAT3 pos;
		FLOAT3 normal;
		FLOAT2 tex;
	};

	// EECylinder
	//----------------------------------------------------------------------------------------------------
	class EECylinder : public EEObject3D
	{
	public:
		EECylinder(float _topRadius, float _bottomRadius, float _height, int _slice, int _stack);
		EECylinder(float _topRadius, float _bottomRadius, float _height, int _slice, int _stack, const EETexture& _tex);
		EECylinder(const EECylinder& _cylinder);
		virtual ~EECylinder();
		virtual inline EEObject* Clone() { return new EECylinder(*this); }

		virtual bool Update();
		virtual bool Render();

	protected:
		// vbuffer and ibuffer
		bool CreateCylinderBuffer();

	protected:
		// the size of the cylinder
		float m_topRadius;
		float m_bottomRadius;
		float m_height;
		int m_slice;
		int m_stack;
	};
}


#endif
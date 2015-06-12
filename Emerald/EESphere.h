#pragma once
#ifndef _EE_SPHERE_H_
#define _EE_SPHERE_H_

#include "EEObject3D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EESphereVertex
	//----------------------------------------------------------------------------------------------------
	struct EESphereVertex
	{
		FLOAT3 pos;
		FLOAT3 normal;
		FLOAT2 tex;
	};

	// EESphere
	//----------------------------------------------------------------------------------------------------
	class EESphere : public EEObject3D
	{
	public:
		EESphere(float _radius, int _slice, int _stack);
		EESphere(float _radius, int _slice, int _stack, const EETexture& _tex);
		EESphere(const EESphere& _sphere);
		virtual ~EESphere();
		virtual inline EEObject* Clone() { return new EESphere(*this); }

		virtual bool Update();
		virtual bool Render();

	protected:
		//vbuffer and ibuffer
		bool CreateSphereBuffer();

	protected:
		//the size of the sphere
		float m_radius;
		int m_slice;
		int m_stack;
	};
}


#endif
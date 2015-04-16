#pragma once
#ifndef _EE_OBJECT3D_H_
#define _EE_OBJECT3D_H_

#include "EEObject.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	class EEObject3D : public EEObject
	{
	public:
		EEObject3D();
		EEObject3D(const FLOAT3& _position);

	public:
		virtual MATRIX GetViewMatrix();
		virtual MATRIX GetProjectionMatrix();
	};

}

#endif
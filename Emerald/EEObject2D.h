#pragma once
#ifndef _EE_OBJECT2D_H_
#define _EE_OBJECT2D_H_

#include "EEObject.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	class EEObject2D : public EEObject
	{
	public:
		EEObject2D();
		EEObject2D(const FLOAT3& _position);

	public:
		virtual MATRIX GetViewMatrix();
		virtual MATRIX GetProjectionMatrix();

		virtual MATRIX GetFinalWorldMatrix() const;
	};

}

#endif
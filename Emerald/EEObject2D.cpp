#include "EEObject2D.h"
#include"EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	EEObject2D::EEObject2D()
		:
		EEObject()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEObject2D::EEObject2D(const FLOAT3& _position)
		:
		EEObject(_position)
	{

	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject2D::GetViewMatrix()
	{
		return MATRIX::IDENTITY;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject2D::GetProjectionMatrix()
	{
		return EECore::s_EECore->GetOrthoLHMatrix();
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject2D::GetFinalWorldMatrix() const
	{
		return MatrixTranslation(GetFinalPosition());
	}
}
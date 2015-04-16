#include "EEObject3D.h"
#include"EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	EEObject3D::EEObject3D()
		:
		EEObject()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEObject3D::EEObject3D(const FLOAT3& _position)
		:
		EEObject(_position)
	{

	}
	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject3D::GetViewMatrix()
	{
		return EECore::s_EECore->GetViewMatrix();
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject3D::GetProjectionMatrix()
	{
		return EECore::s_EECore->GetProjectionMatrix();
	}

}
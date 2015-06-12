#pragma once
#ifndef _EE_OBJECT3D_H_
#define _EE_OBJECT3D_H_

#include "EEObject.h"
#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	class EEObject3D : public EEObject
	{
	protected:
		static bool Initialize();

	protected:
		static bool s_isInitialized;
		static ID3D11InputLayout *s_objectIL;
		static ID3D11VertexShader *s_objectVS;
		static ID3D11PixelShader  *s_objectPS;

	public:
		EEObject3D();
		EEObject3D(const FLOAT3& _position);

	public:
		virtual MATRIX GetViewMatrix();
		virtual MATRIX GetProjectionMatrix();

	protected:
		// the vertex and index buffer of the box
		ID3D11Buffer *m_objectVB;
		int m_vertexCount;
		ID3D11Buffer *m_objectIB;
		int m_indexCount;
		// collision
		EEAxisAlignedBox m_axisAlignedBox;
	};

}

#endif
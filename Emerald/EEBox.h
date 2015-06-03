#pragma once
#ifndef _EE_BOX_H_
#define _EE_BOX_H_

#include "EEObject3D.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEBoxVertex
	//----------------------------------------------------------------------------------------------------
	struct EEBoxVertex
	{
		FLOAT3 pos;
		FLOAT3 normal;
		FLOAT2 tex;
	};

	// EEBox
	//----------------------------------------------------------------------------------------------------
	class EEBox : public EEObject3D
	{
	protected:
		static bool InitializeBox();

	protected:
		static bool s_isBoxInitialized;
		static ID3D11InputLayout *s_boxIL;
		static ID3D11VertexShader *s_boxVS;
		static ID3D11PixelShader  *s_boxPS;
		static ID3D11Buffer *s_boxBuffer;

	public:
		EEBox(const FLOAT3& _pos, const FLOAT3& _size);
		EEBox(const FLOAT3& _pos, const FLOAT3& _size, const EETexture& _tex);
		EEBox(const FLOAT3& _pos, FLOAT _width, FLOAT _height, FLOAT _depth);
		EEBox(const FLOAT3& _pos, FLOAT _width, FLOAT _height, FLOAT _depth, const EETexture& _tex);
		EEBox(const EEBox& _box);
		virtual ~EEBox();
		virtual inline EEObject* Clone() { return new EEBox(*this); }

		virtual bool Update();
		virtual bool Render();

	protected:
		//vbuffer and ibuffer
		bool CreateBoxVertexBuffer();
		bool CreateBoxIndexBuffer();

	protected:
		//the size of the box
		FLOAT3 m_size;
		//the vertex and index buffer of the box
		ID3D11Buffer *m_boxVB;
		ID3D11Buffer *m_boxIB;
	};
}

#endif
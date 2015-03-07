#pragma once
#ifndef _EE_BOX_H_
#define _EE_BOX_H_

#include "EEObject.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBoxBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEBoxBufferDesc
	{
		int isUseColor;
		int isUseTex;
		float tmp2;
		float tmp3;
	};

	//EEBoxVertex
	//----------------------------------------------------------------------------------------------------
	struct EEBoxVertex
	{
		FLOAT3 pos;
		FLOAT3 normal;
		FLOAT2 tex;
	};

	//EEBox
	//----------------------------------------------------------------------------------------------------
	class EEBox : public EEObject
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

		virtual bool Update();
		virtual bool Render();

	protected:
		//cbuffer
		virtual bool MapBoxBuffer();
		//vbuffer and ibuffer
		bool CreateBoxVertexBuffer();
		bool CreateBoxIndexBuffer();

	protected:
		//the size of the box
		FLOAT3 m_size;
		//the vertex and index buffer of the box
		ID3D11Buffer *m_boxVB;
		ID3D11Buffer *m_boxIB;
		//the texture of the box if need
		EETexture m_boxTex;
		bool m_isUseColor;
		bool m_isUseTex;
	};
}

#endif
#pragma once
#ifndef _EE_LINE2D_H_
#define _EE_LINE2D_H_

#include "EEObject2D.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELine2DVertex
	//----------------------------------------------------------------------------------------------------
	struct EELine2DVertex
	{
		FLOAT3 pos;
		FLOAT2 tex;
	};

	//EELine2D
	//----------------------------------------------------------------------------------------------------
	class EELine2D : public EEObject2D
	{
	protected:
		static bool InitializeLine2D();

	protected:
		static bool s_isLine2DInitialized;
		static ID3D11InputLayout *s_lineIL;
		static ID3D11VertexShader *s_lineVS;
		static ID3D11PixelShader *s_linePS;
		static ID3D11Buffer *s_line2DBuffer;

	public:
		EELine2D(FLOAT2& _start, FLOAT2& _end);
		virtual ~EELine2D();

		virtual bool Update();
		virtual bool Render();

	protected:
		bool CreateLineVertexBuffer();

	protected:
		FLOAT2 m_pos0, m_pos1;
		ID3D11Buffer *m_lineVB;
	};

}


#endif
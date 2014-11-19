#pragma once
#ifndef _EE_GEOMETRY_H_
#define _EE_GEOMETRY_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include "EEObject.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEQuadVertex
	//----------------------------------------------------------------------------------------------------
	struct EEQuadVertex
	{
		FLOAT3 pos;
		FLOAT2 tex;
	};

	//EEQuad
	//----------------------------------------------------------------------------------------------------
	class EEQuad : public EEObject
	{
	public:
		static bool InitializeQuadShader();

	protected:
		//the shader of quad
		static bool s_isQuadInitialized;
		static ID3D11InputLayout *s_quadIL;
		static ID3D11VertexShader *s_quadVS;
		static ID3D11PixelShader  *s_quadPS;

	public:
		EEQuad(const FLOAT3& _position, FLOAT _width, FLOAT _height);
		EEQuad(const Rect_Float& _rect);
		EEQuad(const FLOAT3& _position, FLOAT _width, FLOAT _height, const EETexture& _tex);
		EEQuad(const Rect_Float& _rect, const EETexture& _tex);
		EEQuad(const Rect_Float& _rect, ID3D11ShaderResourceView* _tex);
		EEQuad(const EEQuad& _quad);
		~EEQuad();

		virtual bool Update();
		virtual bool Render();

		//position
		virtual void SetPositionX(float _posX);
		virtual void SetPositionY(float _posY);
		virtual void SetPositionXY(const FLOAT2& _pos);
		virtual void SetPosition(const FLOAT3& _pos);
		virtual void SetRect(const Rect_Float& _rect);
		virtual void SetWidth(float _width);
		virtual void SetHeight(float _height);
		//texture
		virtual bool SetTexture(EETexture* _tex);
		virtual bool SetTexture(ID3D11ShaderResourceView* _tex);

		//position
		virtual const Rect_Float& GetRect() const;
		virtual float GetWidht() const;
		virtual float GetHeight() const;
		virtual FLOAT3 GetCenter() const;
		//texture
		virtual EETexture* GetTexture();

		//position
		virtual Rect_Float GetFinalRect() const;
		virtual FLOAT3 GetFinalCenter() const;

	protected:
		bool CreateQuadVertexBuffer();

	protected:
		//the size of the quad
		Rect_Float m_quadRect;
		float m_quadWidth, m_quadHeight;
		//the vertex buffer of the quad
		ID3D11Buffer *m_quadVB;
		//the texture of the quad if need
		EETexture m_quadTex;
	};
}


#endif
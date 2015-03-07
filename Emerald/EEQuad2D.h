#pragma once
#ifndef _EE_QUAD2D_H_
#define _EE_QUAD2D_H_

#include "EEObject.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEQuad2DBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEQuad2DBufferDesc
	{
		int isUseColor;
		int isUseTex;
		float tmp2;
		float tmp3;
	};

	//EEQuad2DVertex
	//----------------------------------------------------------------------------------------------------
	struct EEQuad2DVertex
	{
		FLOAT3 pos;
		FLOAT2 tex;
	};

	//EEQuad2D
	//----------------------------------------------------------------------------------------------------
	class EEQuad2D : public EEObject
	{
	protected:
		static bool InitializeQuad2D();

	protected:
		static bool s_isQuad2DInitialized;
		static ID3D11InputLayout *s_quadIL;
		static ID3D11VertexShader *s_quadVS;
		static ID3D11PixelShader  *s_quadPS;
		static ID3D11Buffer *s_quad2DBuffer;

	public:
		EEQuad2D();
		EEQuad2D(const FLOAT3& _position);
		EEQuad2D(const FLOAT3& _position, FLOAT _width, FLOAT _height);
		EEQuad2D(const FLOAT3& _position, FLOAT _width, FLOAT _height, const EETexture& _tex);
		EEQuad2D(const Rect_Float& _rect);
		EEQuad2D(const Rect_Float& _rect, const EEColor& _color);
		EEQuad2D(const Rect_Float& _rect, const EETexture& _tex);
		EEQuad2D(const Rect_Float& _rect, ID3D11ShaderResourceView* _tex);
		EEQuad2D(const EEQuad2D& _quad);
		virtual ~EEQuad2D();

		virtual bool Update();
		virtual bool Render();

		//position
		virtual void SetPositionX(float _posX);
		virtual void SetPositionY(float _posY);
		virtual void SetPositionXY(const FLOAT2& _position);
		virtual void SetPosition(const FLOAT3& _position);
		virtual void SetRect(const Rect_Float& _rect);
		virtual void SetWidth(float _width);
		virtual void SetHeight(float _height);
		//texture
		virtual bool SetTexture(const EETexture& _tex);
		virtual bool SetTexture(ID3D11ShaderResourceView* _tex);
		virtual bool SetIsUseColor(bool _isUseColor);
		virtual bool SetIsUseTex(bool _isUseTex);

		//position
		virtual const Rect_Float& GetRect() const;
		virtual float GetWidht() const;
		virtual float GetHeight() const;
		virtual FLOAT3 GetCenter() const;
		virtual FLOAT3 GetRowCenter() const;
		virtual MATRIX GetViewMatrix();
		virtual MATRIX GetProjectionMatrix();
		//texture
		virtual EETexture* GetTexture();

		//position
		virtual Rect_Float GetFinalRect() const;
		virtual FLOAT3 GetFinalCenter() const;

	protected:
		//cbuffer
		virtual bool MapQuad2DBuffer();
		//state
		virtual bool UpdateObjectState();
		//vbuffer
		bool CreateQuadVertexBuffer(int _verticesNum = 4);

	protected:
		//the size of the quad
		Rect_Float m_quadRect;
		float m_quadWidth, m_quadHeight;
		//the vertex buffer of the quad
		ID3D11Buffer *m_quadVB;
		//the texture of the quad if need
		EETexture m_quadTex;
		bool m_isUseColor;
		bool m_isUseTex;
	};
}


#endif
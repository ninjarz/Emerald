#pragma once
#ifndef _EE_CURVE2D_H_
#define _EE_CURVE2D_H_

#include "EEObject2D.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EECurve2DVertex
	//----------------------------------------------------------------------------------------------------
	struct EECurve2DVertex
	{
		FLOAT3 pos;
		FLOAT2 tex;
	};

	// EECurve2D
	//----------------------------------------------------------------------------------------------------
	class EECurve2D : public EEObject2D
	{
	protected:
		static bool InitializeCurve2D();

	protected:
		static bool s_isCurve2DInitialized;
		static ID3D11InputLayout *s_curveIL;
		static ID3D11VertexShader *s_curveVS;
		static ID3D11PixelShader *s_curvePS;
		static ID3D11Buffer *s_curveBuffer;

	public:
		EECurve2D(const std::vector<FLOAT2>& _curve, const EEColor& _color);
		virtual ~EECurve2D();

		virtual bool Update();
		virtual bool Render();

		void SetWidth(float _width);

	protected:
		bool CreateCurveVertexBuffer();

	protected:
		std::vector<FLOAT2> m_curve;
		float m_width;
		bool m_isCurveDirty;
		ID3D11Buffer *m_curveVB;
	};

}


#endif
#pragma once
#ifndef _EE_POINTS2D_H_
#define _EE_POINTS2D_H_

#include <vector>
#include "EEObject2D.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEPoints2DVertex
	//----------------------------------------------------------------------------------------------------
	struct EEPoints2DVertex
	{
		FLOAT3 pos;
	};

	//EEPoints2D
	//----------------------------------------------------------------------------------------------------
	class EEPoints2D : public EEObject2D
	{
	protected:
		static bool InitializePoints2D();

	protected:
		static bool s_isPoints2DInitialized;
		static ID3D11InputLayout *s_pointsIL;
		static ID3D11VertexShader *s_pointsVS;
		static ID3D11PixelShader *s_pointsPS;
		static ID3D11Buffer *s_points2DBuffer;

	public:
		EEPoints2D(std::vector<FLOAT2>& _points);
		virtual ~EEPoints2D();

		virtual bool Update();
		virtual bool Render();

		void AddPoints(std::vector<FLOAT2>& _points);

	protected:
		bool CreatePointsVertexBuffer();

	protected:
		bool m_isPointsDirty;
		std::vector<FLOAT2> m_points;
		ID3D11Buffer *m_pointsVB;
	};

}


#endif
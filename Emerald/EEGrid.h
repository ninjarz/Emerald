#pragma once
#ifndef _EE_GRID_H_
#define _EE_GRID_H_

#include "EEObject3D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEGridVertex
	//----------------------------------------------------------------------------------------------------
	struct EEGridVertex
	{
		FLOAT3 pos;
		FLOAT3 normal;
		FLOAT2 tex;
	};

	// EEGrid
	//----------------------------------------------------------------------------------------------------
	class EEGrid : public EEObject3D
	{
	public:
		EEGrid(float _width, float _height, unsigned int _m, unsigned int _n);
		EEGrid(const EEGrid& _grid);
		virtual ~EEGrid();
		virtual inline EEObject* Clone() { return new EEGrid(*this); }

		virtual bool Update();
		virtual bool Render();

	protected:
		//vbuffer and ibuffer
		bool CreateGridBuffer();

	protected:
		//the size of the grid
		FLOAT2 m_size;
		int m_m;
		int m_n;
	};
}


#endif
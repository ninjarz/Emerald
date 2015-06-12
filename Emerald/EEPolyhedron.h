#pragma once
#ifndef _EE_POLYHEDRON_H_
#define _EE_POLYHEDRON_H_

#include "EEObject3D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEPolyhedronVertex
	//----------------------------------------------------------------------------------------------------
	struct EEPolyhedronVertex
	{
		FLOAT3 pos;
		FLOAT3 normal;
		FLOAT2 tex;
	};

	// EEPolyhedron
	//----------------------------------------------------------------------------------------------------
	class EEPolyhedron : public EEObject3D
	{
	public:
		EEPolyhedron(const LPCWSTR _fileName);
		EEPolyhedron(const LPCWSTR _fileName, const EETexture& _tex);
		EEPolyhedron(const EEPolyhedron& _polyhedron);
		virtual ~EEPolyhedron();
		virtual inline EEObject* Clone() { return new EEPolyhedron(*this); }

		virtual bool Update();
		virtual bool Render();

	protected:
		//vbuffer and ibuffer
		bool CreatePolyhedronBuffer(const LPCWSTR _fileName);

	protected:
		//the size of the polyhedron
		float m_radius;
		int m_slice;
		int m_stack;
	};
}


#endif
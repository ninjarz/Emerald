#pragma once
#ifndef _EE_PARTICLE_H_
#define _EE_PARTICLE_H_

#include <vector>
#include "EEGeometry.h"
#include "EEColor.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEParticleInfo
	//----------------------------------------------------------------------------------------------------
	struct EEParticleInfo
	{
		//amount
		int amount;
		//life
		FLOAT durationTime;
		FLOAT deltaDurationTime;
		//position
		FLOAT3 position;
		FLOAT3 deltaPosition;
		FLOAT width;
		FLOAT height;
		//direction
		FLOAT3 direction;
		//color
		EEColor beginColor;
		EEColor deltaBeginColor;
		EEColor endColor;
		EEColor deltaEndColor;
		//scale
		FLOAT beginScale;
		FLOAT deltaBeginScale;
		FLOAT endScale;
		FLOAT deltaEndScale;
		//texture
		EETexture texture;

		inline EEParticleInfo& operator= (const EEParticleInfo& _info)
		{
			memcpy(this, &_info, sizeof(EEParticleInfo));
		}
	};

	//EEParticle
	//----------------------------------------------------------------------------------------------------
	class EEParticle : public EEQuad
	{
	public:
		EEParticle(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture);
		EEParticle(const EEParticle& _particle);
		~EEParticle();

	protected:
		//life
		FLOAT m_durationTime;
		//direction
		FLOAT3 m_positionSpeed;
		//color
		EEColor m_colorSpeed;
		//scale
		FLOAT m_scaleSpeed;
	};

	//EEParticleEmitter
	//----------------------------------------------------------------------------------------------------
	class EEParticleEmitter : public EEObject
	{
	public:
		EEParticleEmitter(const EEParticleInfo& _info);
		EEParticleEmitter(const EEParticleEmitter& _emitter);
		~EEParticleEmitter();

		virtual bool Update();
		virtual bool Render();
		EEParticle* GeneratePaticle();

		//position
		virtual FLOAT3 GetCenter() const;

		//position
		virtual FLOAT3 GetFinalCenter() const;

	protected:
		EEParticleInfo m_particleInfo;
		std::vector<EEParticle*> m_particles;
	};
}

#endif
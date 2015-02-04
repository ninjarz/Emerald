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
		FLOAT3 direction;
		FLOAT3 deltaDirection;
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
	class EEParticle
	{
	public:
		virtual bool Update() = NULL;
		virtual bool Render() = NULL;
		virtual void LoadDate(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture) = NULL;

		virtual bool IsAlive() = NULL;
	};

	//EEParticle2D
	//----------------------------------------------------------------------------------------------------
	class EEParticle2D : public EEParticle, public EEQuad2D
	{
	public:
		EEParticle2D(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture);
		EEParticle2D(const EEParticle2D& _particle);
		~EEParticle2D();

		virtual bool Update();
		virtual bool Render();
		void LoadDate(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture);

		bool IsAlive();

	protected:
		//life
		bool m_isAlive;
		FLOAT m_durationTime;
		//position
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
		EEParticle* GenerateParticle();
		void RecastParticle(EEParticle* _particle);

		//localZOrder
		virtual void SetLocalZOrder(float _localZOrder);

		//position
		virtual FLOAT3 GetCenter() const;
		virtual FLOAT3 GetFinalCenter() const;

	protected:
		EEParticleInfo m_particleInfo;
		std::vector<EEParticle*> m_particles;
	};
}

#endif
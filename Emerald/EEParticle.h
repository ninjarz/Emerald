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
			// do not use memcpy!!!
			//memcpy(this, &_info, sizeof(EEParticleInfo));

			//amount
			amount = _info.amount;
			//life
			durationTime = _info.durationTime;
			deltaDurationTime = _info.deltaDurationTime;
			//position
			position = _info.position;
			deltaPosition = _info.deltaPosition;
			width = _info.width;
			height = _info.height;
			direction = _info.direction;
			deltaDirection = _info.deltaDirection;
			//color
			beginColor = _info.beginColor;
			deltaBeginColor = _info.deltaBeginColor;
			endColor = _info.endColor;
			deltaEndColor = _info.deltaEndColor;
			//scale
			beginScale = _info.beginScale;
			deltaBeginScale = _info.deltaBeginScale;
			endScale = _info.endScale;
			deltaEndScale = _info.deltaEndScale;
			//texture
			texture = _info.texture;

			return *this;
		}
	};

	//EEParticle
	//----------------------------------------------------------------------------------------------------
	class EEParticle
	{
	public:
		inline virtual ~EEParticle() {};

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
		virtual ~EEParticle2D();

		virtual bool Update();
		virtual bool Render();
		void LoadDate(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture);

		bool IsAlive();

	protected:
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
		EEParticleEmitter();
		EEParticleEmitter(const EEParticleInfo& _info);
		EEParticleEmitter(const EEParticleEmitter& _emitter);
		virtual ~EEParticleEmitter();

		virtual bool Update();
		virtual bool Render();
		EEParticle* GenerateParticle();
		void RecastParticle(EEParticle* _particle);

		bool SetParticleInfo(const EEParticleInfo& _info);
		virtual void SetLocalZOrder(float _localZOrder);

		virtual FLOAT3 GetCenter() const;
		virtual FLOAT3 GetFinalCenter() const;

	protected:
		EEParticleInfo m_particleInfo;
		std::vector<EEParticle*> m_particles;
	};
}

#endif
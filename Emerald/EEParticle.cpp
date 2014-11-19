#include "EEParticle.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEParticle
	//----------------------------------------------------------------------------------------------------
	EEParticle::EEParticle(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture)
		:
		m_durationTime(_durationTime),
		EEQuad(_position, _width, _height, _texture),
		m_positionSpeed(_positionSpeed),
		m_colorSpeed(_colorSpeed),
		m_scaleSpeed(_scaleSpeed)
	{
		SetColor(_color);
		SetScale(_scale);
	}

	//----------------------------------------------------------------------------------------------------
	EEParticle::EEParticle(const EEParticle& _particle)
		:
		m_durationTime(_particle.m_durationTime),
		EEQuad(_particle),
		m_positionSpeed(_particle.m_positionSpeed),
		m_colorSpeed(_particle.m_colorSpeed),
		m_scaleSpeed(_particle.m_scaleSpeed)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEParticle::~EEParticle()
	{

	}


	//EEParticleEmitter
	//----------------------------------------------------------------------------------------------------
	EEParticleEmitter::EEParticleEmitter(const EEParticleInfo& _info)
		:
		EEObject(),
		m_particleInfo(_info)
	{
		m_position = _info.position;
		for (int i = 0; i < _info.amount; ++i)
		{
			m_particles.push_back(GeneratePaticle());
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEParticleEmitter::EEParticleEmitter(const EEParticleEmitter& _emitter)
		:
		m_particleInfo(_emitter.m_particleInfo),
		m_particles(_emitter.m_particles)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEParticleEmitter::~EEParticleEmitter()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEParticleEmitter::Update()
	{
		for (EEParticle* particle : m_particles)
		{
			particle->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEParticleEmitter::Render()
	{
		for (EEParticle* particle : m_particles)
		{
			particle->Render();
		}

		return true;
	}

	EEParticle* EEParticleEmitter::GeneratePaticle()
	{
		FLOAT durationTime = m_particleInfo.durationTime + m_particleInfo.deltaDurationTime * ((rand() / double(RAND_MAX)) - 0.5f);
		FLOAT3 position = m_particleInfo.deltaPosition * ((rand() / double(RAND_MAX)) - 0.5f);
		EEColor color = m_particleInfo.beginColor + ((rand() / double(RAND_MAX)) - 0.5f) * m_particleInfo.deltaBeginColor;
		EEColor colorSpeed = (m_particleInfo.endColor + ((rand() / double(RAND_MAX)) - 0.5f) * m_particleInfo.deltaEndColor - color) / durationTime;
		FLOAT scale = m_particleInfo.beginScale + ((rand() / double(RAND_MAX)) - 0.5f) * m_particleInfo.deltaBeginScale;
		FLOAT scaleSpeed = (m_particleInfo.endScale + ((rand() / double(RAND_MAX)) - 0.5f) * m_particleInfo.deltaEndScale - scale) / durationTime;

		EEParticle* particle =  new EEParticle(
			durationTime,
			position,
			m_particleInfo.width,
			m_particleInfo.height,
			m_particleInfo.direction,
			color,
			colorSpeed,
			scale,
			scaleSpeed,
			m_particleInfo.texture
			);
		particle->SetParent(this);
		return particle;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEParticleEmitter::GetCenter() const
	{
		return m_position;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEParticleEmitter::GetFinalCenter() const
	{
		return GetFinalPosition();
	}
}
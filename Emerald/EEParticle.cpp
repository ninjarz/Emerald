#include "EEParticle.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEParticle2D
	//----------------------------------------------------------------------------------------------------
	EEParticle2D::EEParticle2D(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture)
		:
		m_durationTime(_durationTime),
		EEQuad2D(_position, _width, _height, _texture),
		m_positionSpeed(_positionSpeed),
		m_colorSpeed(_colorSpeed),
		m_scaleSpeed(_scaleSpeed)
	{
		m_isUseColor = true;
		SetColor(_color);
		SetScale(_scale);
	}

	//----------------------------------------------------------------------------------------------------
	EEParticle2D::EEParticle2D(const EEParticle2D& _particle)
		:
		m_durationTime(_particle.m_durationTime),
		EEQuad2D(_particle),
		m_positionSpeed(_particle.m_positionSpeed),
		m_colorSpeed(_particle.m_colorSpeed),
		m_scaleSpeed(_particle.m_scaleSpeed)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEParticle2D::~EEParticle2D()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEParticle2D::Update()
	{
		if (m_isAlive)
		{
			FLOAT deltaTime = (FLOAT)EECore::s_EECore->GetDeltaTime();

			if (m_durationTime <= deltaTime)
			{
				SetPosition(GetPosition() + m_positionSpeed * m_durationTime);
				SetColor(GetColor() + m_colorSpeed * m_durationTime);
				SetScale(GetScale() + m_scaleSpeed * m_durationTime);
				m_durationTime = 0.0f;
				m_isAlive = false;
			}
			else
			{
				SetPosition(GetPosition() + m_positionSpeed * deltaTime);
				SetColor(GetColor() + m_colorSpeed * deltaTime);
				SetScale(GetScale() + m_scaleSpeed * deltaTime);
				m_durationTime -= deltaTime;
			}

			return EEQuad2D::Update();
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEParticle2D::Render()
	{
		if (m_isAlive)
		{
			return EEQuad2D::Render();
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	void EEParticle2D::LoadDate(FLOAT _durationTime, const FLOAT3& _position, FLOAT _width, FLOAT _height, const FLOAT3& _positionSpeed, const EEColor& _color, const EEColor& _colorSpeed, FLOAT _scale, FLOAT _scaleSpeed, const EETexture& _texture)
	{
		m_isAlive = true;
		m_durationTime = _durationTime;
		SetPosition(_position);
		SetWidth(_width);
		SetHeight(_height);
		m_positionSpeed = _positionSpeed;
		SetColor(_color);
		m_colorSpeed = _colorSpeed;
		SetScale(_scale);
		m_scaleSpeed = _scaleSpeed;
		m_tex = _texture;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEParticle2D::IsAlive()
	{
		return m_isAlive;
	}

	//EEParticleEmitter
	//----------------------------------------------------------------------------------------------------
	EEParticleEmitter::EEParticleEmitter(const EEParticleInfo& _info)
		:
		EEObject(),
		m_particleInfo(_info)
	{
		m_position = _info.position;
		SetLocalZOrder(_info.position.z);
		for (int i = 0; i < _info.amount; ++i)
		{
			m_particles.push_back(GenerateParticle());
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
		for (EEParticle* paticle : m_particles)
			SAFE_DELETE(paticle);
		m_particles.clear();
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
			if (!particle->IsAlive())
				RecastParticle(particle);
			particle->Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEParticle* EEParticleEmitter::GenerateParticle()
	{
		FLOAT durationTime = m_particleInfo.durationTime + (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f) * m_particleInfo.deltaDurationTime;
		FLOAT3 position = (Rand_FLOAT3() * 2.0f - 1.0f) * m_particleInfo.deltaPosition;
		position.z = 0.0f;
		FLOAT3 positionSpeed = m_particleInfo.direction + (Rand_FLOAT3() * 2.0f - 1.0f) * m_particleInfo.deltaDirection;
		EEColor color = m_particleInfo.beginColor + (Rand_FLOAT4() * 2.0f - 1.0f) * m_particleInfo.deltaBeginColor;
		EEColor colorSpeed = (m_particleInfo.endColor + (Rand_FLOAT4() * 2.0f - 1.0f) * m_particleInfo.deltaEndColor - color) / durationTime;
		FLOAT scale = m_particleInfo.beginScale + (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f) * m_particleInfo.deltaBeginScale;
		FLOAT scaleSpeed = (m_particleInfo.endScale + (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f) * m_particleInfo.deltaEndScale - scale) / durationTime;

		EEParticle2D* particle =  new EEParticle2D(
			durationTime,
			position,
			m_particleInfo.width,
			m_particleInfo.height,
			positionSpeed,
			color,
			colorSpeed,
			scale,
			scaleSpeed,
			m_particleInfo.texture
			);
		particle->SetParent(this);
		particle->SetLocalZOrder(GetLocalZOrder());
		return particle;
	}

	//----------------------------------------------------------------------------------------------------
	void EEParticleEmitter::RecastParticle(EEParticle* _particle)
	{
		FLOAT durationTime = m_particleInfo.durationTime + (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f) * m_particleInfo.deltaDurationTime;
		FLOAT3 position = (Rand_FLOAT3() * 2.0f - 1.0f) * m_particleInfo.deltaPosition;
		position.z = 0.0f;
		FLOAT3 positionSpeed = m_particleInfo.direction + (Rand_FLOAT3() * 2.0f - 1.0f) * m_particleInfo.deltaDirection;
		EEColor color = m_particleInfo.beginColor + (Rand_FLOAT4() * 2.0f - 1.0f) * m_particleInfo.deltaBeginColor;
		EEColor colorSpeed = (m_particleInfo.endColor + (Rand_FLOAT4() * 2.0f - 1.0f) * m_particleInfo.deltaEndColor - color) / durationTime;
		FLOAT scale = m_particleInfo.beginScale + (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f) * m_particleInfo.deltaBeginScale;
		FLOAT scaleSpeed = (m_particleInfo.endScale + (float)((rand() / double(RAND_MAX)) * 2.0f - 1.0f) * m_particleInfo.deltaEndScale - scale) / durationTime;

		_particle->LoadDate(
			durationTime,
			position,
			m_particleInfo.width,
			m_particleInfo.height,
			positionSpeed,
			color,
			colorSpeed,
			scale,
			scaleSpeed,
			m_particleInfo.texture
			);
	}

	//----------------------------------------------------------------------------------------------------
	void EEParticleEmitter::SetLocalZOrder(float _localZOrder)
	{
		EEObject::SetLocalZOrder(_localZOrder);
		for (EEParticle* particle : m_particles)
		{
			//I do it in a wrong way... Use typeid
			((EEParticle2D*)particle)->SetLocalZOrder(_localZOrder);
		}
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
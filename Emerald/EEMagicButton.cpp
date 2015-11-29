#include "EEMagicButton.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEButton
	//----------------------------------------------------------------------------------------------------
	EEMagicButton::EEMagicButton(const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, std::function<void(void)> _funcPtr)
		:
		EEQuad2D(_rect, _tex),
		//scale
		m_aimScale(_scale),
		m_currScaleTime(0.0),
		m_aimScaleTime(_scaleTime),
		//alpha
		m_aimAlpha(0.0f),
		m_currFadeTime(0.0f),
		m_aimFadeTime(_fadeTime),
		//callback function
		m_callbackFunc(_funcPtr)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEMagicButton::EEMagicButton(const EEMagicButton& _Button)
		:
		EEQuad2D(_Button),
		//scale
		m_aimScale(_Button.m_aimScale),
		m_currScaleTime(_Button.m_currScaleTime),
		m_aimScaleTime(_Button.m_aimScaleTime),
		//alpha
		m_aimAlpha(_Button.m_aimAlpha),
		m_currFadeTime(_Button.m_currFadeTime),
		m_aimFadeTime(_Button.m_aimFadeTime),
		//callback function
		m_callbackFunc(_Button.m_callbackFunc)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEMagicButton::~EEMagicButton()
	{
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMagicButton::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty)
		{
			FLOAT3 scale = FLOAT3((m_aimScale - 1.0f) * (m_currScaleTime / m_aimScaleTime) * 0.5f);

			Rect_Float rect(
				-m_quadWidth / 2 - m_quadWidth * scale.x,
				-m_quadHeight / 2 - m_quadHeight * scale.y,
				m_quadWidth / 2 + m_quadWidth * scale.x,
				m_quadHeight / 2 + m_quadHeight * scale.y
				);

			EEQuad2DVertex vertices[4];
			vertices[0].pos = FLOAT3(rect.x, rect.y, m_localZOrder * 0.0001f);
			vertices[0].tex = FLOAT2(0, 0);
			vertices[1].pos = FLOAT3(rect.z, rect.y, m_localZOrder * 0.0001f);
			vertices[1].tex = FLOAT2(1, 0);
			vertices[2].pos = FLOAT3(rect.x, rect.w, m_localZOrder * 0.0001f);
			vertices[2].tex = FLOAT2(0, 1);
			vertices[3].pos = FLOAT3(rect.z, rect.w, m_localZOrder * 0.0001f);
			vertices[3].tex = FLOAT2(1, 1);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_quadVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, vertices, sizeof(vertices));
			deviceContext->Unmap(m_quadVB, 0);

			m_isPositionDirty = false;
			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMagicButton::Render()
	{
		if (!EEObject::Render())
			return false;

		float finalAlpha = GetFinalAlpha() * (1.0f - (m_currFadeTime / m_aimFadeTime));
		MapObjectBuffer(finalAlpha);

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();

		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		deviceConstext->IASetInputLayout(s_quadIL);
		UINT stride = sizeof(EEQuad2DVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_quadVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_quadVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_quadPS, NULL, 0);
		deviceConstext->Draw(4, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	Rect_Float EEMagicButton::GetFinalRect() const
	{
		if (m_parent)
		{
			FLOAT3 scale = (m_parent->GetFinalScale() * m_scale * (1.0f + (m_aimScale - 1.0f) * (m_currScaleTime / m_aimScaleTime)) - 1.0f) * 0.5f;
			FLOAT3 deltaPos = m_parent->GetFinalPosition();
			return Rect_Float(
				m_quadRect.x - m_quadWidth * scale.x + deltaPos.x,
				m_quadRect.y - m_quadHeight * scale.y + deltaPos.y,
				m_quadRect.z + m_quadWidth * scale.x + deltaPos.x,
				m_quadRect.w + m_quadHeight * scale.y + deltaPos.y
				);
		}
		else
		{
			FLOAT3 scale = (m_scale * (1.0f + (m_aimScale - 1.0f) * (m_currScaleTime / m_aimScaleTime)) - 1.0f) * 0.5f;
			return Rect_Float(
				m_quadRect.x - m_quadWidth * scale.x,
				m_quadRect.y - m_quadHeight * scale.y,
				m_quadRect.z + m_quadWidth * scale.x,
				m_quadRect.w + m_quadHeight * scale.y
				);
		}

		return Rect_Float(0.0f, 0.0f, 0.0f, 0.0f);
	}

	//----------------------------------------------------------------------------------------------------
	void EEMagicButton::OnMouseFree(const Point& _pos)
	{
		EEObject::OnMouseFree(_pos);

		if (m_currScaleTime)
			m_isScaleDirty = true;
		m_currScaleTime -= (float)EECore::s_EECore->GetDeltaTime();
		if (m_currScaleTime < 0.0f)
			m_currScaleTime = 0.0f;
		m_currFadeTime -= (float)EECore::s_EECore->GetDeltaTime();
		if (m_currFadeTime < 0.0f)
			m_currFadeTime = 0.0f;
	}

	//----------------------------------------------------------------------------------------------------
	void EEMagicButton::OnMouseOver(const Point& _pos)
	{
		EEObject::OnMouseOver(_pos);

		if (m_currScaleTime < m_aimScaleTime)
			m_isScaleDirty = true;
		m_currScaleTime += (float)EECore::s_EECore->GetDeltaTime();
		if (m_currScaleTime > m_aimScaleTime)
			m_currScaleTime = m_aimScaleTime;
	}

	//----------------------------------------------------------------------------------------------------
	void EEMagicButton::OnMouseClicked(const Point& _pos)
	{
		EEObject::OnMouseClicked(_pos);
	}

	//----------------------------------------------------------------------------------------------------
	void EEMagicButton::OnMouseTriggered(const Point& _pos)
	{
		EEObject::OnMouseTriggered(_pos);

		m_currFadeTime += (float)EECore::s_EECore->GetDeltaTime();
		if (m_currFadeTime >= m_aimFadeTime)
		{
			m_currFadeTime = m_aimFadeTime;
			if (m_callbackFunc)
				m_callbackFunc();
			s_triggeredObject = nullptr;
		}

		m_isScaleDirty = true;
	}
}
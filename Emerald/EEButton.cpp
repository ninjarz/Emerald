#include "EEButton.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEButton
	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, const EETexture& _tex, std::function<void(void)> _funcPtr)
		:
		EEQuad2D(_rect, _tex),
		//type
		m_type(_type),
		//scale
		m_aimScale(1.0),
		m_currScaleTime(0.0),
		m_aimScaleTime(0.0),
		//alpha
		m_aimAlpha(1.0f),
		m_currFadeTime(0.0f),
		m_aimFadeTime(0.0f),
		//callback function
		m_callbackFunc(_funcPtr),
		//button picture
		m_overTex(_tex),
		m_downTex(_tex)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, const EETexture& _upTex, const EETexture& _overTex, const EETexture& _downTex, std::function<void(void)> _funcPtr)
		:
		EEQuad2D(_rect, _upTex),
		//type
		m_type(_type),
		//scale
		m_aimScale(1.0),
		m_currScaleTime(0.0),
		m_aimScaleTime(0.0),
		//alpha
		m_aimAlpha(1.0f),
		m_currFadeTime(0.0f),
		m_aimFadeTime(0.0f),
		//callback function
		m_callbackFunc(_funcPtr),
		//button picture
		m_overTex(_overTex),
		m_downTex(_downTex)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, ID3D11ShaderResourceView* _upTex, ID3D11ShaderResourceView* _overTex, ID3D11ShaderResourceView* _downTex, std::function<void(void)> _funcPtr)
		:
		EEQuad2D(_rect, _upTex),
		//type
		m_type(_type),
		//scale
		m_aimScale(1.0),
		m_currScaleTime(0.0),
		m_aimScaleTime(0.0),
		//alpha
		m_aimAlpha(1.0f),
		m_currFadeTime(0.0f),
		m_aimFadeTime(0.0f),
		//callback function
		m_callbackFunc(_funcPtr),
		//button picture
		m_overTex(_overTex),
		m_downTex(_downTex)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, std::function<void(void)> _funcPtr)
		:
		EEQuad2D(_rect, _tex),
		//type
		m_type(_type),
		//scale
		m_aimScale(_scale),
		m_currScaleTime(0.0),
		m_aimScaleTime(_scaleTime),
		//alpha
		m_aimAlpha(0.0f),
		m_currFadeTime(0.0f),
		m_aimFadeTime(_fadeTime),
		//callback function
		m_callbackFunc(_funcPtr),
		//button picture
		m_overTex(),
		m_downTex()
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float &_rect, float _scale, float _scaleTime, float _fadeTime, ID3D11ShaderResourceView *_tex, std::function<void(void)> _funcPtr)
		:
		EEQuad2D(_rect, _tex),
		//type
		m_type(_type),
		//scale
		m_aimScale(_scale),
		m_currScaleTime(0.0),
		m_aimScaleTime(_scaleTime),
		//alpha
		m_aimAlpha(0.0f),
		m_currFadeTime(0.0f),
		m_aimFadeTime(_fadeTime),
		//callback function
		m_callbackFunc(_funcPtr),
		//button picture
		m_overTex(),
		m_downTex()
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(const EEButton& _Button)
		:
		EEQuad2D(_Button),
		//type
		m_type(_Button.m_type),
		//scale
		m_aimScale(_Button.m_aimScale),
		m_currScaleTime(_Button.m_currScaleTime),
		m_aimScaleTime(_Button.m_aimScaleTime),
		//alpha
		m_aimAlpha(_Button.m_aimAlpha),
		m_currFadeTime(_Button.m_currFadeTime),
		m_aimFadeTime(_Button.m_aimFadeTime),
		//callback function
		m_callbackFunc(_Button.m_callbackFunc),
		//button picture
		m_overTex(_Button.m_overTex),
		m_downTex(_Button.m_downTex)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEButton::~EEButton()
	{
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		switch (m_type)
		{
		case EE_BUTTON_THREE:
		{
								return Update_THREE();
		}
		case EE_BUTTON_SCALE:
		{
								return Update_SCALE();
		}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::Render()
	{
		if (!EEObject::Render())
			return false;

		switch (m_type)
		{
		case EE_BUTTON_THREE:
		{
								return Render_THREE();
		}
		case EE_BUTTON_SCALE:
		{
								return Render_SCALE();
		}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	Rect_Float EEButton::GetFinalRect()
	{
		switch (m_type)
		{
		case EE_BUTTON_THREE:
		{
								return GetFinalRect_THREE();
		}
		case EE_BUTTON_SCALE:
		{
								return GetFinalRect_SCALE();
		}
		}

		return Rect_Float(0.0f, 0.0f, 0.0f, 0.0f);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEButton::GetUpTex()
	{
		return &m_tex;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEButton::GetOverTex()
	{
		return &m_overTex;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEButton::GetDownTex()
	{
		return &m_downTex;
	}

	//----------------------------------------------------------------------------------------------------
	void EEButton::OnMouseOver(const Point& _pos)
	{
		EEObject::OnMouseOver(_pos);
	}

	//----------------------------------------------------------------------------------------------------
	void EEButton::OnMouseClicked(const Point& _pos)
	{
		EEObject::OnMouseClicked(_pos);
	}

	 //----------------------------------------------------------------------------------------------------
	void EEButton::OnMouseTriggered(const Point& _pos)
	{
		EEObject::OnMouseTriggered(_pos);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::UpdateObjectState()
	{
		EEQuad2D::UpdateObjectState();

		switch (m_type)
		{
		case EE_BUTTON_THREE:
		{
								if (s_triggeredObject == this)
								{
									if (m_callbackFunc)
									{
										//(*(void(*)())m_callbackFunc)();
										m_callbackFunc();
										s_triggeredObject = nullptr;
									}
								}
		}
			break;
		case EE_BUTTON_SCALE:
		{
								if (s_triggeredObject == this)
								{
									m_currFadeTime += (float)EECore::s_EECore->GetDeltaTime();
									if (m_currFadeTime >= m_aimFadeTime)
									{
										m_currFadeTime = m_aimFadeTime;
										if (m_callbackFunc)
											m_callbackFunc();
										s_triggeredObject = nullptr;
									}
									//the scale value is changed in a sense
									m_isScaleDirty = true;
								}
								else if (m_state == EE_OBJECT_FREE)
								{
									if (m_currScaleTime)
										m_isScaleDirty = true;
									m_currScaleTime -= (float)EECore::s_EECore->GetDeltaTime();
									if (m_currScaleTime < 0.0f)
										m_currScaleTime = 0.0f;
									m_currFadeTime -= (float)EECore::s_EECore->GetDeltaTime();
									if (m_currFadeTime < 0.0f)
										m_currFadeTime = 0.0f;
								}
								else if (m_state == EE_OBJECT_OVER)
								{
									//the scale value is changed in a sense
									if (m_currScaleTime < m_aimScaleTime)
										m_isScaleDirty = true;
									m_currScaleTime += (float)EECore::s_EECore->GetDeltaTime();
									if (m_currScaleTime > m_aimScaleTime)
										m_currScaleTime = m_aimScaleTime;
								}
		}
			break;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::Update_THREE()
	{
		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty)
		{
			Rect_Float rect(
				-m_quadWidth / 2,
				-m_quadHeight / 2,
				m_quadWidth / 2,
				m_quadHeight / 2
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
	bool EEButton::Render_THREE()
	{
		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();

		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		deviceConstext->IASetInputLayout(s_quadIL);
		UINT stride = sizeof(EEQuad2DVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_quadVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_quadVS, NULL, 0);
		ID3D11ShaderResourceView *texture = NULL;
		switch (m_state)
		{
		case EE_OBJECT_FREE:
			texture = m_tex.GetTexture();
			deviceConstext->PSSetShaderResources(0, 1, &texture);
			break;
		case EE_OBJECT_DOWN:
			texture = m_downTex.GetTexture();
			deviceConstext->PSSetShaderResources(0, 1, &texture);
			break;
		case EE_OBJECT_OVER:
			texture = m_overTex.GetTexture();
			deviceConstext->PSSetShaderResources(0, 1, &texture);
			break;
		}
		deviceConstext->PSSetShader(s_quadPS, NULL, 0);
		deviceConstext->Draw(4, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	Rect_Float EEButton::GetFinalRect_THREE()
	{
		if (m_parent)
		{
			FLOAT3 scale = (m_parent->GetFinalScale() * m_scale - 1.0f) * 0.5f;
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
			FLOAT3 scale = (m_scale - 1.0f) * 0.5f;
			return Rect_Float(
				m_quadRect.x - m_quadWidth * scale.x,
				m_quadRect.y - m_quadHeight * scale.y,
				m_quadRect.z + m_quadWidth * scale.x,
				m_quadRect.w + m_quadHeight * scale.y
				);
		}
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::Update_SCALE()
	{
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
	bool EEButton::Render_SCALE()
	{
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
	Rect_Float EEButton::GetFinalRect_SCALE()
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
	}
}
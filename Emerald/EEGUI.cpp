#include "EEGUI.h"
#include "EECore.h"
#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEProgressBar
	//----------------------------------------------------------------------------------------------------
	EEProgressbar::EEProgressbar(const Rect_Float& _progressRect, const Rect_Float& _frameRect, const EETexture& _progressTex, const EETexture& _frameTex, void(*_funcPtr)())
		:
		EEQuad(_progressRect, _progressTex),
		m_progressFrame(_frameRect, _frameTex),
		m_progress(0.0f),
		m_isProgressDirty(false),
		m_callbackFunc(_funcPtr)
	{
		m_progressFrame.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EEProgressbar::EEProgressbar(const EEProgressbar& _progressbar)
		:
		EEQuad(_progressbar),
		m_progressFrame(_progressbar.m_progressFrame),
		m_progress(_progressbar.m_progress),
		m_isProgressDirty(_progressbar.m_isProgressDirty),
		m_callbackFunc(_progressbar.m_callbackFunc)
	{
		m_progressFrame.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EEProgressbar::~EEProgressbar()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::Update()
	{
		if (m_isPositionDirty || m_isScaleDirty || m_isLocalZOrderDirty || m_isProgressDirty)
		{
			Rect_Float finalRect = GetFinalRect();
			//the value of the z depends on the progress (the scaled end - the scaled width * (1.0f - the progress)
			finalRect.z -= (finalRect.z - finalRect.x) * (1.0f - m_progress);

			EEQuadVertex vertices[4];
			vertices[0].pos = FLOAT3(finalRect.x, finalRect.y, m_localZOrder * 0.0001f);
			vertices[0].tex = FLOAT2(0, 0);
			vertices[1].pos = FLOAT3(finalRect.z, finalRect.y, m_localZOrder * 0.0001f);
			vertices[1].tex = FLOAT2(m_progress, 0);
			vertices[2].pos = FLOAT3(finalRect.x, finalRect.w, m_localZOrder * 0.0001f);
			vertices[2].tex = FLOAT2(0, 1);
			vertices[3].pos = FLOAT3(finalRect.z, finalRect.w, m_localZOrder * 0.0001f);
			vertices[3].tex = FLOAT2(m_progress, 1);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_quadVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, vertices, sizeof(vertices));
			deviceContext->Unmap(m_quadVB, 0);

			//the progress has been updated here
			m_isProgressDirty = false;
			m_isPositionDirty = false;
			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
		}

		m_progressFrame.Update();

		if (m_progress >= 1.0f)
		{
			if (m_callbackFunc)
			{
				(*m_callbackFunc)();
				m_callbackFunc = NULL;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::Render()
	{
		m_progressFrame.Render();
		EEQuad::Render();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::SetProgress(float _progress)
	{
		m_progress = _progress;
		m_isProgressDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEProgressbar::SetCallbackFunc(void(*_funcPtr)())
	{
		m_callbackFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	float EEProgressbar::GetProgress()
	{
		return m_progress;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEProgressbar::GetProgressTex()
	{
		return &m_quadTex;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEProgressbar::GetFrameTex()
	{
		return m_progressFrame.GetTexture();
	}

	//EEButton
	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, const EETexture& _upTex, const EETexture& _overTex, const EETexture& _downTex, void(*_funcPtr)())
		:
		EEQuad(_rect, _upTex),
		//type
		m_type(_type),
		//state
		m_state(EE_BUTTON_UP),
		m_isTriggered(false),
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

	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, ID3D11ShaderResourceView* _upTex, ID3D11ShaderResourceView* _overTex, ID3D11ShaderResourceView* _downTex, void(*_funcPtr)())
		:
		EEQuad(_rect, _upTex),
		//type
		m_type(_type),
		//state
		m_state(EE_BUTTON_UP),
		m_isTriggered(false),
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

	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, void(*_funcPtr)())
		:
		EEQuad(_rect, _tex),
		//type
		m_type(_type),
		//state
		m_state(EE_BUTTON_UP),
		m_isTriggered(false),
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

	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(EEButtonType _type, const Rect_Float &_rect, float _scale, float _scaleTime, float _fadeTime, ID3D11ShaderResourceView *_tex, void(*_funcPtr)())
		:
		EEQuad(_rect, _tex),
		//type
		m_type(_type),
		//state
		m_state(EE_BUTTON_UP),
		m_isTriggered(false),
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

	}

	//----------------------------------------------------------------------------------------------------
	EEButton::EEButton(const EEButton& _Button)
		:
		EEQuad(_Button),
		//type
		m_type(_Button.m_type),
		//state
		m_state(_Button.m_state),
		m_isTriggered(_Button.m_isTriggered),
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

	}

	//----------------------------------------------------------------------------------------------------
	EEButton::~EEButton()
	{
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::Update()
	{
		UpdateButtonState();

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
		return &m_quadTex;
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

	bool EEButton::UpdateButtonState()
	{
		if (m_state != EE_BUTTON_DOWN)
			m_state = EE_BUTTON_UP;
		if (EECollision(m_quadRect, EECore::s_EECore->GetMousePosition()))
		{
			//BUTTON DOWN
			if (EECore::s_EECore->IsKeyDown(VK_LBUTTON))
			{
				switch (m_type)
				{
				case EE_BUTTON_THREE:
				{
										m_state = EE_BUTTON_DOWN;
										m_isTriggered = false;
				}
					break;
				case EE_BUTTON_SCALE:
				{
										m_state = EE_BUTTON_DOWN;
										m_isTriggered = false;
				}
					break;
				}
			}
			//BUTTON DOWN TO UP
			else if (m_state == EE_BUTTON_DOWN)
			{
				m_state = EE_BUTTON_UP;
				m_isTriggered = true;
			}
			//BUTTON OVER
			else
			{
				switch (m_type)
				{
				case EE_BUTTON_THREE:
				{
										m_state = EE_BUTTON_OVER;
				}
					break;
				case EE_BUTTON_SCALE:
				{
										m_state = EE_BUTTON_OVER;
										m_currScaleTime += (float)EECore::s_EECore->GetDeltaTime();
										if (m_currScaleTime > m_aimScaleTime)
											m_currScaleTime = m_aimScaleTime;
										//the scale value is changed in a sense
										m_isScaleDirty = true;
				}
					break;
				}
			}
		}
		else if (m_isTriggered == false)
		{
			m_state = EE_BUTTON_UP;
			m_currScaleTime -= (float)EECore::s_EECore->GetDeltaTime();
			if (m_currScaleTime < 0.0f)
				m_currScaleTime = 0.0f;
			m_currFadeTime -= (float)EECore::s_EECore->GetDeltaTime();
			if (m_currFadeTime < 0.0f)
				m_currFadeTime = 0.0f;
			//the scale value is changed in a sense
			m_isScaleDirty = true;
		}
		if (m_isTriggered)
		{
			switch (m_type)
			{
			case EE_BUTTON_THREE:
			{
									if (m_callbackFunc)
									{
										(*m_callbackFunc)();
										m_isTriggered = false;
									}
			}
				break;
			case EE_BUTTON_SCALE:
			{
									m_currFadeTime += (float)EECore::s_EECore->GetDeltaTime();
									if (m_currFadeTime >= m_aimFadeTime)
									{
										m_currFadeTime = m_aimFadeTime;
										if (m_callbackFunc && m_isTriggered == true)
											(*m_callbackFunc)();
										m_isTriggered = false;
									}
									//the scale value is changed in a sense
									m_isScaleDirty = true;
			}
				break;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEButton::Update_THREE()
	{
		if (m_isPositionDirty || m_isScaleDirty || m_isLocalZOrderDirty)
		{
			Rect_Float finalRect = GetFinalRect_THREE();

			EEQuadVertex vertices[4];
			vertices[0].pos = FLOAT3(finalRect.x, finalRect.y, m_localZOrder * 0.0001f);
			vertices[0].tex = FLOAT2(0, 0);
			vertices[1].pos = FLOAT3(finalRect.z, finalRect.y, m_localZOrder * 0.0001f);
			vertices[1].tex = FLOAT2(1, 0);
			vertices[2].pos = FLOAT3(finalRect.x, finalRect.w, m_localZOrder * 0.0001f);
			vertices[2].tex = FLOAT2(0, 1);
			vertices[3].pos = FLOAT3(finalRect.z, finalRect.w, m_localZOrder * 0.0001f);
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
		UINT stride = sizeof(EEQuadVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_quadVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_quadVS, NULL, 0);
		ID3D11ShaderResourceView *texture = NULL;
		switch (m_state)
		{
		case EE_BUTTON_UP:
			texture = m_quadTex.GetTexture();
			deviceConstext->PSSetShaderResources(0, 1, &texture);
			break;
		case EE_BUTTON_DOWN:
			texture = m_downTex.GetTexture();
			deviceConstext->PSSetShaderResources(0, 1, &texture);
			break;
		case EE_BUTTON_OVER:
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
		if (m_isPositionDirty || m_isScaleDirty || m_isLocalZOrderDirty)
		{
			Rect_Float finalRect = GetFinalRect_SCALE();

			EEQuadVertex vertices[4];
			vertices[0].pos = FLOAT3(finalRect.x, finalRect.y, m_localZOrder * 0.0001f);
			vertices[0].tex = FLOAT2(0, 0);
			vertices[1].pos = FLOAT3(finalRect.z, finalRect.y, m_localZOrder * 0.0001f);
			vertices[1].tex = FLOAT2(1, 0);
			vertices[2].pos = FLOAT3(finalRect.x, finalRect.w, m_localZOrder * 0.0001f);
			vertices[2].tex = FLOAT2(0, 1);
			vertices[3].pos = FLOAT3(finalRect.z, finalRect.w, m_localZOrder * 0.0001f);
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
		UINT stride = sizeof(EEQuadVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_quadVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_quadVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_quadTex.GetTexture();
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

	//EEScene
	//----------------------------------------------------------------------------------------------------
	EEScene::EEScene(const Rect_Float &_rect, const EETexture& _tex)
		:
		EEQuad(_rect, _tex)
	{
		//controls

	}
	
	//----------------------------------------------------------------------------------------------------
	EEScene::EEScene(const Rect_Float &_rect, ID3D11ShaderResourceView *_tex)
		:
		EEQuad(_rect, _tex)
	{
		//controls

	}

	//----------------------------------------------------------------------------------------------------
	EEScene::EEScene(const EEScene &_scene)
		:
		EEQuad(_scene),
		m_objects(m_objects)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEScene::~EEScene()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEScene::Update()
	{
		EEQuad::Update();
		for (UINT i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i]->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScene::Render()
	{
		EEQuad::Render();
		for (UINT i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i]->Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEScene::AddObject(EEObject* _object)
	{
		_object->SetParent(this);
		//if (m_control.size() == m_control.capacity())
		//	m_control.reserve(m_control.size() + 1);
		m_objects.push_back(_object);
	}
}
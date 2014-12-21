#include "EEProgressBar.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEProgressBar
	//----------------------------------------------------------------------------------------------------
	EEProgressbar::EEProgressbar(const Rect_Float& _progressRect, const Rect_Float& _frameRect, const EETexture& _progressTex, const EETexture& _frameTex, void(*_funcPtr)())
		:
		EEQuad2D(_progressRect, _progressTex),
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
		EEQuad2D(_progressbar),
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

			EEQuad2DVertex vertices[4];
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
		EEQuad2D::Render();

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
}
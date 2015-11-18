#pragma once
#ifndef _EE_D3D_H_
#define _EE_D3D_H_

#include <d3d11.h>
#include "EETexture.h"
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EED3D
	//----------------------------------------------------------------------------------------------------
	class EED3D
	{
	public:
		EED3D();
		EED3D(const EED3D&);
		inline virtual ~EED3D() { Shutdown(); }

		bool Initialize(bool _isSSAA, bool _isVsync);
		void Shutdown();

		void BeginScene(float, float, float, float);
		void BeginScene(const FLOAT4&);
		void EndScene();
		void ClearDepthStencilView();

		bool SetDevice(ID3D11Device* _para);
		bool SetDeviceeContext(ID3D11DeviceContext* _para);
		bool SetScreenNear(float _para);
		bool SetScreenDeep(float _para);
		bool SetIsSSAA(bool _para);
		bool SetIsVsync(bool _para);
		bool SetRenderTarget(EETexture* _target = nullptr);

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		float GetScreenNear() const;
		float GetScreenDeep() const;
		bool GetIsSSAA() const;
		bool GetIsVsync() const;
		void GetVideoCardInfo(char*, int&);
		ID3D11RenderTargetView* GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthStencilView();

	protected:
		ID3D11Device *m_device;
		ID3D11DeviceContext *m_deviceContext;
		float m_screenNear;
		float m_screenDeep;
		bool m_isSSAA;
		bool m_isVsync;
		int m_videoCardMemory;
		char m_videoCardDescription[128];
		UINT m_x4MultiQuality;
		IDXGISwapChain *m_swapChain;
		ID3D11Texture2D *m_depthStencilBuffer;
		ID3D11RenderTargetView *m_renderTargetView;
		ID3D11DepthStencilView *m_depthStencilView;
	};

	//EED3D_APIs
	//----------------------------------------------------------------------------------------------------
	void EEBeginScene(float, float, float, float);
	void EEBeginScene(const FLOAT4&);
	void EEEndScene();
	void EEClearDepthStencilView();

	bool EESetRenderTarget(EETexture* _target = nullptr);

	ID3D11Device* EEGetDevice();
	ID3D11DeviceContext* EEGetDeviceContext();
	float EEGetScreenNear();
	float EEGetScreenDeep();
	bool EEGetIsSSAA();
	bool EEGetIsVsync();
	void EEGetVideoCardInfo(char*, int&);
	ID3D11RenderTargetView* EEGetRenderTargetView();
	ID3D11DepthStencilView* EEGetDepthStencilView();
}

#endif
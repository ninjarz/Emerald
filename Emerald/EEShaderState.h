#pragma once
#ifndef _EE_SHADERSTATE_H_
#define _EE_SHADERSTATE_H_

#include <d3d11.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEShaderState
	//----------------------------------------------------------------------------------------------------
	class EEShaderState
	{
	public:
		EEShaderState();
		EEShaderState(const EEShaderState& _shaderState);
		~EEShaderState();

		bool Initialize();
		void Shutdown();

		bool SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _desc);
		bool SetRasterizerState(const D3D11_RASTERIZER_DESC& _desc);
		bool SetSamplerState(const D3D11_SAMPLER_DESC& _desc);
		bool SetBlendState(const D3D11_BLEND_DESC& _desc);

	private:
		D3D11_DEPTH_STENCIL_DESC m_depthStencilDesc;
		ID3D11DepthStencilState *m_depthStencilState;
		D3D11_RASTERIZER_DESC m_rasterizerDesc;
		ID3D11RasterizerState *m_rasterizerState;
		D3D11_SAMPLER_DESC m_samplerDesc;
		ID3D11SamplerState *m_samplerState;
		float m_blendFactor[4];
		D3D11_BLEND_DESC m_blendDesc;
		ID3D11BlendState *m_blendState;
	};

	//EEShaderState_APIs
	//----------------------------------------------------------------------------------------------------
	bool EESetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _desc);
	bool EESetRasterizerState(const D3D11_RASTERIZER_DESC& _desc);
	bool EESetSamplerState(const D3D11_SAMPLER_DESC& _desc);
	bool EESetBlendState(const D3D11_BLEND_DESC& _desc);
}


#endif
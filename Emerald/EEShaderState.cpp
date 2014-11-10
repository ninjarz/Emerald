#include "EEShaderState.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEShaderState
	//----------------------------------------------------------------------------------------------------
	EEShaderState::EEShaderState()
		:
		m_depthStencilState(NULL),
		m_rasterizerState(NULL),
		m_samplerState(NULL),
		m_blendState(NULL)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEShaderState::EEShaderState(const EEShaderState& _shaderState)
		:
		m_depthStencilDesc(_shaderState.m_depthStencilDesc),
		m_depthStencilState(_shaderState.m_depthStencilState),
		m_rasterizerDesc(_shaderState.m_rasterizerDesc),
		m_rasterizerState(_shaderState.m_rasterizerState),
		m_samplerDesc(_shaderState.m_samplerDesc),
		m_samplerState(_shaderState.m_samplerState),
		m_blendDesc(_shaderState.m_blendDesc),
		m_blendState(_shaderState.m_blendState)
	{
	}

	//----------------------------------------------------------------------------------------------------
	EEShaderState::~EEShaderState()
	{
		Shutdown();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEShaderState::Initialize()
	{
		ID3D11Device *device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();

		ZeroMemory(&m_depthStencilDesc, sizeof(m_depthStencilDesc));
		m_depthStencilDesc.DepthEnable = true;
		m_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		m_depthStencilDesc.StencilEnable = false;
		m_depthStencilDesc.StencilReadMask = 0xFF;
		m_depthStencilDesc.StencilWriteMask = 0xFF;
		m_depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		m_depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		m_depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		device->CreateDepthStencilState(&m_depthStencilDesc, &m_depthStencilState);
		deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		ZeroMemory(&m_rasterizerDesc, sizeof(m_rasterizerDesc));
		m_rasterizerDesc.AntialiasedLineEnable = false;
		m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
		m_rasterizerDesc.DepthBias = 0;
		m_rasterizerDesc.DepthBiasClamp = 0.0f;
		m_rasterizerDesc.DepthClipEnable = true;
		m_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		m_rasterizerDesc.FrontCounterClockwise = false;
		m_rasterizerDesc.MultisampleEnable = false;
		m_rasterizerDesc.ScissorEnable = false;
		m_rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		device->CreateRasterizerState(&m_rasterizerDesc, &m_rasterizerState);
		deviceContext->RSSetState(m_rasterizerState);

		ZeroMemory(&m_samplerDesc, sizeof(m_samplerDesc));
		m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		device->CreateSamplerState(&m_samplerDesc, &m_samplerState);
		deviceContext->PSSetSamplers(0, 1, &m_samplerState);

		float m_blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		ZeroMemory(&m_blendDesc, sizeof(m_blendDesc));
		m_blendDesc.AlphaToCoverageEnable = false;
		m_blendDesc.IndependentBlendEnable = false;
		m_blendDesc.RenderTarget[0].BlendEnable = true;
		m_blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		m_blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		m_blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		m_blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		m_blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		device->CreateBlendState(&m_blendDesc, &m_blendState);
		deviceContext->OMSetBlendState(m_blendState, m_blendFactor, 0xffffffff);
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEShaderState::Shutdown()
	{
		SAFE_RELEASE(m_depthStencilState);
		SAFE_RELEASE(m_rasterizerState);
		SAFE_RELEASE(m_samplerState);
		SAFE_RELEASE(m_blendState);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEShaderState::SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _desc)
	{
		SAFE_RELEASE(m_depthStencilState);
		ID3D11Device *device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();

		m_depthStencilDesc = _desc;
		device->CreateDepthStencilState(&m_depthStencilDesc, &m_depthStencilState);
		deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEShaderState::SetRasterizerState(const D3D11_RASTERIZER_DESC& _desc)
	{
		SAFE_RELEASE(m_rasterizerState);
		ID3D11Device *device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();

		m_rasterizerDesc = _desc;
		device->CreateRasterizerState(&m_rasterizerDesc, &m_rasterizerState);
		deviceContext->RSSetState(m_rasterizerState);
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEShaderState::SetSamplerState(const D3D11_SAMPLER_DESC& _desc)
	{
		SAFE_RELEASE(m_samplerState);
		ID3D11Device *device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();

		m_samplerDesc = _desc;
		device->CreateSamplerState(&m_samplerDesc, &m_samplerState);
		deviceContext->PSSetSamplers(0, 1, &m_samplerState);
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEShaderState::SetBlendState(const D3D11_BLEND_DESC& _desc)
	{
		SAFE_RELEASE(m_blendState);
		ID3D11Device *device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();

		m_blendDesc = _desc;
		device->CreateBlendState(&m_blendDesc, &m_blendState);
		deviceContext->OMSetBlendState(m_blendState, m_blendFactor, 0xffffffff);
		return true;
	}

	//EEShaderState_APIs
	//----------------------------------------------------------------------------------------------------
	bool EESetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _desc)
	{
		return EECore::s_EECore->GetEEShaderState()->SetDepthStencilState(_desc);
	}

	//----------------------------------------------------------------------------------------------------
	bool EESetRasterizerState(const D3D11_RASTERIZER_DESC& _desc)
	{
		return EECore::s_EECore->GetEEShaderState()->SetRasterizerState(_desc);
	}

	//----------------------------------------------------------------------------------------------------
	bool EESetSamplerState(const D3D11_SAMPLER_DESC& _desc)
	{
		return EECore::s_EECore->GetEEShaderState()->SetSamplerState(_desc);
	}

	//----------------------------------------------------------------------------------------------------
	bool EESetBlendState(const D3D11_BLEND_DESC& _desc)
	{
		return EECore::s_EECore->GetEEShaderState()->SetBlendState(_desc);
	}
}
#include "EETexture.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETexture
	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture()
		:
		m_texture(NULL)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(LPCWSTR _file)
		:
		m_texture(NULL)
	{
		D3DX11CreateShaderResourceViewFromFileW(EECore::s_EECore->GetDevice(), _file, 0, 0, &m_texture, 0);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(ID3D11ShaderResourceView* _texture)
		:
		m_texture(_texture)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(const EETexture& _texture)
		:
		m_texture(_texture.m_texture)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETexture::~EETexture()
	{
		SAFE_RELEASE(m_texture);
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::LoadTextureFromFile(LPCWSTR _file)
	{
		if (FAILED(D3DX11CreateShaderResourceViewFromFile(EECore::s_EECore->GetDevice(), _file, 0, 0, &m_texture, 0)))
			return false;
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(ID3D11ShaderResourceView* _texture)
	{
		SAFE_RELEASE(_texture);

		m_texture = _texture;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11ShaderResourceView* EETexture::GetTexture()
	{
		return m_texture;
	}

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool SaveTextureToFile(EETextureType _type, LPCTSTR _fileName, EETexture& _texture)
	{
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
		ID3D11Resource *texture = NULL;
		_texture.GetTexture()->GetResource(&texture);
		switch (_type)
		{
		case EE_BMP:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_BMP, _fileName)))
				return false;
			break;
		case EE_JPG:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_JPG, _fileName)))
				return false;
			break;
		case EE_PNG:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_PNG, _fileName)))
				return false;
			break;
		case EE_DDS:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_DDS, _fileName)))
				return false;
			break;
		case EE_TIFF:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_TIFF, _fileName)))
				return false;
			break;
		case EE_GIF:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_GIF, _fileName)))
				return false;
			break;
		case EE_WMP:
			if (FAILED(D3DX11SaveTextureToFileW(deviceContext, texture, D3DX11_IFF_WMP, _fileName)))
				return false;
			break;
		default:
			return false;
		}

		return true;
	}
}
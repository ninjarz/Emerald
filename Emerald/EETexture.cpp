#include "EETexture.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETexture
	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture()
		:
		EESmartPtr()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(LPCWSTR _file)
		:
		EESmartPtr()
	{
		D3DX11CreateShaderResourceViewFromFileW(EECore::s_EECore->GetDevice(), _file, 0, 0, &m_value, 0);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(const char* _file, unsigned int _width, unsigned int _height)
		:
		EESmartPtr()
	{
		D3DX11_IMAGE_LOAD_INFO info;
		info.Width = _width;
		info.Height = _height;
		info.Depth = 0;
		info.FirstMipLevel = 0;
		info.MipLevels = D3DX11_DEFAULT;
		info.Usage = D3D11_USAGE_DEFAULT;
		info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		info.CpuAccessFlags = D3D11_CPU_ACCESS_READ;
		info.MiscFlags = 0;
		info.Format;
		info.Filter;
		info.MipFilter;
		info.pSrcInfo;

		D3DX11CreateShaderResourceViewFromMemory(EECore::s_EECore->GetDevice(), _file, _width * _height, 0, 0, &m_value, 0);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(ID3D11ShaderResourceView* _texture)
		:
		EESmartPtr(_texture)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(const EETexture& _texture)
		:
		EESmartPtr(_texture)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETexture::~EETexture()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::LoadTextureFromFile(LPCWSTR _file)
	{
		ID3D11ShaderResourceView *tex;
		if (FAILED(D3DX11CreateShaderResourceViewFromFileW(EECore::s_EECore->GetDevice(), _file, 0, 0, &tex, 0)))
			return false;
		SetValue(tex);
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(ID3D11ShaderResourceView *_texture)
	{
		SetValue(_texture);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11ShaderResourceView* EETexture::GetTexture()
	{
		return m_value;
	}

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool SaveTextureToFile(EETexture& _texture, LPCWSTR _fileName, EETextureType _type)
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
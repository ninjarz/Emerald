#include "EETexture.h"
#include "EECore.h"
#include "DirectXTex/DirectXTex.h"
//#include "DirectXTex/DirectXTexP.h"
using namespace DirectX;

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
	EETexture::EETexture(wchar_t* _file)
		:
		EESmartPtr()
	{
		/*
		ID3D11Device *device = EECore::s_EECore->GetDevice();
		D3DX11_IMAGE_LOAD_INFO info;
		info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		info.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		ID3D11ShaderResourceView *texture = nullptr;
		D3DX11CreateShaderResourceViewFromFileW(device, _file, &info, 0, &texture, 0);
		m_value = new EETextureData(texture);
		*/

		SetTexture(_file);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(unsigned int _width, unsigned int _height)
		:
		EESmartPtr()
	{

		SetTexture(_width, _height);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(const unsigned char* _buffer, unsigned int _width, unsigned int _height)
		:
		EESmartPtr()
	{
		SetTexture(_buffer, _width, _height);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(EEBitmap& _bitmap)
		:
		EESmartPtr()
	{
		SetTexture(_bitmap.GetData(), _bitmap.GetWidth(), _bitmap.GetHeight());
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(ID3D11Resource* _resource)
		:
		EESmartPtr()
	{
		m_value = new EETextureData(_resource);
	}

	//----------------------------------------------------------------------------------------------------
	EETexture::EETexture(ID3D11ShaderResourceView* _texture)
		:
		EESmartPtr()
	{
		m_value = new EETextureData(_texture);
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
	bool EETexture::SetTexture(LPCWSTR _file)
	{
		WCHAR ext[_MAX_EXT];
		_wsplitpath_s(_file, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

		ScratchImage image;
		HRESULT hr;
		if (_wcsicmp(ext, L".dds") == 0)
		{
			hr = LoadFromDDSFile(_file, DDS_FLAGS_FORCE_RGB, nullptr, image);
		}
		else if (_wcsicmp(ext, L".tga") == 0)
		{
			hr = LoadFromTGAFile(_file, nullptr, image);
		}
		else
		{
			hr = LoadFromWICFile(_file, WIC_FLAGS_ALL_FRAMES | WIC_FLAGS_FORCE_RGB | WIC_FLAGS_IGNORE_SRGB, nullptr, image);
		}
		if (SUCCEEDED(hr))
		{
			ID3D11Device *device = EECore::s_EECore->GetDevice();
			ID3D11ShaderResourceView *texture = nullptr;
			hr = CreateShaderResourceViewEx(device,
				image.GetImages(), image.GetImageCount(), image.GetMetadata(),
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET, 0, 0, false,
				&texture);
			SetValue(new EETextureData(texture));

			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(unsigned int _width, unsigned int _height)
	{
		ID3D11Device *device = EECore::s_EECore->GetDevice();

		D3D11_TEXTURE2D_DESC tex2DDesc;
		ZeroMemory(&tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		tex2DDesc.Width = _width;
		tex2DDesc.Height = _height;
		tex2DDesc.MipLevels = 1;
		tex2DDesc.ArraySize = 1;
		//tex2DDesc.Format = DXGI_FORMAT_R32_UINT;
		tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex2DDesc.SampleDesc.Count = 1;
		tex2DDesc.SampleDesc.Quality = 0;
		tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
		tex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
		tex2DDesc.CPUAccessFlags = 0;
		tex2DDesc.MiscFlags = 0;

		ID3D11Texture2D *texture2D = nullptr;
		HRESULT result;
		if (!FAILED(result = device->CreateTexture2D(&tex2DDesc, NULL, &texture2D)))
		{
			ID3D11ShaderResourceView *texture = nullptr;
			if (!FAILED(device->CreateShaderResourceView(texture2D, NULL, &texture)))
			{
				SetValue(new EETextureData(texture));
				SAFE_RELEASE(texture2D);
				return true;
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(const unsigned char* _buffer, unsigned int _width, unsigned int _height)
	{
		ID3D11Device *device = EECore::s_EECore->GetDevice();

		D3D11_TEXTURE2D_DESC tex2DDesc;
		ZeroMemory(&tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		tex2DDesc.Width = _width;
		tex2DDesc.Height = _height;
		tex2DDesc.MipLevels = 1;
		tex2DDesc.ArraySize = 1;
		//tex2DDesc.Format = DXGI_FORMAT_R32_UINT;
		tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex2DDesc.SampleDesc.Count = 1;
		tex2DDesc.SampleDesc.Quality = 0;
		tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
		tex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
		tex2DDesc.CPUAccessFlags = 0;
		tex2DDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = _buffer;
		data.SysMemPitch = sizeof(unsigned int)* _width;
		data.SysMemSlicePitch = sizeof(unsigned int)* _width * _height;

		ID3D11Texture2D *texture2D = nullptr;
		HRESULT result;
		if (SUCCEEDED(result = device->CreateTexture2D(&tex2DDesc, &data, &texture2D)))
		{
			ID3D11ShaderResourceView *texture = nullptr;
			if (SUCCEEDED(device->CreateShaderResourceView(texture2D, NULL, &texture)))
			{
				m_value = new EETextureData(texture);
				SAFE_RELEASE(texture2D);
				return true;
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(unsigned int _width, unsigned int _height, DXGI_FORMAT _format)
	{
		ID3D11Device *device = EECore::s_EECore->GetDevice();

		D3D11_TEXTURE2D_DESC tex2DDesc;
		ZeroMemory(&tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		tex2DDesc.Width = _width;
		tex2DDesc.Height = _height;
		tex2DDesc.MipLevels = 1;
		tex2DDesc.ArraySize = 1;
		//tex2DDesc.Format = DXGI_FORMAT_R32_UINT;
		tex2DDesc.Format = _format;
		tex2DDesc.SampleDesc.Count = 1;
		tex2DDesc.SampleDesc.Quality = 0;
		tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
		tex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
		tex2DDesc.CPUAccessFlags = 0;
		tex2DDesc.MiscFlags = 0;

		ID3D11Texture2D *texture2D = nullptr;
		HRESULT result;
		if (!FAILED(result = device->CreateTexture2D(&tex2DDesc, NULL, &texture2D)))
		{
			ID3D11ShaderResourceView *texture = nullptr;
			if (!FAILED(device->CreateShaderResourceView(texture2D, NULL, &texture)))
			{
				SetValue(new EETextureData(texture));
				SAFE_RELEASE(texture2D);
				return true;
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(EEBitmap& _bitmap)
	{
		return SetTexture(_bitmap.GetData(), _bitmap.GetWidth(), _bitmap.GetHeight());
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(ID3D11Resource* _resource)
	{
		SetValue(new EETextureData(_resource));

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::SetTexture(ID3D11ShaderResourceView *_texture)
	{
		SetValue(new EETextureData(_texture));

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	int EETexture::GetWidth()
	{
		if (m_value)
			return m_value->width;

		return 0;
	}

	//----------------------------------------------------------------------------------------------------
	int EETexture::GetHeight()
	{
		if (m_value)
			return m_value->height;

		return 0;
	}

	//----------------------------------------------------------------------------------------------------
	int EETexture::GetNumber()
	{
		if (m_value)
			return m_value->number;

		return 0;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETexture::GetBitmap(EEBitmap& _bitmap, unsigned int _mipLevel)
	{
		if (m_value)
		{
			if (m_value->texture)
			{
				//I spent a lot of time on it...
				ID3D11Resource *src = NULL;
				m_value->texture->GetResource(&src);
				D3D11_TEXTURE2D_DESC tex2DDesc;
				((ID3D11Texture2D *)src)->GetDesc(&tex2DDesc);
				tex2DDesc.BindFlags = 0;
				tex2DDesc.Usage = D3D11_USAGE_STAGING;
				tex2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

				ID3D11Texture2D *cpuBuf = nullptr;
				if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateTexture2D(&tex2DDesc, NULL, &cpuBuf)))
				{
					EECore::s_EECore->GetDeviceContext()->CopyResource(cpuBuf, src);

					ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
					D3D11_MAPPED_SUBRESOURCE mappedResource;
					if (FAILED(deviceContext->Map(cpuBuf, _mipLevel, D3D11_MAP_READ, 0, &mappedResource)))
						return false;
					unsigned char* tmp = (unsigned char*)mappedResource.pData;
					//It have not been verified yet
					_bitmap.SetData(m_value->width, m_value->height, tmp, mappedResource.RowPitch);
					deviceContext->Unmap(cpuBuf, 0);
					SAFE_RELEASE(cpuBuf);
					return true;
				}
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11Resource* EETexture::GetResource()
	{
		if (m_value)
			return m_value->resource;

		return nullptr;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11ShaderResourceView* EETexture::GetTexture()
	{
		if (m_value)
			return m_value->texture;

		return nullptr;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11UnorderedAccessView* EETexture::GetTextureUAV()
	{
		if (m_value)
			return m_value->textureUAV;

		return nullptr;
	}

	//----------------------------------------------------------------------------------------------------
	ID3D11RenderTargetView* EETexture::GetTextureRTV()
	{
		if (m_value)
			return m_value->textureRTV;

		return nullptr;
	}

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool EESaveTextureToFile(EETexture& _texture, LPCWSTR _fileName, EETextureType _type)
	{
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
		ID3D11Resource *texture = NULL;
		_texture.GetTexture()->GetResource(&texture); // safety check
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

	EETexture EETextureCombine(EETexture* _texture, unsigned int _num)
	{
		if (!_num)
			return EETexture();

		ID3D11Device *device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();

		D3D11_TEXTURE2D_DESC firstTexDesc;
		((ID3D11Texture2D*)_texture[0].GetResource())->GetDesc(&firstTexDesc);

		D3D11_TEXTURE2D_DESC texArrayDesc;
		texArrayDesc.Width = firstTexDesc.Width;
		texArrayDesc.Height = firstTexDesc.Height;
		texArrayDesc.MipLevels = firstTexDesc.MipLevels;
		texArrayDesc.ArraySize = _num;
		texArrayDesc.Format = firstTexDesc.Format;
		texArrayDesc.SampleDesc.Count = 1;
		texArrayDesc.SampleDesc.Quality = 0;
		texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
		texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_RENDER_TARGET;
		texArrayDesc.CPUAccessFlags = 0;
		texArrayDesc.MiscFlags = 0;

		ID3D11Texture2D *texArray(nullptr);
		if (FAILED(device->CreateTexture2D(&texArrayDesc, 0, &texArray)))
			return EETexture();

		//For each texture
		for (UINT i = 0; i < _num; ++i)
		{
			ID3D11Texture2D *src = (ID3D11Texture2D*)_texture[i].GetResource();
			D3D11_TEXTURE2D_DESC subtexDesc;
			src->GetDesc(&subtexDesc);
			subtexDesc.BindFlags = 0;
			subtexDesc.Usage = D3D11_USAGE_STAGING;
			subtexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			ID3D11Texture2D *cpuBuf = nullptr;
			if (FAILED(EECore::s_EECore->GetDevice()->CreateTexture2D(&subtexDesc, NULL, &cpuBuf)))
				continue;
			deviceContext->CopyResource(cpuBuf, src);

			//For each mipmap level
			for (UINT j = 0; j < texArrayDesc.MipLevels; ++j)
			{
				D3D11_MAPPED_SUBRESOURCE mappedTex2D;
				if (FAILED(deviceContext->Map(cpuBuf, j, D3D11_MAP_READ, 0, &mappedTex2D)))
					break;
				//Resizing is needed... !!! If the size of the src is smaller than the first frame, there will be a choke.
				Image tmp;
				tmp.width = subtexDesc.Width;
				tmp.height = subtexDesc.Height;
				tmp.format = subtexDesc.Format;
				tmp.rowPitch = mappedTex2D.RowPitch;
				tmp.slicePitch = mappedTex2D.DepthPitch;
				tmp.pixels = (uint8_t*)mappedTex2D.pData;
				ScratchImage result;
				Resize(tmp, texArrayDesc.Width, texArrayDesc.Height, 0, result);

				//Add to the array
				deviceContext->UpdateSubresource(
					texArray,
					D3D11CalcSubresource(j, i, texArrayDesc.MipLevels),
					0, result.GetImages()->pixels, result.GetImages()->rowPitch, result.GetImages()->slicePitch);

				deviceContext->Unmap(cpuBuf, j);
			}
			SAFE_RELEASE(cpuBuf);
		}

		return EETexture(texArray);
	}
}
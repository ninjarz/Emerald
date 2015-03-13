#pragma once
#ifndef _EE_TEXTURE_H_
#define _EE_TEXTURE_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include "EESmartPtr.h"
#include "EEBitmap.h"
#include "EECore.h"

#include <iostream>
//It should be designed to be a smart pointer
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETextureType
	//----------------------------------------------------------------------------------------------------
	typedef enum EETextureType
	{
		EE_BMP = 1,
		EE_JPG = 2,
		EE_PNG = 3,
		EE_DDS = 4,
		EE_TIFF = 5,
		EE_GIF = 6,
		EE_WMP = 7,
	} EETextureType;

	//EETextureData
	//----------------------------------------------------------------------------------------------------
	class EETextureData
	{
	public:
		EETextureData() : texture(nullptr), textureUAV(nullptr), width(0), height(0) {}
		EETextureData(ID3D11Resource* _resource) : texture(nullptr), textureUAV(nullptr), width(0), height(0)
		{
			if (_resource)
			{
				D3D11_RESOURCE_DIMENSION resourceDimension;
				_resource->GetType(&resourceDimension);
				if (resourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				{
					D3D11_TEXTURE2D_DESC texture2DDesc;
					((ID3D11Texture2D*)_resource)->GetDesc(&texture2DDesc);

					D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
					SRVDesc.Format = texture2DDesc.Format;
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					SRVDesc.Texture2D.MipLevels = 1;
					SRVDesc.Texture2D.MostDetailedMip = 0;
					if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateShaderResourceView(_resource, &SRVDesc, &texture)))
					{
					}

					D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
					UAVDesc.Format = texture2DDesc.Format;
					UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
					UAVDesc.Texture2D.MipSlice = 0;
					if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateUnorderedAccessView(_resource, &UAVDesc, &textureUAV)))
					{
					}
				}
			}
		}
		EETextureData(ID3D11ShaderResourceView* _texture) : texture(_texture), textureUAV(nullptr), width(0), height(0)
		{
			if (_texture)
			{
				ID3D11Resource *resource;
				texture->GetResource(&resource);
				D3D11_RESOURCE_DIMENSION resourceDimension;
				resource->GetType(&resourceDimension);
				if (resourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				{
					D3D11_TEXTURE2D_DESC texture2DDesc;
					((ID3D11Texture2D*)resource)->GetDesc(&texture2DDesc);
					width = texture2DDesc.Width;
					height = texture2DDesc.Height;

					D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
					UAVDesc.Format = texture2DDesc.Format;
					UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
					UAVDesc.Texture2D.MipSlice = 0;
					if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateUnorderedAccessView(resource, &UAVDesc, &textureUAV)))
					{

					}
				}
			}
		}
		virtual ~EETextureData()
		{
			if (texture)
			{
				texture->Release();
				texture = nullptr;
			}
		}

	public:
		ID3D11ShaderResourceView *texture;
		ID3D11UnorderedAccessView *textureUAV;
		int width;
		int height;
	};

	//EETexture
	//----------------------------------------------------------------------------------------------------
	class EETexture : public EESmartPtr<EETextureData>
	{
	public:
		EETexture();
		EETexture(wchar_t* _file);
		EETexture(unsigned int _width, unsigned int _height);
		EETexture(const unsigned char* _buffer, unsigned int _width, unsigned int _height);
		EETexture(EEBitmap& _bitmap);
		EETexture(ID3D11Resource* _resource);
		EETexture(ID3D11ShaderResourceView* _texture);
		EETexture(const EETexture& _texture);
		virtual ~EETexture();
		
		bool LoadTextureFromFile(LPCWSTR _file);
		bool SetTexture(unsigned int _width, unsigned int _height);
		bool SetTexture(unsigned int _width, unsigned int _height, DXGI_FORMAT _format);
		bool SetTexture(ID3D11Resource* _resource);
		bool SetTexture(ID3D11ShaderResourceView* _texture);

		int GetWidth();
		int GetHeight();
		bool GetBitmap(EEBitmap& _bitmap);
		ID3D11ShaderResourceView* GetTexture();
		ID3D11UnorderedAccessView* GetTextureUAV();

	//private:
	//	ID3D11ShaderResourceView *m_texture;
	};

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool SaveTextureToFile(EETexture& _texture, LPCWSTR _fileName, EETextureType _type);
}

#endif
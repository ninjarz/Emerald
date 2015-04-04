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
		EETextureData() : resource(nullptr), texture(nullptr), textureUAV(nullptr), textureRTV(nullptr), width(0), height(0) {}
		EETextureData(ID3D11Resource* _resource) : resource(_resource), texture(nullptr), textureUAV(nullptr), textureRTV(nullptr), width(0), height(0)
		{
			if (resource)
			{
				D3D11_RESOURCE_DIMENSION resourceDimension;
				resource->GetType(&resourceDimension);
				if (resourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				{
					D3D11_TEXTURE2D_DESC texture2DDesc;
					((ID3D11Texture2D*)resource)->GetDesc(&texture2DDesc);
					width = texture2DDesc.Width;
					height = texture2DDesc.Height;
					number = texture2DDesc.ArraySize;

					//SRV
					if (FAILED(EECore::s_EECore->GetDevice()->CreateShaderResourceView(_resource, NULL, &texture)))
						return;

					//UAV
					if (FAILED(EECore::s_EECore->GetDevice()->CreateUnorderedAccessView(_resource, NULL, &textureUAV)))
						return;

					//RTV
					if (FAILED(EECore::s_EECore->GetDevice()->CreateRenderTargetView(_resource, NULL, &textureRTV)))
						return;
				}
			}
		}
		EETextureData(ID3D11ShaderResourceView* _texture) : resource(nullptr), texture(_texture), textureUAV(nullptr), textureRTV(nullptr), width(0), height(0)
		{
			if (texture)
			{
				texture->GetResource(&resource);
				D3D11_RESOURCE_DIMENSION resourceDimension;
				resource->GetType(&resourceDimension);
				if (resourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				{
					D3D11_TEXTURE2D_DESC texture2DDesc;
					((ID3D11Texture2D*)resource)->GetDesc(&texture2DDesc);
					width = texture2DDesc.Width;
					height = texture2DDesc.Height;
					number = texture2DDesc.ArraySize;

					//UAV
					if (FAILED(EECore::s_EECore->GetDevice()->CreateUnorderedAccessView(resource, NULL, &textureUAV)))
						return;

					//RTV
					if (FAILED(EECore::s_EECore->GetDevice()->CreateRenderTargetView(resource, NULL, &textureRTV)))
						return;
				}
			}
		}
		virtual ~EETextureData()
		{
			SAFE_RELEASE(resource);
			SAFE_RELEASE(texture);
			SAFE_RELEASE(textureUAV);
		}

	public:
		ID3D11Resource *resource;
		ID3D11ShaderResourceView *texture;
		ID3D11UnorderedAccessView *textureUAV;
		ID3D11RenderTargetView *textureRTV;
		int width;
		int height;
		int number;
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
		
		bool SetTexture(LPCWSTR _file);
		bool SetTexture(unsigned int _width, unsigned int _height);
		bool SetTexture(const unsigned char* _buffer, unsigned int _width, unsigned int _height);
		bool SetTexture(unsigned int _width, unsigned int _height, DXGI_FORMAT _format);
		bool SetTexture(ID3D11Resource* _resource);
		bool SetTexture(ID3D11ShaderResourceView* _texture);

		int GetWidth();
		int GetHeight();
		int GetNumber();
		bool GetBitmap(EEBitmap& _bitmap, unsigned int _mipLevel = 0);
		ID3D11Resource* GetResource();
		ID3D11ShaderResourceView* GetTexture();
		ID3D11UnorderedAccessView* GetTextureUAV();
		ID3D11RenderTargetView* GetTextureRTV();

	//private:
	//	ID3D11ShaderResourceView *m_texture;
	};

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool EESaveTextureToFile(EETexture& _texture, LPCWSTR _fileName, EETextureType _type);
	EETexture EETextureCombine(EETexture* _texture, unsigned int _num);
}

#endif
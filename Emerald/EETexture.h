#pragma once
#ifndef _EE_TEXTURE_H_
#define _EE_TEXTURE_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include "EESmartPtr.h"
#include "EEBitmap.h"

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
		EETextureData() : texture(nullptr), width(0), height(0) {}
		EETextureData(ID3D11ShaderResourceView* _texture) : texture(_texture), width(0), height(0)
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
		EETexture(const unsigned char* _buffer, unsigned int _width, unsigned int _height);
		EETexture(EEBitmap& _bitmap);
		EETexture(ID3D11ShaderResourceView* _texture);
		EETexture(const EETexture& _texture);
		virtual ~EETexture();
		
		bool LoadTextureFromFile(LPCWSTR _file);
		bool SetTexture(ID3D11ShaderResourceView* _texture);

		int GetWidth();
		int GetHeight();
		bool GetBitmap(EEBitmap& _bitmap);

		ID3D11ShaderResourceView* GetTexture();

	//private:
	//	ID3D11ShaderResourceView *m_texture;
	};

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool SaveTextureToFile(EETexture& _texture, LPCWSTR _fileName, EETextureType _type);
}

#endif
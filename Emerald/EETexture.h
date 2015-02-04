#pragma once
#ifndef _EE_TEXTURE_H_
#define _EE_TEXTURE_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include "EESmartPtr.h"

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
	class EETextureData
	{
	public:
		EETextureData() : texture(nullptr) {}
		EETextureData(ID3D11ShaderResourceView* _texture) : texture(_texture) {}
		~EETextureData()
		{
			if (texture)
			{
				texture->Release();
				texture = nullptr;
			}
		}

	public:
		ID3D11ShaderResourceView *texture;
	};

	//EETexture
	//----------------------------------------------------------------------------------------------------
	class EETexture : public EESmartPtr<EETextureData>
	{
	public:
		EETexture();
		EETexture(LPCWSTR _file);
		EETexture(const char* _file, unsigned int _width, unsigned int _height);
		EETexture(ID3D11ShaderResourceView* _texture);
		EETexture(const EETexture& _texture);
		~EETexture();
		
		bool LoadTextureFromFile(LPCWSTR _file);
		bool SetTexture(ID3D11ShaderResourceView* _texture);

		ID3D11ShaderResourceView* GetTexture();

	//private:
	//	ID3D11ShaderResourceView *m_texture;
	};

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool SaveTextureToFile(EETexture& _texture, LPCWSTR _fileName, EETextureType _type);
}

#endif
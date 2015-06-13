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

	// EETextureData
	//----------------------------------------------------------------------------------------------------
	class EETextureData
	{
	public:
		EETextureData();
		EETextureData(ID3D11Resource* _resource);
		EETextureData(ID3D11ShaderResourceView* _texture);
		virtual ~EETextureData();

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

		virtual EETexture Clone();
		
		bool SetTexture(LPCWSTR _file);
		bool SetTexture(unsigned int _width, unsigned int _height);
		bool SetTexture(const unsigned char* _buffer, unsigned int _width, unsigned int _height);
		bool SetTexture(unsigned int _width, unsigned int _height, DXGI_FORMAT _format);
		bool SetTexture(EEBitmap& _bitmap);
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
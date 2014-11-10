#pragma once
#ifndef _EE_TEXTURE_H_
#define _EE_TEXTURE_H_

#include <d3d11.h>
#include <d3dx11async.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETextureType
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


	//EETexture
	//----------------------------------------------------------------------------------------------------
	class EETexture
	{
	public:
		EETexture();
		EETexture(LPCWSTR _file);
		EETexture(ID3D11ShaderResourceView* _texture);
		EETexture(const EETexture& _texture);
		~EETexture();
		
		bool LoadTextureFromFile(LPCWSTR _file);

		bool SetTexture(ID3D11ShaderResourceView* _texture);

		ID3D11ShaderResourceView* GetTexture();

	private:
		ID3D11ShaderResourceView *m_texture;
	};

	//EETexture_APIs
	//----------------------------------------------------------------------------------------------------
	bool SaveTextureToFile(EETextureType _type, LPCTSTR _fileName, EETexture& _texture);
}

#endif
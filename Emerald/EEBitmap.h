#pragma once
#ifndef _EE_BITMAP_H_
#define _EE_BITMAP_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include <vector>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBitmap
	//----------------------------------------------------------------------------------------------------
	class EEBitmap
	{
	public:
		EEBitmap();
		EEBitmap(wchar_t* _file);
		EEBitmap(unsigned int _width, unsigned int _height);
		EEBitmap(unsigned int _width, unsigned int _height, const unsigned char* _buffer);
		EEBitmap(unsigned int _width, unsigned int _height, const unsigned char* _buffer, unsigned int _rowPitch);
		EEBitmap(const EEBitmap& _bitmap);
		EEBitmap(const EEBitmap&& _bitmap);
		virtual ~EEBitmap();

		void Resize(unsigned int _width, unsigned int _height);

		unsigned char* GetData();
		int GetDataSize();
		int GetWidth() const;
		int GetHeight() const;
		bool IsEmpty() const;
		EEBitmap GetSubmap(int _x, int _y, int _width = -1, int _height = -1) const;

		bool SetData(unsigned int _width, unsigned int _height, const unsigned char* _buffer);
		bool SetData(unsigned int _width, unsigned int _height, const unsigned char* _buffer, unsigned int _rowPitch);
		bool PutData(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, EEBitmap& _src);

	protected:
		std::vector<unsigned char> m_data;
		int m_width;
		int m_height;
	};

	// EEBitmap_APIs
	// Rvalue reference
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(EEBitmap& _top, EEBitmap& _bottom);
	EEBitmap EEBitmapCombineHorizontal(EEBitmap* _bitmaps, int _count);
	EEBitmap EEBitmapCombineHorizontal(std::vector<EEBitmap>& _bitmaps);
	EEBitmap EEBitmapCombineVertical(EEBitmap& _top, EEBitmap& _bottom);
	void EEBitmapDivideHorizontal(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps);
	void EEBitmapDivideVertical(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps);
	void EEMerge(std::vector<EEBitmap>& _bitmaps, EEBitmap& _result);
}

#endif
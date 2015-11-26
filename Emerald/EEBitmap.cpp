#include "EEBitmap.h"
#include "EECore.h"
#include "DirectXTex/DirectXTex.h"

using namespace DirectX;


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBitmap
	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap()
		:
		m_data(),
		m_width(0),
		m_height(0)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(wchar_t* _file)
		:
		m_data(),
		m_width(0),
		m_height(0)
	{
		SetData(_file);
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(unsigned int _width, unsigned int _height)
		:
		m_data(_width * _height << 2, (unsigned char)0),
		m_width(_width),
		m_height(_height)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(unsigned int _width, unsigned int _height, const unsigned char* _buffer)
		:
		m_data(_buffer, _buffer + (_width * _height << 2)),
		m_width(_width),
		m_height(_height)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(unsigned int _width, unsigned int _height, const unsigned char* _buffer, unsigned int _rowPitch)
		:
		m_data(),
		m_width(_width),
		m_height(_height)
	{
		SetData(_width, _height, _buffer, _rowPitch);
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(const EEBitmap& _bitmap)
		:
		m_data(_bitmap.m_data),
		m_width(_bitmap.m_width),
		m_height(_bitmap.m_height)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(const EEBitmap&& _bitmap)
		:
		m_data(_bitmap.m_data),
		m_width(_bitmap.m_width),
		m_height(_bitmap.m_height)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::~EEBitmap()
	{

	}

	//----------------------------------------------------------------------------------------------------
	void EEBitmap::Resize(unsigned int _width, unsigned int _height)
	{
		std::vector<unsigned char> tmp(m_data);
		int width = m_width;
		int height = m_height;

		m_data.clear();
		m_data.resize(_width * _height << 2, (unsigned char)0);
		m_width = _width;
		m_height = _height;

		int minWidth = width > m_width ? m_width : width;
		int minHeight = height > m_height ? m_height : height;
		unsigned int *dst = (unsigned int*)m_data.data();
		unsigned int *src = (unsigned int*)tmp.data();
		for (int i = 0; i < minHeight; ++i)
		{
			memcpy(&dst[m_width * i], &src[width * i], minWidth << 2);
		}
	}

	//----------------------------------------------------------------------------------------------------
	unsigned char* EEBitmap::GetData()
	{
		return m_data.data();
	}

	//----------------------------------------------------------------------------------------------------
	int EEBitmap::GetDataSize()
	{
		return m_data.size();
	}

	//----------------------------------------------------------------------------------------------------
	int EEBitmap::GetWidth() const
	{
		return m_width;
	}

	//----------------------------------------------------------------------------------------------------
	int EEBitmap::GetHeight() const
	{
		return m_height;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBitmap::IsEmpty() const
	{
		return m_data.empty();
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmap::GetSubmap(int _x, int _y, int _width, int _height) const
	{
		if (_x > m_width || _y > m_height)
			return EEBitmap();
		unsigned int width(0);
		if (_width <= 0)
		{
			if (!_width)
				return EEBitmap();
			else
				width = m_width - _x;
		}
		else if (_x + _width > m_width)
		{
			width = m_width - _x;
		}
		unsigned int height(0);
		if (_height <= 0)
		{
			if (!_height)
				return EEBitmap();
			else
				height = m_height - _y;
		}
		else if (_y + _height > m_height)
		{
			height = m_height - _y;
		}

		unsigned int *src = (unsigned int*)m_data.data();
		EEBitmap submap(width, height);
		unsigned int *buffer = (unsigned int*)submap.GetData();
		unsigned int z(_x + width);
		unsigned int w(_y + height);
		unsigned int dstIndex(0);
		for (unsigned int i = _y; i < w; ++i)
		{
			for (unsigned int j = _x; j < z; ++j)
			{
				buffer[dstIndex++] = src[i * m_width + j];
			}
		}
		return submap;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBitmap::SetData(const wchar_t* _file)
	{
		wchar_t ext[_MAX_EXT];
		_wsplitpath_s(_file, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

		ScratchImage scratchImage;
		HRESULT hr;
		if (_wcsicmp(ext, L".dds") == 0)
		{
			hr = LoadFromDDSFile(_file, DDS_FLAGS_FORCE_RGB, nullptr, scratchImage);
		}
		else if (_wcsicmp(ext, L".tga") == 0)
		{
			hr = LoadFromTGAFile(_file, nullptr, scratchImage);
		}
		else
		{
			hr = LoadFromWICFile(_file, WIC_FLAGS_ALL_FRAMES | WIC_FLAGS_FORCE_RGB | WIC_FLAGS_IGNORE_SRGB, nullptr, scratchImage);
		}
		if (SUCCEEDED(hr))
		{
			if (scratchImage.GetImageCount())
			{
				// First frame
				const Image *image = scratchImage.GetImages();

				if (image->format == DXGI_FORMAT_R8G8B8A8_UNORM) // tmp
				{
					return SetData(image->width, image->height, image->pixels, image->rowPitch);
				}
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBitmap::SetData(unsigned int _width, unsigned int _height, const unsigned char* _buffer)
	{
		m_data.clear();
		m_data.assign(_buffer, _buffer + (_width * _height << 2));
		m_width = _width;
		m_height = _height;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBitmap::SetData(unsigned int _width, unsigned int _height, const unsigned char* _buffer, unsigned int _rowPitch)
	{
		m_data.clear();
		m_data.resize(_width * _height << 2);
		m_width = _width;
		m_height = _height;

		unsigned char* dst = m_data.data();
		unsigned int rowSpan = _width << 2;
		for (int i = 0; i < m_height; ++i)
		{
			memcpy(dst, _buffer, rowSpan);
			dst += rowSpan;
			_buffer += _rowPitch;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBitmap::SetData(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, EEBitmap& _src)
	{
		if (_x + _width > (unsigned int)m_width)
			_width = (unsigned int)m_width - _x;
		if (_width > (unsigned int)_src.GetWidth())
			_width = (unsigned int)_src.GetWidth();
		if (_y + _height > (unsigned int)m_height)
			_height = (unsigned int)m_height - _y;
		if (_height > (unsigned int)_src.GetHeight())
			_height = (unsigned int)_src.GetHeight();

		unsigned int* src = ((unsigned int*)_src.GetData());
		unsigned int* dst = ((unsigned int*)m_data.data()) + _y * m_width + _x;
		for (unsigned int i = _y; i < _y + _height; ++i)
		{
			memcpy(dst, src, _width << 2);
			dst += m_width;
			src += _src.GetWidth();
		}

		return true;
	}

	//EEBitmap_APIs
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(EEBitmap& _left, EEBitmap& _right)
	{
		unsigned int* srcL = (unsigned int*)_left.GetData();
		unsigned int* srcR = (unsigned int*)_right.GetData();
		int widthL = _left.GetWidth();
		int widthR = _right.GetWidth();
		int heightL = _left.GetHeight();
		int heightR = _right.GetHeight();

		int width = widthL + widthR;
		if (heightL >= heightR)
		{
			int height = heightL;
			EEBitmap result(width, height);
			unsigned int *buffer = (unsigned int*)result.GetData();
			for (int i = 0; i < heightR; ++i)
			{
				int dstIndex = i * width;
				memcpy(&buffer[dstIndex], &srcL[i * widthL], widthL << 2);
				memcpy(&buffer[dstIndex + widthL], &srcR[i * widthR], widthR << 2);
			}
			for (int i = heightR; i < heightL; ++i)
			{
				memcpy(&buffer[i * width], &srcL[i * widthL], widthL << 2);
			}
			return result;
		}
		else
		{
			int height = heightR;
			EEBitmap result(width, height);
			unsigned int *buffer = (unsigned int*)result.GetData();
			for (int i = 0; i < heightL; ++i)
			{
				int dstIndex = i * width;
				memcpy(&buffer[dstIndex], &srcL[i * widthL], widthL << 2);
				memcpy(&buffer[dstIndex + widthL], &srcR[i * widthR], widthR << 2);
			}
			for (int i = heightL; i < heightR; ++i)
			{
				memcpy(&buffer[i * width + widthL], &srcR[i * widthR], widthR << 2);
			}
			return result;
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(EEBitmap* _bitmaps, int _count)
	{
		return EEBitmap();
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(std::vector<EEBitmap>& _bitmaps)
	{
		int width(0), height(0);
		for (EEBitmap& bitmap : _bitmaps)
		{
			width += bitmap.GetWidth();
			if (bitmap.GetHeight() > height)
				height = bitmap.GetHeight();
		}
		EEBitmap result(width, height);
		unsigned int *dst = (unsigned int*)result.GetData();
		for (int i = 0; i < height; ++i)
		{
			int tmpWidth(0);
			for (EEBitmap& bitmap : _bitmaps)
			{
				if (bitmap.GetHeight() > i)
				{
					memcpy(&dst[width * i + tmpWidth], &bitmap.GetData()[bitmap.GetWidth() * i << 2], bitmap.GetWidth() << 2);
				}
				tmpWidth += bitmap.GetWidth();
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineVertical(EEBitmap& _top, EEBitmap& _bottom)
	{
		unsigned int* srcT = (unsigned int*)_top.GetData();
		unsigned int* srcB = (unsigned int*)_bottom.GetData();
		int widthT = _top.GetWidth();
		int widthB = _bottom.GetWidth();
		int heightT = _top.GetHeight();
		int heightB = _bottom.GetHeight();

		int width = widthT >= widthB ? widthT : widthB;
		int height = heightT + heightB;
		EEBitmap result(width, height);
		unsigned int *buffer = (unsigned int*)result.GetData();
		for (int i = 0; i < heightT; ++i)
		{
			memcpy(&buffer[i * width], &srcT[i * widthT], widthT << 2);
		}
		for (int i = heightT; i < height; ++i)
		{
			memcpy(&buffer[i * width], &srcB[(i - heightT) * widthB], widthB << 2);
		}
		return result;
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineVertical(std::vector<EEBitmap>& _bitmaps)
	{
		int width(0), height(0);
		for (EEBitmap& bitmap : _bitmaps)
		{
			if (bitmap.GetWidth() > width)
				width = bitmap.GetWidth();
			height += bitmap.GetHeight();
		}
		EEBitmap result(width, height);
		unsigned int *dst = (unsigned int*)result.GetData();
		int heightIndex(0);
		for (EEBitmap& bitmap : _bitmaps)
		{
			for (int i = 0; i < bitmap.GetHeight(); ++i)
			{
				memcpy(&dst[width * heightIndex++], &bitmap.GetData()[bitmap.GetWidth() * i << 2], bitmap.GetWidth() << 2);
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	void EEBitmapDivideHorizontal(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps)
	{
		if (!_amount || _bitmap.IsEmpty())
			return;
		_bitmaps.clear();

		unsigned int* src = (unsigned int*)_bitmap.GetData();
		int width = _bitmap.GetWidth();
		int height = _bitmap.GetHeight();
		int divideHeight = height % _amount ? height / _amount + 1 : height / _amount;
		for (unsigned int i = 0; i < _amount; ++i)
		{
			if (height >= divideHeight)
			{
				height -= divideHeight;
				_bitmaps.push_back(EEBitmap(width, divideHeight));
				memcpy(_bitmaps[i].GetData(), &src[width * i * divideHeight], _bitmaps[i].GetDataSize());
			}
			else if (height > 0)
			{
				_bitmaps.push_back(EEBitmap(width, height));
				memcpy(_bitmaps[i].GetData(), &src[width * i * height], _bitmaps[i].GetDataSize());
				return;
			}
			else
				return;
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EEBitmapDivideVertical(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps)
	{
		if (!_amount || _bitmap.IsEmpty())
			return;
		_bitmaps.clear();

		unsigned int* src = (unsigned int*)_bitmap.GetData();
		int width = _bitmap.GetWidth();
		int divideWidth = width % _amount ? width / _amount + 1 : width / _amount;
		int height = _bitmap.GetHeight();
		for (unsigned int i = 0; i < _amount; ++i)
		{
			if (width >= divideWidth)
			{
				width -= divideWidth;
				_bitmaps.push_back(EEBitmap(divideWidth, height));
				unsigned int *buffer = (unsigned int*)_bitmaps[i].GetData();
				int srcIndex = i * divideWidth;
				for (int j = 0; j < height; ++j)
				{
					memcpy(&buffer[j * divideWidth], &src[_bitmap.GetWidth() * j + srcIndex], divideWidth << 2);
				}
			}
			else if (width > 0)
			{
				_bitmaps.push_back(EEBitmap(width, height));
				unsigned int *buffer = (unsigned int*)_bitmaps[i].GetData();
				int srcIndex = i * divideWidth;
				for (int j = 0; j < height; ++j)
				{
					memcpy(&buffer[j * width], &src[_bitmap.GetWidth() * j + srcIndex], width << 2);
				}
				break;
			}
			else
				break;
		}
	}

	// all the _bitmaps must have the same size
	//----------------------------------------------------------------------------------------------------
	void EEMerge(std::vector<EEBitmap>& _bitmaps, EEBitmap& _result)
	{
		if (_bitmaps.empty())
			return;

		for (EEBitmap& _bitmap : _bitmaps)
		{

		}
	}
}
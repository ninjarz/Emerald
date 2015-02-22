#include "EEBitmap.h"


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
	EEBitmap::EEBitmap(const unsigned char* _buffer, unsigned int _width, unsigned int _height)
		:
		m_data(_buffer, _buffer + _width * _height * 4),
		m_width(_width),
		m_height(_height)
	{

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
	EEBitmap::~EEBitmap()
	{

	}

	//----------------------------------------------------------------------------------------------------
	unsigned char* EEBitmap::GetData()
	{
		return m_data.data();
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
		unsigned int* src = (unsigned int*)m_data.data();
		std::vector<unsigned int> buffer(width * height, (unsigned int)0);
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
		return EEBitmap((unsigned char*)buffer.data(), width, height);
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
			std::vector<unsigned int> buffer(width * height, (unsigned int)0);
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
			return EEBitmap((unsigned char*)buffer.data(), width, height);
		}
		else
		{
			int height = heightR;
			std::vector<unsigned int> buffer(width * height, (unsigned int)0);
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
			return EEBitmap((unsigned char*)buffer.data(), width, height);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(EEBitmap* _bitmaps)
	{
		return EEBitmap();
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
		std::vector<unsigned int> buffer(width * height, (unsigned int)0);
		for (int i = 0; i < heightT; ++i)
		{
			memcpy(&buffer[i * width], &srcT[i * widthT], widthT << 2);
		}
		for (int i = heightT; i < height; ++i)
		{
			memcpy(&buffer[i * width], &srcB[(i - heightT) * widthB], widthB << 2);
		}
		return EEBitmap((unsigned char*)buffer.data(), width, height);
	}

	//----------------------------------------------------------------------------------------------------
	void EEBitmapDivideHorizontal(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps)
	{
		if (!_amount)
			return;
		_bitmaps.clear();

		unsigned int* src = (unsigned int*)_bitmap.GetData();
		int height = _bitmap.GetHeight();
		int divideHeight = height / _amount;
		for (unsigned int i = 0; i < _amount; ++i)
		{
			if (height >= divideHeight)
			{
				height -= divideHeight;
				std::vector<unsigned int> buffer(_bitmap.GetWidth() * divideHeight, (unsigned int)0);
				memcpy(&buffer[_bitmap.GetWidth() * i * divideHeight], buffer.data(), buffer.size() << 2);
				_bitmaps.push_back(EEBitmap((unsigned char*)buffer.data(), _bitmap.GetWidth(), divideHeight));
			}
			else if (height > 0)
			{
				std::vector<unsigned int> buffer(_bitmap.GetWidth() * height, (unsigned int)0);
				memcpy(&buffer[_bitmap.GetWidth() * i * divideHeight], buffer.data(), buffer.size() << 2);
				_bitmaps.push_back(EEBitmap((unsigned char*)buffer.data(), _bitmap.GetWidth(), height));
				return;
			}
			else
				return;
		}
		
	}
}
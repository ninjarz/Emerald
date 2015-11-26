#include "EECharacterGenerator.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EECharacterGenerator
	//----------------------------------------------------------------------------------------------------
	EECharacterGenerator::EECharacterGenerator()
	{
		Initialize();
	}

	//----------------------------------------------------------------------------------------------------
	EECharacterGenerator::~EECharacterGenerator()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EECharacterGenerator::Generate(EEBitmap& _bitmap, std::vector<std::string>& _result)
	{
		_result.clear();

		unsigned char *data = _bitmap.GetData();
		for (int i = 0; i < _bitmap.GetHeight() * _bitmap.GetWidth() << 2; i += _bitmap.GetWidth() << 2)
		{
			std::string rowData;
			for (int j = 0; j < _bitmap.GetWidth() << 2; j += 4)
			{
				float gray = data[i + j] * 0.299f + data[i + j + 1] * 0.587f + data[i + j + 2] * 0.114f;
				char character = ' ';
				characterTree.FindData(gray, gray, character);
				rowData.push_back(character);
			}

			_result.push_back(rowData);
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECharacterGenerator::Generate(EEBitmap& _bitmap, std::string& _result)
	{
		_result.clear();

		unsigned char *data = _bitmap.GetData();
		for (int i = 0; i < _bitmap.GetHeight() * _bitmap.GetWidth() << 2; i += _bitmap.GetWidth() << 2)
		{
			std::string rowData;
			for (int j = 0; j < _bitmap.GetWidth() << 2; j += 4)
			{
				float gray = data[i + j] * 0.299f + data[i + j + 1] * 0.587f + data[i + j + 2] * 0.114f;
				char character = ' ';
				characterTree.FindData(gray, gray, character);
				rowData.push_back(character);
			}

			rowData += '\n';
			_result.append(rowData);
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECharacterGenerator::Initialize()
	{
		characterTree.Insert(0.f, 20.f, '#');
		characterTree.Insert(20.f, 50.f, '#');
		characterTree.Insert(50.f, 70.f, '&');
		characterTree.Insert(70.f, 80.f, '$');
		characterTree.Insert(80.f, 110.f, '*');
		characterTree.Insert(110.f, 150.f, '+');
		characterTree.Insert(150.f, 180.f, '~');
		characterTree.Insert(180.f, 200.f, '^');
		characterTree.Insert(200.f, 220.f, '-');
		characterTree.Insert(220.f, 240.f, '`');
		characterTree.Insert(240.f, 256.f, ' ');

		return true;
	}
}
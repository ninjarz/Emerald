#pragma once
#ifndef _EE_CHARACTERGENERATOR_H_
#define _EE_CHARACTERGENERATOR_H_

#include <vector>
#include <string>
#include <fstream>
#include "EEBitmap.h"
#include "EEIntervalTree.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	class EECharacterGenerator
	{
	public:
		EECharacterGenerator();
		~EECharacterGenerator();

		bool Generate(EEBitmap& _bitmap, std::vector<std::string>& _result);
		bool Generate(EEBitmap& _bitmap, std::string& _result);

	protected:
		bool Initialize();

	protected:
		EEIntervalTree<char> characterTree;
	};
}



#endif
#pragma once
#ifndef _EE_CHARACTERGENERATOR_H_
#define _EE_CHARACTERGENERATOR_H_

#include <vector>
#include <string>
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

		bool Generate(const EEBitmap& _bitmap, std::vector<std::string>& _result);

	protected:
		// Interval Tree
	};
}



#endif
#pragma once
#ifndef _EE_SORT_HH_
#define _EE_SORT_HH_

#include <vector>
#include "EEObject.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESort_APIs
	//----------------------------------------------------------------------------------------------------
	void EEBubbleSort(std::vector<EEObject*>& _objects, bool _isInverse = false);
	void EEInsertSort(std::vector<EEObject*>& _objects, EEObject* _object, bool _isInverse = false);
}

#endif
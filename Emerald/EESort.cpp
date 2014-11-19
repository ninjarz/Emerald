#include "EESort.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESort_APIs
	//----------------------------------------------------------------------------------------------------
	void EEBubbleSort(std::vector<EEObject*>& _objects, bool _isInverse)
	{
		if (_isInverse)
		for (unsigned int i = 0; i < _objects.size() - 1; ++i)
		for (unsigned int j = 0; j < _objects.size() - i - 1; ++j)
		{
			if (*_objects[j] < *_objects[j + 1])
			{
				EEObject* tmp = _objects[j];
				_objects[j] = _objects[j + 1];
				_objects[j + 1] = tmp;
			}
		}
		else
		for (unsigned int i = 0; i < _objects.size() - 1; ++i)
		for (unsigned int j = 0; j < _objects.size() - i - 1; ++j)
		{
			if (_objects[j] > _objects[j + 1])
			{
				EEObject* tmp = _objects[j];
				_objects[j] = _objects[j + 1];
				_objects[j + 1] = tmp;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EEInsertSort(std::vector<EEObject*>& _objects, EEObject* _object, bool _isInverse)
	{
		unsigned int i = 0;
		if (_isInverse)
		for (; i < _objects.size(); ++i)
		{
			if (*_objects[i] > *_object)
				++i;
			else
				break;
		}
		else
		for (; i < _objects.size(); ++i)
		{
			if (*_objects[i] < *_object)
				++i;
			else
				break;
		}

		_objects.insert(_objects.begin() + i, _object);
	}

}
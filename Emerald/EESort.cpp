#include "EESort.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EESort_APIs
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
		auto it = _objects.begin();
		if (_isInverse)
		for (; it != _objects.end(); ++it)
		{
			if (**it > *_object)
				continue;
			else
				break;
		}
		else
		for (; it != _objects.end(); ++it)
		{
			if (**it < *_object)
				continue;
			else
				break;
		}

		_objects.insert(it, _object);
	}

}
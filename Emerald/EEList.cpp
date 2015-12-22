#include "EEList.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEList
	//----------------------------------------------------------------------------------------------------
	EEList::EEList()
		:
		EEObject()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEList::EEList(const EEList &_list)
		:
		EEObject(_list),
		m_objects(m_objects)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEList::~EEList()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEList::Update()
	{
		if (!EEObject::Update())
			return false;

		for (UINT i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i].anchor->Update();
			m_objects[i].content->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEList::Render()
	{
		if (!EEObject::Render())
			return false;

		for (UINT i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i].anchor->Render();
			m_objects[i].content->Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEList::AddObject(EEObject* _object)
	{
		if (_object)
		{
			if (m_objects.size())
			{
				EEObject *lastAnchor = m_objects.back().anchor;
				FLOAT3 position = lastAnchor->GetPosition();
				position.y += lastAnchor->GetOffsetHeight();

				EEObject *anchor = new EEObject(position);
				anchor->SetParent(this);
				_object->SetParent(anchor);
				m_objects.push_back(EEListItem(anchor, _object));
			}
			else
			{
				EEObject *anchor = new EEObject();
				anchor->SetParent(this);
				_object->SetParent(anchor);
				m_objects.push_back(EEListItem(anchor, _object));
			}
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEList::AddObjectAt(EEObject* _object, unsigned int _index)
	{
		if (_object)
		{

			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEList::RemoveObject(EEObject* _object)
	{
		if (_object)
		{

			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEList::RemoveObjectAt(unsigned int _index)
	{
		if (0 <= _index && _index < m_objects.size())
		{

			return true;
		}

		return false;
	}
}

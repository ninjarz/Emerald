#include "EEListView.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEListView
	//----------------------------------------------------------------------------------------------------
	EEListView::EEListView()
		:
		EEScrollView()
	{
		SetContent(&m_list);
	}

	//----------------------------------------------------------------------------------------------------
	EEListView::EEListView(const Rect_Float& _area)
		:
		EEScrollView(_area)
	{
		SetContent(&m_list);
	}

	//----------------------------------------------------------------------------------------------------
	EEListView::EEListView(const EEListView& _listView)
		:
		EEScrollView(_listView)
	{
	}

	//----------------------------------------------------------------------------------------------------
	EEListView::~EEListView()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::Update()
	{
		if (!EEScrollView::Update())
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::Render()
	{
		if (!EEScrollView::Render())
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::Process()
	{
		return EEObject::Process();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::AddObject(EEObject* _object)
	{
		if (m_list.AddObject(_object))
		{
			Repull();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::AddObjectAt(EEObject* _object, int _index)
	{
		if (m_list.AddObjectAt(_object, _index))
		{
			Repull();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::RemoveObject(EEObject* _object)
	{
		if (m_list.RemoveObject(_object))
		{
			Repull();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::RemoveObjectAt(int _index)
	{
		if (m_list.RemoveObjectAt(_index))
		{
			Repull();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEListView::SetContent(EEObject* _content)
	{
		return EEScrollView::SetContent(_content);
	}
}
#include "EEScrollView.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEScrollView
	//----------------------------------------------------------------------------------------------------
	EEScrollView::EEScrollView(const Rect_Float& _area)
		:
		EEQuad2D(_area),
		m_offsetHeight(0),
		m_verticalScrollHeight(0),
		m_verticalScrollCapacity(0),
		m_verticalScrollPos(0),
		m_verticalPos(0)
	{
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEScrollView::EEScrollView(const EEScrollView& _scrollView)
		:
		EEQuad2D(_scrollView),
		m_offsetHeight(_scrollView.m_offsetHeight),
		m_verticalScrollHeight(_scrollView.m_verticalScrollHeight),
		m_verticalScrollCapacity(_scrollView.m_verticalScrollCapacity),
		m_verticalScrollPos(_scrollView.m_verticalScrollPos),
		m_verticalPos(_scrollView.m_verticalPos)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEScrollView::~EEScrollView()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Render()
	{
		if (!EEQuad2D::Render())
			return false;
		
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Pull(int _scrollPos)
	{
		m_verticalScrollPos = _scrollPos;
		if (m_verticalScrollPos < 0)
			m_verticalScrollPos = 0;
		if (m_verticalScrollCapacity < m_verticalScrollPos)
			m_verticalScrollPos = m_verticalScrollCapacity;

		m_verticalPos = (m_verticalScrollPos / m_verticalScrollCapacity * (m_offsetHeight - GetHeight()));

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Repull()
	{
		// refresh
		m_offsetHeight = m_content.GetOffsetHeight();
		m_verticalScrollHeight = GetHeight() / m_offsetHeight * GetHeight();
		if (m_verticalScrollHeight > GetHeight())
			m_verticalScrollHeight = GetHeight();
		m_verticalScrollCapacity = m_verticalScrollHeight - GetHeight();

		// calculate the new pos
		m_verticalScrollPos = m_verticalPos * (m_verticalScrollCapacity / (m_offsetHeight - GetHeight()));
		if (m_verticalScrollPos < 0)
			m_verticalScrollPos = 0;
		if (m_verticalScrollCapacity < m_verticalScrollPos)
			m_verticalScrollPos = m_verticalScrollCapacity;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::SetContent(EEObject* _content)
	{
		if (_content)
		{
			_content->SetParent(&m_content);
			m_verticalPos = 0;
			Repull();

			return true;
		}

		return false;
	}
}
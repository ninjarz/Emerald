#include "EEScrollView.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEScrollView
	//----------------------------------------------------------------------------------------------------
	EEScrollView::EEScrollView(const Rect_Float& _area)
		:
		EEQuad2D(_area),
		m_contentFrame(FLOAT3(- GetWidth() / 2, - GetHeight() / 2, 0.f)),
		m_content(nullptr),
		m_offsetHeight(0),
		m_verticalScrollHeight(0),
		m_verticalScrollCapacity(0),
		m_verticalScrollPos(0),
		m_verticalPos(0)
	{
		SetIsFocusable(true);

		m_contentFrame.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EEScrollView::EEScrollView(const EEScrollView& _scrollView)
		:
		EEQuad2D(_scrollView),
		m_contentFrame(_scrollView.m_contentFrame),
		m_content(_scrollView.m_content),
		m_offsetHeight(_scrollView.m_offsetHeight),
		m_verticalScrollHeight(_scrollView.m_verticalScrollHeight),
		m_verticalScrollCapacity(_scrollView.m_verticalScrollCapacity),
		m_verticalScrollPos(_scrollView.m_verticalScrollPos),
		m_verticalPos(_scrollView.m_verticalPos)
	{
		// memo: m_contentFrame.SetParent(this);
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

		if (s_triggeredObject == this)
		{
			if (EECore::s_EECore->GetMouseDeltaM())
			{
				// ¡ü-m_verticalScrollPos  ¡ý+m_verticalScrollPos
				VerticalPull(m_verticalScrollPos - EECore::s_EECore->GetMouseDeltaM() / 30.f); // speed
			}
		}
		m_contentFrame.SetPositionY(- GetHeight() / 2 - m_verticalPos);
		m_contentFrame.Update();
		if (m_content)
		{
			m_content->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Render()
	{
		if (!EEQuad2D::Render())
			return false;
		
		m_contentFrame.Render();
		if (m_content)
		{
			m_content->Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::VerticalPull(float _scrollPos)
	{
		m_verticalScrollPos = _scrollPos;
		if (m_verticalScrollPos < 0)
			m_verticalScrollPos = 0;
		if (m_verticalScrollCapacity < m_verticalScrollPos)
			m_verticalScrollPos = m_verticalScrollCapacity;

		if (m_verticalScrollCapacity)
			m_verticalPos = (m_verticalScrollPos / m_verticalScrollCapacity * (m_offsetHeight - GetHeight()));
		else
			m_verticalPos = 0;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Repull()
	{
		// refresh
		m_offsetHeight = m_contentFrame.GetOffsetHeight() + GetHeight() / 2;
		m_verticalScrollHeight = GetHeight() / m_offsetHeight * GetHeight();
		if (m_verticalScrollHeight > GetHeight())
			m_verticalScrollHeight = GetHeight();
		m_verticalScrollCapacity = GetHeight() - m_verticalScrollHeight;

		// get the new pos
		if (m_offsetHeight - GetHeight())
			m_verticalScrollPos = m_verticalPos * (m_verticalScrollCapacity / (m_offsetHeight - GetHeight()));
		else
			m_verticalScrollPos = 0;

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
			m_content = _content;
			m_content->SetParent(&m_contentFrame);
			m_verticalPos = 0;
			Repull();

			return true;
		}

		return false;
	}
}
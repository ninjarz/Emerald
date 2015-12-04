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
		m_contentOrigin(FLOAT3(- GetWidth() / 2, - GetHeight() / 2, 0.f)),
		m_content(nullptr),
		m_offsetHeight(0),
		m_verticalScrollHeight(0),
		m_verticalScrollCapacity(0),
		m_verticalScrollPos(0),
		m_verticalPos(0),
		m_verticalPosDelta(0)
	{
		SetIsFocusable(true);

		m_contentOrigin.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EEScrollView::EEScrollView(const EEScrollView& _scrollView)
		:
		EEQuad2D(_scrollView),
		m_contentOrigin(_scrollView.m_contentOrigin),
		m_content(_scrollView.m_content),
		m_offsetHeight(_scrollView.m_offsetHeight),
		m_verticalScrollHeight(_scrollView.m_verticalScrollHeight),
		m_verticalScrollCapacity(_scrollView.m_verticalScrollCapacity),
		m_verticalScrollPos(_scrollView.m_verticalScrollPos),
		m_verticalPos(_scrollView.m_verticalPos),
		m_verticalPosDelta(_scrollView.m_verticalPosDelta)
	{
		// memo: m_contentOrigin.SetParent(this);
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
				m_verticalPosDelta += EECore::s_EECore->GetMouseDeltaM() / 5.f;
			}

			if (m_verticalPosDelta)
			{
				// The posDelta depends on the deltaTime (1.0 - ¡Þ from inverse proportional function)
				float deltaFactor = float(0.16 / EECore::s_EECore->GetDeltaTime() + 1.0);
				float currentPosDelta = abs(m_verticalPosDelta) > 1.f ? m_verticalPosDelta / deltaFactor : m_verticalPosDelta;
				VerticalPull(m_verticalScrollPos - currentPosDelta);
				m_verticalPosDelta -= currentPosDelta;
			}
		}
		m_contentOrigin.SetPositionY(- GetHeight() / 2 - m_verticalPos);
		m_contentOrigin.Update();
		if (m_content)
		{
			m_content->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEScrollView::Render()
	{
		EECore::s_EECore->ClearDepthStencilView();
		D3D11_DEPTH_STENCIL_DESC descBackup = EECore::s_EECore->GetDepthStencilDesc();
		D3D11_DEPTH_STENCIL_DESC desc = descBackup;
		desc.StencilEnable = true;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER;
		EECore::s_EECore->SetDepthStencilState(desc);

		if (!EEQuad2D::Render())
		{
			EECore::s_EECore->SetDepthStencilState(descBackup);
			return false;
		}

		if (m_content)
		{
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
			EECore::s_EECore->SetDepthStencilState(desc);
			m_contentOrigin.Render();
			m_content->Render();
		}

		EECore::s_EECore->ClearDepthStencilView();
		EECore::s_EECore->SetDepthStencilState(descBackup);
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
		m_offsetHeight = m_contentOrigin.GetOffsetHeight() + GetHeight() / 2;
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
			m_content->SetParent(&m_contentOrigin);
			m_verticalPos = 0;
			Repull();

			return true;
		}

		return false;
	}
}
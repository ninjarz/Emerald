#include "EEComboBox.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	EEComboBox::EEComboBox(const Rect_Float& _rect, const EEColor& _color, const EEColor& _fontColor)
		:
		EELineBrowser(_rect, _color, _fontColor),
		m_isSelecting(false),
		m_options(),
		m_selected(-1),
		m_isSelectedDirty(true)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEComboBox::~EEComboBox()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEComboBox::Update()
	{
		if (m_isSelectedDirty)
		{
			if (0 <= m_selected && m_selected < (int)m_options.size())
			{
				SetText(m_options[m_selected]->GetText());
				m_isSelectedDirty = false;
			}
		}
		if (!EELineBrowser::Update())
			return false;

		if (m_isSelecting)
		{
			for (EELineBrowser* item : m_options)
				item->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEComboBox::Render()
	{
		if (!EELineBrowser::Render())
			return false;

		if (m_isSelecting)
		{
			for (EELineBrowser* item : m_options)
				item->Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEComboBox::AddItem(const std::wstring& _str, std::function<void(void)> _funcPtr)
	{
		float height = GetHeight() * (m_options.size() + 1);
		m_options.push_back(new EELineBrowser(Rect_Float(0.0f, height, GetWidht(), height + GetHeight()),
			m_color, m_font.GetColor(), _str));
		int index = m_options.size() - 1;
		m_options[index]->SetParent(this);
		m_options[index]->SetTriggeredFunc(
			[&, index, _funcPtr]()
		{
			_funcPtr();
			m_selected = index;
			m_isSelectedDirty = true;
			m_isSelecting = false;
		});

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEComboBox::OnMouseClicked(const Point& _pos)
	{
		EELineBrowser::OnMouseClicked(_pos);

		m_isSelecting = !m_isSelecting;
	}
}
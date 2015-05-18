#include "EEColor.h"

namespace Emerald
{
	//EEColor
	//----------------------------------------------------------------------------------------------------
	const EEColor EEColor::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
	const EEColor EEColor::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	const EEColor EEColor::RED(1.0f, 0.0f, 0.0f, 1.0f);
	const EEColor EEColor::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
	const EEColor EEColor::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
	const EEColor EEColor::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
	const EEColor EEColor::CYAN(0.0f, 1.0f, 1.0f, 1.0f);
	const EEColor EEColor::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
	const EEColor EEColor::SILVER(0.75f, 0.75f, 0.75f, 1.0f);
	const EEColor EEColor::COLORS[9] = { EEColor::BLACK, EEColor::WHITE, EEColor::RED, EEColor::GREEN, EEColor::BLUE, EEColor::YELLOW, EEColor::CYAN, EEColor::MAGENTA, EEColor::SILVER };
	const EEColor EEColor::LUCENCY(0.0f, 0.0f, 0.0f, 0.0f);
	
	//----------------------------------------------------------------------------------------------------
	EEColor::EEColor()
		:
		FLOAT4()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEColor::EEColor(FLOAT _red, FLOAT _green, FLOAT _yellow, FLOAT _alpha)
		:
		FLOAT4(_red, _green, _yellow, _alpha)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEColor::EEColor(const FLOAT4& _color)
		:
		FLOAT4(_color)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEColor::EEColor(const EEColor& _color)
		:
		FLOAT4(_color)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEColor::~EEColor()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEColor::SetColor(const FLOAT4& _color)
	{
		*this = _color;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT4& EEColor::GetColor()
	{
		return *this;
	}

}
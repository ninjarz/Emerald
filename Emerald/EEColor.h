#pragma once
#ifndef _EE_COLOR_H_
#define _EE_COLOR_H_

#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEColor
	//----------------------------------------------------------------------------------------------------
	class EEColor : public FLOAT4
	{
	public:
		static const EEColor BLACK;
		static const EEColor WHITE;
		static const EEColor RED;
		static const EEColor GREEN;
		static const EEColor BLUE;
		static const EEColor YELLOW;
		static const EEColor CYAN;
		static const EEColor MAGENTA;
		static const EEColor SILVER;
		static const EEColor COLORS[9];
		static const EEColor LUCENCY;

	public:
		EEColor();
		EEColor(FLOAT _red, FLOAT _green, FLOAT _yellow, FLOAT _alpha);
		EEColor(const FLOAT4& _color);
		EEColor(const EEColor& _color);
		virtual ~EEColor();

		bool SetColor(const FLOAT4& _color);

		const FLOAT4& GetColor();
	};
}

#endif
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
		static const FLOAT4 BLACK;
		static const FLOAT4 WHITE;
		static const FLOAT4 RED;
		static const FLOAT4 GREEN;
		static const FLOAT4 BLUE;
		static const FLOAT4 YELLOW;
		static const FLOAT4 CYAN;
		static const FLOAT4 MAGENTA;
		static const FLOAT4 SILVER;
		static const FLOAT4 COLORS[9];
		static const FLOAT4 LUCENCY;

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
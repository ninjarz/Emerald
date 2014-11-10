#include "EEColor.h"

namespace Emerald
{
	//EEColor
	//----------------------------------------------------------------------------------------------------
	const FLOAT4 EEColor::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
	const FLOAT4 EEColor::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	const FLOAT4 EEColor::RED(1.0f, 0.0f, 0.0f, 1.0f);
	const FLOAT4 EEColor::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
	const FLOAT4 EEColor::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
	const FLOAT4 EEColor::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
	const FLOAT4 EEColor::CYAN(0.0f, 1.0f, 1.0f, 1.0f);
	const FLOAT4 EEColor::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
	const FLOAT4 EEColor::SILVER(0.75f, 0.75f, 0.75f, 1.0f);

	const FLOAT4 EEColor::COLORS[9] = { EEColor::BLACK, EEColor::WHITE, EEColor::RED, EEColor::GREEN, EEColor::BLUE, EEColor::YELLOW, EEColor::CYAN, EEColor::MAGENTA, EEColor::SILVER };
}
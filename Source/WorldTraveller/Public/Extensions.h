#pragma once

#include "CoreMinimal.h"

namespace Extensions
{
	constexpr float Remap(float x, float a, float b, float c, float d)
	{
		if (a == b) return 0;
		return (x - a) * (d - c) / (b - a) + c;
	}

	constexpr float RemapClamped(float x, float a, float b, float c, float d)
	{
		if (a == b) return 0;
		float o = (x - a) * (d - c) / (b - a) + c;
		return FMath::Clamp(o, c, d);
	}
}

#pragma once

namespace Extensions
{
	constexpr float Remap(float x, float a, float b, float c, float d)
	{
		if (a == b) return 0;
		return (x - a) * (d - c) / (b - a) + c;
	}
}

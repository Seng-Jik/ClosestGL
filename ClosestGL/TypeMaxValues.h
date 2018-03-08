#pragma once
#include <cstdint>

namespace ClosestGL::Math
{
	template<typename T>
	constexpr T TypeMaxValue()
	{
		return T::MaxValue();
	}

	template<>
	constexpr float TypeMaxValue()
	{
		return 1.0f;
	}

	template<>
	constexpr double TypeMaxValue()
	{
		return 1.0;
	}

	template<>
	constexpr uint8_t TypeMaxValue()
	{
		return 255u;
	}

	template<>
	constexpr uint16_t TypeMaxValue()
	{
		return 65535u;
	}
}
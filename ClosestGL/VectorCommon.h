#pragma once

namespace ClosestGL::Math
{
	template<typename T>
	constexpr auto Distance(const T& a, const T& b)
	{
		return (a - b).Length();
	}

	template<typename T>
	constexpr T Normalize(const T& a)
	{
		return a / a.Length();
	}
}
#pragma once

namespace ClosestGL::Math
{
	template<typename T>
	constexpr T& operator += (T& a, const T& b)
	{
		a = a + b;
		return a;
	}

	template<typename T>
	constexpr T& operator -= (T& a, const T& b)
	{
		a = a - b;
		return a;
	}


	template<typename T, typename U>
	constexpr T& operator *= (T& a, const U& b)
	{
		a = a * b;
		return a;
	}

	template<typename T, typename U>
	constexpr T& operator /= (T& a, const U& b)
	{
		a = a / b;
		return a;
	}
}

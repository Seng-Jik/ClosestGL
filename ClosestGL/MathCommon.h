#pragma once

namespace ClosestGL::Math
{
	/* 数学公共库 */

	/* 以下操作符用于为向量和矩阵提供赋值运算符的处理。*/
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

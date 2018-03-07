#pragma once

namespace ClosestGL::Math
{
	/* 向量公共库 */

	/* 求两点距离 */
	template<typename T>
	constexpr auto Distance(const T& a, const T& b)
	{
		return (a - b).Length();
	}

	/* 求归一化 */
	template<typename T>
	constexpr T Normalize(const T& a)
	{
		return a / a.Length();
	}
}
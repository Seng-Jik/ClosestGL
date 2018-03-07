#pragma once
#include "Number.h"

namespace ClosestGL::Math
{
	/* 4维向量 */
	template<typename T>
	struct Vector4
	{
		T x, y, z, w;

		/* 求模 */
		constexpr T Length() const
		{
			return static_cast<T>(Sqrt(x * x + y * y + z * z + w * w));
		}
	};

	template<typename T>
	constexpr Vector4<T> operator + (const Vector4<T>& a, const Vector4<T>& b)
	{
		return
		{
			a.x + b.x,
			a.y + b.y,
			a.z + b.z,
			a.w + b.w
		};
	}

	template<typename T>
	constexpr Vector4<T> operator - (const Vector4<T>& a, const Vector4<T>& b)
	{
		return
		{
			a.x - b.x,
			a.y - b.y,
			a.z - b.z,
			a.w - b.w
		};
	}

	template<typename T>
	constexpr Vector4<T> operator * (const Vector4<T>& a, const T& b)
	{
		return
		{
			a.x * b,
			a.y * b,
			a.z * b,
			a.w * b
		};
	}

	template<typename T>
	constexpr bool operator == (const Vector4<T>& a, const Vector4<T>& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	template<typename T>
	constexpr Vector4<T> operator / (const Vector4<T>& a, const T& b)
	{
		return
		{
			a.x / b,
			a.y / b,
			a.z / b,
			a.w / b
		};
	}

	/* 求4维向量点积 */
	template<typename T>
	constexpr T Dot(const Vector4<T>& a, const Vector4<T>& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
}
#pragma once
#include "Number.h"

namespace ClosestGL::Math
{
	/* 3维向量 */
	template<typename T>
	struct Vector3
	{
		T x, y, z;

		using BaseType = T;

		/* 求模 */
		constexpr T Length() const
		{
			return static_cast<T>(Sqrt(x * x + y * y + z * z));
		}
	};

	template<typename T>
	constexpr Vector3<T> operator + (const Vector3<T>& a, const Vector3<T>& b)
	{
		return
		{
			a.x + b.x,
			a.y + b.y,
			a.z + b.z
		};
	}

	template<typename T>
	constexpr Vector3<T> operator - (const Vector3<T>& a, const Vector3<T>& b)
	{
		return
		{
			a.x - b.x,
			a.y - b.y,
			a.z - b.z
		};
	}

	template<typename T>
	constexpr Vector3<T> operator * (const Vector3<T>& a, const T& b)
	{
		return
		{
			a.x * b,
			a.y * b,
			a.z * b
		};
	}

	template<typename T>
	constexpr bool operator == (const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	template<typename T>
	constexpr Vector3<T> operator / (const Vector3<T>& a, const T& b)
	{
		return Vector3<T>
		{
			a.x / b,
			a.y / b,
			a.z / b
		};
	}

	/* 求3维向量点积 */
	template<typename T>
	constexpr T Dot(const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
}
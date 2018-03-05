#pragma once
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace ClosestGL::Math
{
	template<typename T>
	constexpr Matrix2<T> GetScaleMatrix(T x, T y)
	{
		return Matrix2<T>
		{
			{ x, 0 },
			{ 0, y }
		};
	}

	template<typename T>
	constexpr Matrix3<T> GetScaleMatrix(T x, T y,T z)
	{
		return Matrix3<T>
		{
			{ x, 0, 0 },
			{ 0, y, 0 },
			{ 0, 0, z },
		};
	}

	template<typename T>
	constexpr Matrix4<T> GetScaleMatrix(T x, T y, T z,T w)
	{
		return Matrix4<T>
		{
			{ x, 0, 0, 0 },
			{ 0, y, 0, 0 },
			{ 0, 0, z, 0 },
			{ 0, 0, 0, w }
		};
	}

	template<typename T>
	constexpr Matrix3<T> GetTransformMatrix(Vector2<T> v)
	{
		return Matrix3<T>
		{
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ v.x, v.y, 1 },
		};
	}

	template<typename T>
	constexpr Matrix4<T> GetTransformMatrix(Vector3<T> v)
	{
		return Matrix4<T>
		{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ v.x, v.y, v.z, 1}
		};
	}
}

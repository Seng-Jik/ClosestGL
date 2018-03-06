#pragma once
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace ClosestGL::Math
{
	template<typename T>
	constexpr Matrix2<T> GetScaleMatrix(T x, T y)
	{
		return
		{
			{ x, 0 },
			{ 0, y }
		};
	}

	template<typename T>
	constexpr Matrix3<T> GetScaleMatrix(T x, T y,T z)
	{
		return
		{
			{ x, 0, 0 },
			{ 0, y, 0 },
			{ 0, 0, z },
		};
	}

	template<typename T>
	constexpr Matrix4<T> GetScaleMatrix(T x, T y, T z,T w)
	{
		return
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
		return
		{
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ v.x, v.y, 1 },
		};
	}

	template<typename T>
	constexpr Matrix4<T> GetTransformMatrix(Vector3<T> v)
	{
		return
		{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ v.x, v.y, v.z, 1}
		};
	}

	template<typename T>
	Matrix2<T> GetRotateMatrix(T radian, Vector2<T> point) = delete;

	inline Matrix2<float> GetZRotateMatrix(float radian)
	{
		return
		{
			{std::cosf(radian), std::sinf(radian)},
			{-std::sinf(radian),std::cosf(radian)}
		};
	}

	inline Matrix2<double> GetZRotateMatrix(double radian)
	{
		return
		{
			{std::cos(radian), std::sin(radian)},
			{-std::sin(radian),std::cos(radian) }
		};
	}

	template<typename T>
	Matrix3<T> GetYRotateMatrix(double radian) = delete;

	inline Matrix3<float> GetYRotateMatrix(float radian)
	{
		return
		{
			{ std::cosf(radian),0,-std::sinf(radian) },
			{ 0,1,0 },
			{ std::sinf(radian),0,std::cosf(radian) }
		};
	}

	inline Matrix3<double> GetYRotateMatrix(double radian)
	{
		return
		{
			{ std::cos(radian),0,-std::sin(radian) },
			{ 0,1,0 },
			{ std::sin(radian),0,std::cos(radian) }
		};
	}

	template<typename T>
	Matrix3<T> GetXRotateMatrix(double radian) = delete;

	inline Matrix3<float> GetXRotateMatrix(float radian)
	{
		return
		{
			{ 1,0,0 },
			{ 0,std::cosf(radian),std::sinf(radian) },
			{ 0,-std::sinf(radian),std::cosf(radian) }
		};
	}

	inline Matrix3<double> GetXRotateMatrix(double radian)
	{
		return
		{
			{ 1,0,0 },
			{ 0,std::cos(radian),std::sin(radian) },
			{ 0,-std::sin(radian),std::cos(radian) }
		};
	}
}

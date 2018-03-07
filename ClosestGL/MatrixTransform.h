#pragma once
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace ClosestGL::Math
{
	/* 矩阵公式库 */

	/* 获取2x2的缩放矩阵 */
	template<typename T>
	constexpr Matrix2<T> GetScaleMatrix(T x, T y)
	{
		return
		{
			{ x, 0 },
			{ 0, y }
		};
	}

	/* 获取3x3的缩放矩阵 */
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

	/* 获取3x3的平移矩阵（适用于2D） */
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

	/* 获取4x4的平移矩阵（适用于3D） */
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

	/* 获取2x2的绕Z轴旋转的矩阵 
	* radian - 旋转角（使用弧度表示）
	*/
	template<typename T>
	Matrix2<T> GetRotateMatrix(T radian) = delete;

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

	/* 获取3x3的绕Y轴旋转的矩阵
	* radian - 旋转角（使用弧度表示）
	*/
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

	/* 获取3x3的绕X轴旋转的矩阵
	* radian - 旋转角（使用弧度表示）
	*/
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

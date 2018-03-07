#pragma once
#include "Vector3.h"
#include "Matrix4.h"
namespace ClosestGL::Math
{
	/*	3x3矩阵
	*	T - 基于的标量类型
	*/
	template<typename T>
	struct Matrix3
	{
		Vector3<T> col1, col2, col3;

		/* 转换为同语义的4x4矩阵 */
		operator Matrix4<T>()
		{
			return
			{
				{col1.x, col1.y, col1.z, 0},
				{col2.x, col2.y, col2.z, 0},
				{col3.x, col3.y, col3.z, 0},
				{0, 0, 0, 1}
			};
		}
	};

	/* 4x4单位矩阵 */
	template<typename T>
	constexpr static Matrix3<T> Matrix3Identity
	{
		{ 1,0,0 },
		{ 0,1,0 },
		{ 0,0,1 }
	};

	/* 注意：向量只能左乘矩阵 */
	template<typename T>
	constexpr Vector3<T> operator * (const Matrix3<T> mat, const Vector3<T>& vec)
	{
		const auto a = mat.col1 * vec.x;
		const auto b = mat.col2 * vec.y;
		const auto c = mat.col3 * vec.z;
		return a + b + c;
	}

	/* 注意：使用矩阵间的乘法时，请使用括号写明运算顺序，否则不会执行左乘。 */
	template<typename T>
	constexpr Matrix3<T> operator * (const Matrix3<T>& a, const Matrix3<T>& b)
	{
		return
		{
			a * b.col1,
			a * b.col2,
			a * b.col3
		};
	}

	template<typename T>
	constexpr bool operator == (const Matrix3<T>& a, const Matrix3<T>& b)
	{
		return
			a.col1 == b.col1 &&
			a.col2 == b.col2 &&
			a.col3 == b.col3;
	}
}

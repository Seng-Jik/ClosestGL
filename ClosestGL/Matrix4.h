#pragma once
#include "Vector4.h"

namespace ClosestGL::Math
{
	/* 4x4矩阵 */
	template<typename T>
	struct Matrix4
	{
		Vector4<T> col1, col2, col3, col4;

		using BaseType = T;
	};

	/* 4x4单位矩阵 */
	template<typename T>
	constexpr static Matrix4<T> Matrix4Identity
	{
		{ 1,0,0,0 },
		{ 0,1,0,0 },
		{ 0,0,1,0 },
		{ 0,0,0,1 }
	};

	/* 注意：向量只能左乘矩阵 */
	template<typename T>
	constexpr Vector4<T> operator * (const Matrix4<T> mat, const Vector4<T>& vec)
	{
		const auto a = mat.col1 * vec.x;
		const auto b = mat.col2 * vec.y;
		const auto c = mat.col3 * vec.z;
		const auto d = mat.col4 * vec.w;
		return a + b + c + d;
	}

	/* 注意：使用矩阵间的乘法时，请使用括号写明运算顺序，否则不会执行左乘。 */
	template<typename T>
	constexpr Matrix4<T> operator * (const Matrix4<T>& a, const Matrix4<T>& b)
	{
		return
		{
			a * b.col1,
			a * b.col2,
			a * b.col3,
			a * b.col4
		};
	}

	template<typename T>
	constexpr bool operator == (const Matrix4<T>& a, const Matrix4<T>& b)
	{
		return
			a.col1 == b.col1 &&
			a.col2 == b.col2 &&
			a.col3 == b.col3 &&
			a.col4 == b.col4;
	}
}

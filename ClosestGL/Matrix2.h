#pragma once
#include "Vector2.h"
#include "Matrix3.h"

namespace ClosestGL::Math
{
	template<typename T>
	struct Matrix2
	{
		Vector2<T> col1, col2;

		operator Matrix3<T>()
		{
			return
			{
				{ col1.x, col1.y, 0 },
				{ col2.x, col2.y, 0 },
				{ 0, 0, 1 }
			};
		}
	};

	template<typename T>
	constexpr static Matrix2<T> Matrix2Identity
	{
		{ 1,0 },
		{ 0,1 }
	};

	template<typename T>
	constexpr Vector2<T> operator * (const Matrix2<T> mat, const Vector2<T>& vec)
	{
		const auto a = mat.col1 * vec.x;
		const auto b = mat.col2 * vec.y;
		return a + b;
	}

	template<typename T>
	constexpr Matrix2<T> operator * (const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return Matrix2<T>
		{
			a * b.col1,
			a * b.col2
		};
	}

	template<typename T>
	constexpr bool operator == (const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return
			a.col1 == b.col1 &&
			a.col2 == b.col2;
	}
}

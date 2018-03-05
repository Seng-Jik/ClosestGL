#pragma once
#include "Vector3.h"

namespace ClosestGL::Math
{
	template<typename T>
	struct Matrix3
	{
		Vector3<T> col1, col2, col3;
	};

	template<typename T>
	inline constexpr static Matrix3<T> Matrix3Identity
	{
		{ 1,0,0 },
		{ 0,1,0 },
		{ 0,0,1 }
	};

	template<typename T>
	constexpr Vector3<T> operator * (const Matrix3<T> mat, const Vector3<T>& vec)
	{
		const auto a = mat.col1 * vec.x;
		const auto b = mat.col2 * vec.y;
		const auto c = mat.col3 * vec.z;
		return a + b + c;
	}


	template<typename T>
	constexpr Matrix3<T> operator * (const Matrix3<T>& a, const Matrix3<T>& b)
	{
		return Matrix3<T>
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

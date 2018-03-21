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
	Matrix3<T> GetXRotateMatrix(double radian);

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

	/* 获取4x4的透视投影矩阵
	 *     fovy   - 视角（弧度制表示）
	 *     aspect - 视口宽高比
	 *     zn     - 近平面
	 *     zf     - 远平面
	 */

	template<typename T>
	Matrix4<T> GetPerspectiveMatrixLH(T fovy, T aspect, T zn, T zf);

	template<>
	inline Matrix4<float> GetPerspectiveMatrixLH(float fovy, float aspect, float zn, float zf)
	{
		float fax = 1.0f / std::tanf(fovy * 0.5f);
		return {
			{ fax / aspect, 0, 0, 0 },
			{ 0, fax, 0, 0 },
			{ 0, 0, zf / (zf - zn) ,1 },
			{ 0, 0, -zn * zf / (zf - zn) , 0 },
		};
	}

	template<>
	inline Matrix4<double> GetPerspectiveMatrixLH(double fovy, double aspect, double zn, double zf)
	{
		double fax = 1.0f / std::tan(fovy * 0.5f);
		return {
			{ fax / aspect, 0, 0, 0 },
			{ 0, fax, 0, 0 },
			{ 0, 0, zf / (zf - zn) ,1 },
			{ 0, 0, -zn * zf / (zf - zn) , 0 },
		};
	}

	/* 获取视角矩阵
	 *     eyePos - 眼睛位置
	 *     lookAt - 看向
	 *     up     - 上方向
	 */

	template<typename T>
	constexpr Matrix4<T> GetLookAtMatrix(Vector3<T> eyePos, Vector3<T> lookAt, Vector3<T> up)
	{
		auto zaxis = Normalize(lookAt - eyePos);
		auto xaxis = Normalize(Cross(up,zaxis));
		auto yaxis = Normalize(Cross(zaxis,xaxis));

		auto eyex = Dot(xaxis * T(-1), eyePos);
		auto eyey = Dot(yaxis * T(-1), eyePos);
		auto eyez = Dot(zaxis * T(-1), eyePos);

		return {
			{ xaxis.x, yaxis.x, zaxis.x, 0},
			{ xaxis.y, yaxis.y, zaxis.y, 0},
			{ xaxis.z, yaxis.z, zaxis.z, 0},
			{ eyex, eyey,eyez , 1}
		};
	}
}

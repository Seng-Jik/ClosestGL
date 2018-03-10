#pragma once
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace ClosestGL::Math
{
	/* ����ʽ�� */

	/* ��ȡ2x2�����ž��� */
	template<typename T>
	constexpr Matrix2<T> GetScaleMatrix(T x, T y)
	{
		return
		{
			{ x, 0 },
			{ 0, y }
		};
	}

	/* ��ȡ3x3�����ž��� */
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

	/* ��ȡ3x3��ƽ�ƾ���������2D�� */
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

	/* ��ȡ4x4��ƽ�ƾ���������3D�� */
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

	/* ��ȡ2x2����Z����ת�ľ��� 
	* radian - ��ת�ǣ�ʹ�û��ȱ�ʾ��
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

	/* ��ȡ3x3����Y����ת�ľ���
	* radian - ��ת�ǣ�ʹ�û��ȱ�ʾ��
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

	/* ��ȡ3x3����X����ת�ľ���
	* radian - ��ת�ǣ�ʹ�û��ȱ�ʾ��
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

	/* ��ȡ4x4��͸��ͶӰ����
	 *     fovy   - �ӽǣ������Ʊ�ʾ��
	 *     aspect - �ӿڿ�߱�
	 *     zn     - ��ƽ��
	 *     zf     - Զƽ��
	 */

	template<typename T>
	Matrix4<T> GetPerspectiveMatrixLH(T fovy, T aspect, T zn, T zf) = delete;

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

	/* ��ȡ�ӽǾ���
	 *     eyePos - �۾�λ��
	 *     lookAt - ����
	 *     up     - �Ϸ���
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

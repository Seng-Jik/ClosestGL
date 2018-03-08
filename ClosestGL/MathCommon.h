#pragma once

namespace ClosestGL::Math
{
	/* ��ѧ������ */

	/* ��ֵ���� */
	template<typename T,typename U>
	constexpr auto Lerp(const T& top, const T& bottom, const U& x)
	{
		return (bottom - top) * x + top;
	}

	/* ���²���������Ϊ�����;����ṩ��ֵ������Ĵ���*/
	template<typename T>
	constexpr T& operator += (T& a, const T& b)
	{
		a = a + b;
		return a;
	}

	template<typename T>
	constexpr T& operator -= (T& a, const T& b)
	{
		a = a - b;
		return a;
	}


	template<typename T, typename U>
	constexpr T& operator *= (T& a, const U& b)
	{
		a = a * b;
		return a;
	}

	template<typename T, typename U>
	constexpr T& operator /= (T& a, const U& b)
	{
		a = a / b;
		return a;
	}
}

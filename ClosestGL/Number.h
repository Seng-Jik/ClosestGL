#pragma once
#include <cmath>

namespace ClosestGL::Math
{
	template<typename T>
	constexpr auto Sqrt(const T& v)
	{
		return v.Sqrt();
	}

	inline auto Sqrt(const int& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const short& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const char& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const long& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const long long& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const unsigned int& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const unsigned short& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const unsigned char& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const unsigned long& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const unsigned long long& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const float& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const double& v)
	{
		return std::sqrt(v);
	}

	inline auto Sqrt(const long double& v)
	{
		return std::sqrt(v);
	}
}

#pragma once
#include <algorithm>

namespace ClosestGL::Texture::Sampler::UVNormalizer
{
	const auto UV2DClamp = [](const auto& uv)
	{
		auto x = uv.x;
		if (x > 1) x = 1;
		if (x < 0) x = 0;

		auto y = uv.y;
		if (y > 1) y = 1;
		if (y < 0) y = 0;

		return std::decay<decltype(uv)>::type{
			x,y
		};
	};

	const auto UV2DRepeat = [](const auto& uv)
	{
		auto x = uv.x;
		while (x < 0) x += 1;
		while (x > 1) x -= 1;

		auto y = uv.y;
		while (y < 0) y += 1;
		while (y > 1) y -= 1;

		return std::decay<decltype(uv)>::type{
			x,y
		};
	};
}
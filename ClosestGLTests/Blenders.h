#pragma once
namespace ClosestGLTests::Tools::Blenders
{
	const auto NoBlend = [](Tools::TestCol src, auto) { return src; };

	const auto SimpleBlender = [](auto src, auto dst) 
	{
		return (src + dst) / 2.0f;
	};

	const auto AddtiveBlender = [](auto src, auto dst)
	{
		return src + dst;
	};
}
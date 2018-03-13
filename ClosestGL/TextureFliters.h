#pragma once
#include "Vector2.h"
namespace ClosestGL::Texture::Sampler::Fliters
{
	const auto Nearest = [](const auto& uv, const auto& tex) {

		auto texSize = tex.GetSize();
		Math::Vector2<size_t> pos
		{
			size_t((texSize.x - 1) * uv.x),
			size_t((texSize.y - 1) * uv.y)
		};
		return tex.ReadPixelUnsafe(pos);
	};

	const auto Bilinear = [](auto uv, const auto& tex)
	{
		const auto texSize = tex.GetSize();
		uv.x *= texSize.x - 1;
		uv.y *= texSize.y - 1;

		Math::Vector2<size_t> uvBase
		{
			size_t(uv.x),
			size_t(uv.y)
		};

		decltype(uv) uvOffset
		{
			uv.x - uvBase.x,
			uv.y - uvBase.y
		};

		auto texelBase = tex.ReadPixelUnsafe(uvBase);

		auto uvsmp = uvBase;
		uvsmp.x++;
		if (uvsmp.x >= texSize.x) uvsmp.x = uvBase.x;
		auto texelXO = tex.ReadPixelUnsafe(uvsmp);

		uvsmp.y++;
		if (uvsmp.y >= texSize.y) uvsmp.y = uvBase.y;
		auto texelXYO = tex.ReadPixelUnsafe(uvsmp);

		uvsmp.x = uvBase.x;
		auto texelYO = tex.ReadPixelUnsafe(uvsmp);

		return
			texelBase * (1 - uvOffset.x)*	(1 - uvOffset.y) +	//左上角分量
			texelXO * uvOffset.x *		(1 - uvOffset.y) +	//右上角分量
			texelYO * (1 - uvOffset.x)*	uvOffset.y +		//左下角分量
			texelXYO * uvOffset.x *		uvOffset.y;			//右下角分量
	};
}

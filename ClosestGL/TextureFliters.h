#pragma once

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
}

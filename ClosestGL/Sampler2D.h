#pragma once
#include "Vector2.h"

namespace ClosestGL::Texture::Sampler
{
	/* 采样器
	 * 用于进行2D的采样，需要传入UVNormalizer来确定超出采样范围后采样器的处理方式。
	 */
	template<typename Texture2D,typename UVNormalizer>
	class Sampler2D
	{
	private:
		const Texture2D* const tex_;
		UVNormalizer uvNorm_;

	public:
		Sampler2D(Texture2D* tex, UVNormalizer uvNorm):
			tex_{ tex },
			uvNorm_{ uvNorm }
		{}

		template<typename UVType>
		auto Sample(UVType uv) const
		{
			uv = uvNorm_(uv);

			const auto& texSize = tex_->GetSize();

			//Nearest
			Math::Vector2<size_t> pos
			{
				size_t((texSize.x - 1) * uv.x),
				size_t((texSize.y - 1) * uv.y)
			};

			return tex_->ReadPixelUnsafe(pos);
		}
	};
}

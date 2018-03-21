#pragma once
#include "Vector2.h"

namespace ClosestGL::Texture::Sampler
{
	/* 采样器
	 * 用于进行2D的采样，需要传入UVNormalizer来确定超出采样范围后采样器的处理方式。
	 */
	template<typename Texture2D, typename UVNormalizer, typename Fliter>
	class Sampler2D
	{
	private:
		const Texture2D* const tex_;
		UVNormalizer uvNorm_;
		Fliter fliter_;

	public:
		Sampler2D(const Texture2D* tex, UVNormalizer uvNorm, Fliter fliter) :
			tex_{ tex },
			uvNorm_{ uvNorm },
			fliter_{ fliter }
		{}

		template<typename UVType>
		auto Sample(UVType uv) const
		{
			uv = uvNorm_(uv);
			return fliter_(uv, *tex_);
		}
	};
}
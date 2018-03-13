#pragma once
#include "Vector2.h"

namespace ClosestGL::Texture::Sampler
{
	/* ������
	 * ���ڽ���2D�Ĳ�������Ҫ����UVNormalizer��ȷ������������Χ��������Ĵ���ʽ��
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

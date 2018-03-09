#pragma once
#include "Vector2.h"
#include "Texture2D.h"
#include <optional>
#include <array>

namespace ClosestGL::RenderPipeline
{	template<int ColorBufferCount,typename ColorType,typename Blender>
	class RenderTarget
	{
	private:
		using Texture = Texture::Texture2D<ColorType>;

		std::array<std::optional<Texture>, ColorBufferCount> cbs_;
		Blender blender_;
		Math::Vector2<size_t> size_;

	public:

		constexpr static int GetColorBufferCount()
		{
			return ColorBufferCount;
		}

		RenderTarget(const Blender& blender,const Math::Vector2<size_t>& size):
			blender_{ blender },
			size_{ size }
		{
			for (auto& p : cbs_)
				p.emplace(size);
		}

		Texture& GetTexture(int n)
		{
			return *cbs_.at(n);
		}

		void EmitPixel(const std::array<ColorType, ColorBufferCount>& colors, const Math::Vector2<size_t>& pos)
		{
			for (int i = 0; i < ColorBufferCount; ++i)
			{
				auto& dst = (*cbs_[i]).AccessPixelUnsafe(pos);
				dst = blender_(colors[i], dst);
			}
		}

		auto GetSize() const
		{
			return size_;
		}
	};
}
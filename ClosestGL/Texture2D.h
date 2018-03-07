#pragma once
#include <vector>
#include "Vector2.h"

namespace ClosestGL::Texture
{
	template<typename ColorType>
	class Texture2D
	{
	private:
		std::vector<ColorType> color_;
		Math::Vector2<size_t> size_;

	public:
		Texture2D(Math::Vector2<size_t> size) :
			size_{ size },
			color_{ size.x * size.y }
		{}

		Math::Vector2<size_t> GetSize() const { return size_; };

		ColorType* Data() { return color_.data(); }

		size_t CalcOffset(Math::Vector2<size_t> pos)
		{
			return pos.y * size_.x + pos.x;
		}

		ColorType& AccessPixel(Math::Vector2<size_t> pos)
		{
			return color_.at(CalcOffset(pos));
		}

		ColorType& AccessPixelUnsafe(Math::Vector2<size_t> pos)
		{
			return color_[CalcOffset(pos)];
		}

		template<typename TextureShader,typename Runner>
		void Shade(TextureShader& ts,Runner& runner)
		{
			runner.Commit(0, size_.y,
				[this,ts](size_t y,size_t) {
					for (size_t x = 0; x < size_.x; ++x)
					{
						Math::Vector2<size_t> pos{ x, y };
						AccessPixelUnsafe(pos) = ts(pos);
					}
				}
			);
		}

		template<typename Runner>
		void Clear(ColorType color, Runner& runner)
		{
			Shade(
				[color](auto)
				{
					return color;
				}, 
				runner
			);
		}
	};
}
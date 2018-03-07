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

		size_t CalcOffset(Math::Vector2<size_t> offset)
		{
			return offset.y * size_.x + offset.x;
		}

		ColorType& AccessPixel(Math::Vector2<size_t> offset)
		{
			return color_.at(CalcOffset(offset));
		}

		ColorType& AccessPixelUnsafe(Math::Vector2<size_t> offset)
		{
			return color_[CalcOffset(offset)];
		}

		
	};
}
#pragma once
#include "Vector2.h"

namespace ClosestGL::Utils
{
	template<typename ColorType,typename DepthType>
	struct PixelInformation
	{
		Math::Vector2<size_t> position;
		ColorType color;
		DepthType depth;
	};
}
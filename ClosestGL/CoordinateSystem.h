#pragma once
#include "Vector2.h"
namespace ClosestGL::Math
{
	template<typename T>
	Math::Vector2<size_t> ConvertVertexPosToRenderTargetPos(
		T vertexPos,
		Math::Vector2<size_t> rtSize)
	{
		T conved
		{
			((vertexPos.x + 1) / 2) * rtSize.x,
			(1 - (vertexPos.y + 1) / 2) * rtSize.y
		};

		if (conved.x - int(conved.x) > 0.5f)
			conved.x += 1;

		if (conved.y - int(conved.y) > 0.5f)
			conved.y += 1;

		return {
			size_t(conved.x),
			size_t(conved.y)
		};
	}
}
#pragma once
#include "Vector2.h"
namespace ClosestGL::Math
{
	template<typename T>
	Math::Vector2<size_t> ConvertVertexPosToRenderTargetPos(
		T vertexPos,
		Math::Vector2<size_t> rtSize)
	{
		return {
			size_t(((vertexPos.x + 1) / 2) * rtSize.x),
			size_t((1 - (vertexPos.y + 1) / 2) * rtSize.y)
		};
	}
}
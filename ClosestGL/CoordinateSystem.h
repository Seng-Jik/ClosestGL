#pragma once
#include "Vector2.h"
namespace ClosestGL::Math
{
	template<typename T>
	Math::Vector2<size_t> ConvertVertexPosToRenderTargetPos(
		T vertexPos,
		Math::Vector2<size_t> rtSize)
	{
		Math::Vector2<size_t> ret;

		ret.x = static_cast<size_t>(rtSize.x * vertexPos.x);
		ret.y = static_cast<size_t>(rtSize.y * (1 - vertexPos.y));

		return ret;
	}
}
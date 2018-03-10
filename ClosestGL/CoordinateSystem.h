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

		vertexPos.x += 1;
		vertexPos.y += 1;
		vertexPos.x /= 2;
		vertexPos.y /= 2;
		vertexPos.y = 1 - vertexPos.y;


		ret.x = static_cast<size_t>(rtSize.x * vertexPos.x);
		ret.y = static_cast<size_t>(rtSize.y * (vertexPos.y));

		return ret;
	}
}
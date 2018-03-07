#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	inline std::vector<size_t> Generate(size_t vertexCount)
	{
		std::vector<size_t> ret(vertexCount);
		for (size_t i = 0; i < vertexCount; ++i)
			ret[i] = i;
		return ret;
	}
}

#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	/* Index Buffer 生成器 */

	/* 简单的为没有IndexBuffer的VertexBuffer生成一个顺序的IndexBuffer。
	 * count - 顶点数量
	 */
	inline std::vector<size_t> GenerateDefaultIndexBuffer(size_t vertexCount)
	{
		std::vector<size_t> ret(vertexCount);
		for (size_t i = 0; i < vertexCount; ++i)
			ret[i] = i;
		return ret;
	}
}

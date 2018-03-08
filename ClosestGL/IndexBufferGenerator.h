#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	/* Index Buffer ������ */

	/* �򵥵�Ϊû��IndexBuffer��VertexBuffer����һ��˳���IndexBuffer��
	 * count - ��������
	 */
	inline std::vector<size_t> GenerateDefaultIndexBuffer(size_t vertexCount)
	{
		std::vector<size_t> ret(vertexCount);
		for (size_t i = 0; i < vertexCount; ++i)
			ret[i] = i;
		return ret;
	}
}

#pragma once
#include <array>

namespace ClosestGL::Primitive
{
	/* CVV测试器
	 * 传入顶点缓存和图元以检查是否在CVV内
	 */
	template<typename TVertex,int TVertexPerPrimitive>
	bool CheckCVV(const TVertex* vertex, const std::array<size_t, TVertexPerPrimitive>& primitive)
	{
		int clipped = 0;
		for (int i = 0; i < TVertexPerPrimitive; ++i)
		{
			const auto& v = vertex[primitive[i]].SVPosition;
			auto w = v.w;

			if (v.z < 0 || v.z >  w || v.x < -w || v.x >  w || v.y < -w || v.y >  w)
			{
				clipped++;
			}

			if (w <= 0)
			{
				clipped = TVertexPerPrimitive;
				break;
			}
		}

		return clipped < TVertexPerPrimitive;

	}
}
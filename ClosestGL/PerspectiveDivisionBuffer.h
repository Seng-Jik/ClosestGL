#pragma once
#include <vector>
#include "Vector3.h"
#include "Vector4.h"

namespace ClosestGL::Primitive
{
	/* 透视除法缓存
	 *
	 * 缓存透视除法的结果
	 * 这个类仅用于实现新的光栅器，不应该被直接使用。
	 */
	template<typename TBaseType>
	class PerspectiveDivisionBuffer
	{
	private:
		std::vector<ClosestGL::Math::Vector2<TBaseType>> pbuf_;
	public:
		template<typename TVertex,typename TRunner>
		void Update(const TVertex* vb, size_t count, TRunner& runner)
		{
			pbuf_.resize(count);
			runner.Commit(0,count,[this,vb](auto i,auto)
			{
				pbuf_[i].x = vb[i].SVPosition.x / vb[i].SVPosition.w;
				pbuf_[i].y = vb[i].SVPosition.y / vb[i].SVPosition.w;
			});
		}

		template<typename TVertex,typename TRunner>
		PerspectiveDivisionBuffer(const TVertex* vb, size_t count, TRunner& runner)
		{
			Update(vb, count);
		}

		PerspectiveDivisionBuffer() {}

		inline auto AccessUnsafe(size_t i)
		{
			return pbuf_[i];
		}
	};
}
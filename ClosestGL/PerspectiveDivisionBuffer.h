#pragma once
#include <vector>
#include "Vector3.h"
#include "Vector4.h"

namespace ClosestGL::Primitive
{
	/* ͸�ӳ�������
	 *
	 * ����͸�ӳ����Ľ��
	 * ����������ʵ���µĹ�դ������Ӧ�ñ�ֱ��ʹ�á�
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
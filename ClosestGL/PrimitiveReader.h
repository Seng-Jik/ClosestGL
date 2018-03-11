#pragma once
#include <array>
namespace ClosestGL::Primitive
{
	/* ֱͨͼԪ�Ķ���
	 * �����е�ͼԪ�б�ֱ�Ӷ�ȡͼԪ
	 * ģ�������
	 *       TPrimitive - ����һ��ͼԪ�Ķ�����Read���ն�����std::array<size_t,N>ͼԪ����
	 */
	template<typename TPrimitive>
	class PrimitiveReader
	{
	private:
		const TPrimitive* const primitives_;
		size_t offset_;
		const size_t size_;

	public:
		PrimitiveReader(const TPrimitive* primitives,size_t size):
			size_{ size },
			offset_{0},
			primitives_{primitives}
		{}


		static constexpr int GetVertexPerPrimitive() { return std::tuple_size<TPrimitive>::value; }
		inline void Reset()
		{
			offset_ = 0;
		}

		auto Read()
		{
			return primitives_[offset_++];
		}

		inline bool CanRead()
		{
			return offset_ < size_;
		}
	};
}
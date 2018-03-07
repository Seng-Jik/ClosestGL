#pragma once
#pragma once
#include <array>

namespace ClosestGL::Primitive
{
	/* ͼԪStrip�Ķ��� */

	/* ͼԪStrip�Ķ�����Strip�ķ�ʽ��IndexBuffer�Ķ�ͼԪ��*/

	/* ����ͼԪ�Ķ���������μ�PrimitiveListReader */

	template<int VertexPerPrimitive>
	class PrimitiveStripReader
	{
	private:
		size_t* indexBuffer_;
		size_t size_;
		size_t offset_;
	public:
		static constexpr int GetVertexPerPrimitive() { return VertexPerPrimitive; }

		inline PrimitiveStripReader(size_t* indexBuffer, size_t size) :
			indexBuffer_{ indexBuffer },
			size_{ size },
			offset_{ VertexPerPrimitive - 1 }
		{}

		PrimitiveStripReader(const PrimitiveStripReader&) = delete;

		inline void Reset()
		{
			offset_ = VertexPerPrimitive - 1;
		}

		inline std::array<size_t, VertexPerPrimitive> Read()
		{
			std::array<size_t, VertexPerPrimitive> ret;

			//��ȡǰ��Ķ���
			for (int i = 0; i < VertexPerPrimitive; ++i)
				ret[VertexPerPrimitive - i - 1] = indexBuffer_[offset_ - i];

			offset_++;
			return ret;
		}

		inline bool CanRead()
		{
			return offset_ < size_;
		}
	};
}

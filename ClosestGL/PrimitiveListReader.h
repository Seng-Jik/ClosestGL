#pragma once
#include <array>

namespace ClosestGL::Primitive
{
	template<int VertexPerPrimitive>
	class PrimitiveListReader
	{
	private:
		size_t* indexBuffer_;
		size_t size_;
		size_t offset_;
	public:
		static constexpr int GetVertexPerPrimitive() { return VertexPerPrimitive; }

		inline PrimitiveListReader(size_t* indexBuffer, size_t size):
			indexBuffer_{ indexBuffer },
			size_{ size },
			offset_{0}
		{}

		PrimitiveListReader(const PrimitiveListReader&) = delete;

		inline void Reset()
		{
			offset_ = 0;
		}

		inline std::array<size_t,VertexPerPrimitive> Read()
		{
			std::array<size_t,VertexPerPrimitive> ret;

			for (int i = 0; i < VertexPerPrimitive; ++i)
				ret[i] = indexBuffer_[offset_++];

			return ret;
		}

		inline bool CanRead()
		{
			return offset_ < size_ - (VertexPerPrimitive - 1);
		}
	};
}

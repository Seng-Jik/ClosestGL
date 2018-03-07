#pragma once
#include <array>

namespace ClosestGL::Primitive
{

	/* 图元列表阅读器 */

	/* 图元列表阅读器将会以List的形式读取图元 */

	/* 关于图元阅读器 
	 * 图元阅读器的模板参数VertexPerPrimitive，指示每多少个顶点构成一个图元。
	 * 图元阅读器用于从IndexBuffer取出图元。
	 * 对于列表式的IndexBuffer，需要使用图元列表阅读器。
	 * 对于Strip式的IndexBuffer，则需要使用图元Strip阅读器。
	 * 图元阅读器将会逐个从IndexBuffer里取出单个图元中的所有顶点。
	 * 图元阅读器的构造函数传入indexBuffer和它的大小，以构造一个图元阅读器。
	 * 使用Reset函数重置状态以从头重新读取图元。
	 * CanRead函数用来查询是否可以继续阅读图元。
	 * Read函数用于读取一个图元，如果调用Read时CanRead返回值为false，则未定义。
	 */

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

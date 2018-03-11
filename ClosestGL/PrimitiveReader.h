#pragma once
#include <array>
namespace ClosestGL::Primitive
{
	/* 直通图元阅读器
	 * 从已有的图元列表直接读取图元
	 * 模板参数：
	 *       TPrimitive - 从上一个图元阅读器的Read接收而来的std::array<size_t,N>图元类型
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
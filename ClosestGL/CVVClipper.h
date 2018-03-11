#pragma once
#include <utility>
#include <optional>

namespace ClosestGL::Primitive
{
	/* CVV剪裁器 
	 *
	 * 套在图元阅读器上使用
	 * 从图元阅读器的输出上过滤出通过剪裁的图元。
	 * 模板参数：
	 *        TPrimitiveReader - 原始类型的图元阅读器
	 *        TVertex          - 顶点数据类型
	 * 构造函数特殊说明：
	 *     构造函数传入的参数为顶点缓存和TPrimitiveReader的构造函数参数。
	 */
	template<typename TPrimitiveReader,typename TVertex>
	class CVVClipper
	{
	private:
		TPrimitiveReader* const reader_;
		const TVertex* const vb_;

	public:
		static constexpr auto GetVertexPerPrimitive() { return TPrimitiveReader::GetVertexPerPrimitive(); }
		using Primitive = decltype(reader_->Read());
		
	private:
		std::optional<Primitive> next_;

		void PrepareNextPrimitive()
		{
			next_.reset();

			while (reader_->CanRead())
			{
				auto primitive = reader_->Read();

				int clipped = 0;
				for (int i = 0; i < GetVertexPerPrimitive(); ++i)
				{
					const auto& v = vb_[primitive[i]].SVPosition;
					auto w = v.w;

					if (v.z < 0 || v.z >  w || v.x < -w || v.x >  w || v.y < -w || v.y >  w)
					{
						clipped++;
					}

					if (w <= 0)
					{
						clipped = GetVertexPerPrimitive();
						break;
					}
				}

				if (clipped < GetVertexPerPrimitive())
				{
					next_ = std::move(primitive);
					break;
				}
			}
		}

	public:
		CVVClipper(const TVertex* vertexBuffer, TPrimitiveReader* reader):
			reader_ { reader },
			vb_{ vertexBuffer }
		{
			PrepareNextPrimitive();
		}

		

		inline void Reset()
		{
			reader_->Reset();
			PrepareNextPrimitive();
		}

		inline auto Read()
		{
			auto ret = std::move(*next_);
			PrepareNextPrimitive();
			return ret;
		}

		inline bool CanRead()
		{
			return next_.has_value();
		}

		void WriteToVertexList(std::vector<Primitive>& output)
		{
			while (CanRead())
			{
				output.push_back(Read());
			}
		}
	};
}
#pragma once
#include <utility>
#include "Vector2.h"
#include "VectorCommon.h"
#include "CoordinateSystem.h"

namespace ClosestGL::RenderPipeline
{
	/* 直线光栅器 
	 * 用于光栅化直线
	 * 模板参数：
	 *     TNextStage - 管线下个阶段的类
	 *     TLerpType  - 插值类型（可以使用double或者float，取决于你的顶点使用的类型）
	 * 公开成员函数：
	 *     构造函数
	 *         nextStage - 下个阶段的类对象的指针，该对象生命周期必须大于等于本对象。
	 *     EmitPrimitive - 提交一组图元
	 *         primitiveReader - 提交的图元阅读器，该阅读器会被直接阅读到结束，再次使用需要Reset
	 *             关于图元阅读器，参见Primitive::PrimitiveListReader或PrimitiveStripReader
	 *         vbo             - 指向VertexBuffer的指针
	 *         runner          - 执行器，参阅命名空间ParallelStrategy
	 */
	template<typename TNextStage,typename TLerpType>
	class LineRasterizer
	{
	private:
		TNextStage* nextStage_;

		template<typename TVertexAttributes,typename TRunner>
		void DrawLine(
			const TVertexAttributes& v1,
			const TVertexAttributes& v2, 
			const Math::Vector2<size_t>& rtSize,
			size_t rtSizeLength, 
			TRunner& runner)
		{
			const auto stepCount = static_cast<size_t>
				(Math::Distance(v1.SVPosition, v2.SVPosition) * rtSizeLength) + 1;

			const auto stepCountLerp = static_cast<TLerpType>(stepCount);

			auto nextStage = nextStage_;

			runner.Commit(0,stepCount,
				[v1,v2, stepCountLerp, rtSize, nextStage](size_t step, auto)
			{
				const auto lerper = step / stepCountLerp;
				const auto p = TVertexAttributes::Lerp(lerper, v1, v2);

				const auto pos = 
					Math::ConvertVertexPosToRenderTargetPos(p.SVPosition, rtSize);

				if (pos.x < 0) return;
				if (pos.y < 0) return;
				if (pos.x >= rtSize.x) return;
				if (pos.y >= rtSize.y) return;
				nextStage->EmitPixel(p, pos);
			});
		}

	public:
		LineRasterizer(TNextStage* nextStage):
			nextStage_(nextStage)
		{}

		template<
			typename TPrimitiveReader,
			typename TVertexBuffer,
			typename TRunner>

		void EmitPrimitive(
			TPrimitiveReader& primitiveReader, 
			TVertexBuffer* vbo,
			TRunner& runner)
		{
			static_assert(primitiveReader.GetVertexPerPrimitive() == 2);

			const auto rtSize = nextStage_->GetRenderTargetSize();
			const auto rtSizeLength = rtSize.Length();

			while (primitiveReader.CanRead())
			{
				auto ps = primitiveReader.Read();
				DrawLine<std::decay<TVertexBuffer>::type,TRunner>
					(vbo[ps[0]], vbo[ps[1]], rtSize, rtSizeLength, runner);
			}
		}
	};
}
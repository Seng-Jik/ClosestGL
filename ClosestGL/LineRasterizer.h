#pragma once
#include <utility>
#include <PerspectiveDivisionBuffer.h>
#include "Vector2.h"
#include "VectorCommon.h"
#include "CoordinateSystem.h"
#include "CVVClipper.h"
#include "LineRasterizer.h"

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
		TNextStage* const nextStage_;

		Primitive::PerspectiveDivisionBuffer<TLerpType> pbuf_;

		template<typename TVertexAttributes>
		void DrawLine(
			const TVertexAttributes& v1,
			const TVertexAttributes& v2,
			size_t i1,size_t i2,
			const Math::Vector2<size_t>& rtSize,
			size_t rtSizeLength)
		{
			const auto pdpos1 = pbuf_.AccessUnsafe(i1);
			const auto pdpos2 = pbuf_.AccessUnsafe(i2);
			const auto stepCount = static_cast<size_t>
				(Math::Distance(pdpos1, pdpos2) * rtSizeLength) + 1;

			const auto stepCountLerp = static_cast<TLerpType>(stepCount);

			auto nextStage = nextStage_;

			for(size_t step = 0; step < stepCount;++step)
			{
				const auto lerper = step / stepCountLerp;
				const auto pdpos = ClosestGL::Math::Lerp(lerper, pdpos1, pdpos2);

				const auto pos = 
					Math::ConvertVertexPosToRenderTargetPos(pdpos, rtSize);

				if (pos.x < 0) continue;
				if (pos.y < 0) continue;
				if (pos.x >= rtSize.x) continue;
				if (pos.y >= rtSize.y) continue;

				const auto p = TVertexAttributes::Lerp(lerper, v1, v2);
				nextStage->EmitPixel(p, pos);
			}
		}

	public:
		LineRasterizer(TNextStage* nextStage):
			nextStage_(nextStage)
		{}

		template<
			typename TPrimitiveReader,
			typename TVertex,
			typename TRunner>

		void EmitPrimitive(
			TPrimitiveReader& primitiveReader, 
			const TVertex* vbo,
			size_t vertexCount,
			TRunner& runner)
		{
			static_assert(primitiveReader.GetVertexPerPrimitive() == 2);

			pbuf_.Update(vbo, vertexCount,runner);

			const auto rtSize = nextStage_->GetRenderTargetSize();
			const auto rtSizeLength = rtSize.Length();

			runner.Wait();

			while (primitiveReader.CanRead())
			{
				auto ps = primitiveReader.Read();

				if (Primitive::CheckCVV(vbo, ps)) 
				{
					runner.Commit([vbo, ps, rtSize, rtSizeLength, this] {
						DrawLine(vbo[ps[0]], vbo[ps[1]], ps[0], ps[1], rtSize, rtSizeLength);
					});
				}
			}
		}
	};
}
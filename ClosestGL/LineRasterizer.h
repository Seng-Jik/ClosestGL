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
	/* ֱ�߹�դ�� 
	 * ���ڹ�դ��ֱ��
	 * ģ�������
	 *     TNextStage - �����¸��׶ε���
	 *     TLerpType  - ��ֵ���ͣ�����ʹ��double����float��ȡ������Ķ���ʹ�õ����ͣ�
	 * ������Ա������
	 *     ���캯��
	 *         nextStage - �¸��׶ε�������ָ�룬�ö����������ڱ�����ڵ��ڱ�����
	 *     EmitPrimitive - �ύһ��ͼԪ
	 *         primitiveReader - �ύ��ͼԪ�Ķ��������Ķ����ᱻֱ���Ķ����������ٴ�ʹ����ҪReset
	 *             ����ͼԪ�Ķ������μ�Primitive::PrimitiveListReader��PrimitiveStripReader
	 *         vbo             - ָ��VertexBuffer��ָ��
	 *         runner          - ִ���������������ռ�ParallelStrategy
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
				const auto p = TVertexAttributes::Lerp(lerper, v1, v2);
				const auto pdpos = ClosestGL::Math::Lerp(lerper, pdpos1, pdpos2);

				const auto pos = 
					Math::ConvertVertexPosToRenderTargetPos(pdpos, rtSize);

				if (pos.x < 0) continue;
				if (pos.y < 0) continue;
				if (pos.x >= rtSize.x) continue;
				if (pos.y >= rtSize.y) continue;
				nextStage->EmitPixel(p, pos);
			}
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
			const TVertexBuffer* vbo,
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

				if (!Primitive::CheckCVV(vbo, ps)) continue;

				runner.Commit([vbo,ps,rtSize, rtSizeLength,this] {
					DrawLine(vbo[ps[0]], vbo[ps[1]], ps[0],ps[1], rtSize, rtSizeLength);
				});
			}
		}
	};
}
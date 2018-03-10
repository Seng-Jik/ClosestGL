#pragma once
#include <utility>
#include "Vector2.h"
#include "VectorCommon.h"
#include "CoordinateSystem.h"

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
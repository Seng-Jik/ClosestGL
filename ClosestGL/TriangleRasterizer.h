#pragma once
#include <algorithm>
#include "MathCommon.h"
#include "PerspectiveDivisionBuffer.h"
#include "CVVClipper.h"
#include "CoordinateSystem.h"
#include "Vector2.h"
namespace ClosestGL::RenderPipeline
{
	template<typename TNextStage, typename TLerpType>
	class TriangleRasterizer
	{
	private:
		TNextStage * const nextStage_;

		Primitive::PerspectiveDivisionBuffer<TLerpType> pbuf_;

		template<typename TVertex>
		void DrawScanLine(
			const TVertex& x1,size_t x1r,
			const TVertex& x2,size_t x2r,
			size_t y,size_t rtWidth
		)
		{
			const int xBegin = int(x1r), xEnd = int(x2r);
			const int xInc = (xEnd - xBegin) > 0 ? 1 : -1;
			for (int x = xBegin; x != xEnd; x += xInc)
			{
				if (x < 0) continue;
				if (x > rtWidth) continue;
				const TLerpType xLerp = TLerpType(x - xBegin) / TLerpType(xEnd - xBegin);
				const Math::Vector2<size_t> rpos{ size_t(x),y };
				const auto vertex = TVertex::Lerp(xLerp, x1, x2);
				nextStage_->EmitPixel(vertex, rpos);
			}
		}


		template<typename TVertex>
		void DrawHalfTriangle(
			const TVertex& hat, const Math::Vector2<TLerpType>& hatp,
			const TVertex& foot1, const Math::Vector2<TLerpType>& foot1p,
			const TVertex& foot2, const Math::Vector2<TLerpType>& foot2p)
		{
			const auto& rtSize = nextStage_->GetRenderTargetSize();

			const auto hatr = Math::ConvertVertexPosToRenderTargetPos(hatp, rtSize);
			const auto foot1r = Math::ConvertVertexPosToRenderTargetPos(foot1p, rtSize);
			const auto foot2r = Math::ConvertVertexPosToRenderTargetPos(foot2p, rtSize);

			const int yBegin = int(hatr.y), yEnd = int(foot1r.y);
			const int yInc = (yEnd - yBegin) > 0 ? 1 : -1;

			for (int y = yBegin; y != yEnd; y += yInc)
			{
				if (y < 0 || y >= rtSize.y) continue;

				const TLerpType yLerp = TLerpType(y - yBegin) / TLerpType(yEnd - yBegin);

				const auto x1 = TVertex::Lerp(yLerp, hat, foot1);
				const auto x2 = TVertex::Lerp(yLerp, hat, foot2);
				const auto x1p = Math::Lerp(yLerp, hatp, foot1p);
				const auto x2p = Math::Lerp(yLerp, hatp, foot2p);

				const auto x1r = Math::ConvertVertexPosToRenderTargetPos(x1p, rtSize);
				const auto x2r = Math::ConvertVertexPosToRenderTargetPos(x2p, rtSize);

				DrawScanLine(x1,x1r.x, x2,x2r.x, size_t(y),rtSize.x);
			}
		}

	public:
		TriangleRasterizer(TNextStage* nextStage) :
			nextStage_(nextStage)
		{}

		template<
			typename TPrimitiveReader,
			typename TVertex,
			typename TRunner>

			void EmitPrimitive(
				TPrimitiveReader& primitiveReader,
				const TVertex* vb,
				size_t vertexCount,
				TRunner& runner)
		{
			static_assert(primitiveReader.GetVertexPerPrimitive() == 3);
			pbuf_.Update(vb, vertexCount,runner);

			runner.Wait();
			const auto& rtSize = nextStage_->GetRenderTargetSize();

			while (primitiveReader.CanRead())
			{
				auto tri = primitiveReader.Read();
				if (Primitive::CheckCVV(vb,tri))
				{
					std::sort(tri.begin(), tri.end(), 
						[this](size_t p1,size_t p2) {
						return pbuf_.AccessUnsafe(p1).y < pbuf_.AccessUnsafe(p2).y;
					});

					const auto splitLerp =
						1-((pbuf_.AccessUnsafe(tri[1]).y - pbuf_.AccessUnsafe(tri[0]).y) /
						(pbuf_.AccessUnsafe(tri[2]).y - pbuf_.AccessUnsafe(tri[0]).y));

					const auto splitVertex =
						TVertex::Lerp(splitLerp, vb[tri[2]], vb[tri[0]]);

				

					const auto splitVertexPerspectived = Math::Lerp(splitLerp, pbuf_.AccessUnsafe(tri[2]), pbuf_.AccessUnsafe(tri[0]));

					const auto splitVertexRPos =
						Math::ConvertVertexPosToRenderTargetPos(splitVertexPerspectived, rtSize);

					const auto middleRpos = 
						Math::ConvertVertexPosToRenderTargetPos(pbuf_.AccessUnsafe(tri[1]), rtSize);

					if (splitVertexRPos.y > 0 && splitVertexRPos.y < rtSize.y)
					{
						DrawScanLine(
							splitVertex, splitVertexRPos.x,
							vb[tri[1]], middleRpos.x,
							splitVertexRPos.y, rtSize.x);
					}

					DrawHalfTriangle(
						vb[tri[0]], pbuf_.AccessUnsafe(tri[0]),
						vb[tri[1]], pbuf_.AccessUnsafe(tri[1]),
						splitVertex, splitVertexPerspectived);

					DrawHalfTriangle(
						vb[tri[2]], pbuf_.AccessUnsafe(tri[2]),
						vb[tri[1]], pbuf_.AccessUnsafe(tri[1]),
						splitVertex, splitVertexPerspectived);
				}
			}
		}
	};
}
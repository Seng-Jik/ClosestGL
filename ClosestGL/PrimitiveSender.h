#pragma once
#include <vector>
#include "Transformers.h"
#include "CVVClipper.h"
#include "PrimitiveReader.h"
namespace ClosestGL::RenderPipeline
{
	/* BUG! */
	template<typename TVertex,typename TPrimitiveReader>
	class PrimitiveSender
	{
	private:
		std::vector<TVertex> vb_;
		ClosestGL::Primitive::CVVClipper<TPrimitiveReader, TVertex> reader_;

		using Primitive = std::array<size_t, TPrimitiveReader::GetVertexPerPrimitive()>;
		std::vector<Primitive> ibClipped_;	// Cache

	public:
		PrimitiveSender(TPrimitiveReader* reader,size_t vbSize = 0):
			vb_{ vbSize },
			reader_{ vb_.data(),reader }
		{
			ibClipped_.reserve(100000);
		}

		inline std::vector<TVertex>& GetVertexBuffer()
		{
			return vb_;
		}
		
		template<typename TParalelStrategy,typename TPrimitiveReader, typename TRasterizer>
		void Send(TPrimitiveReader& reader, TRasterizer rasterizer,TParalelStrategy& runner)
		{
			//CVVClip
			{
				ibClipped_.clear();
				reader_.Reset();
				reader_.WriteToVertexList(ibClipped_);
			}

			//PerspectiveDivision
			{
				ClosestGL::Primitive::PlaceTransform(
					[](auto& v)
				{
					v.SVPosition /= v.SVPosition.w;
				}, vb_.data(),vb_.size(),runner);
			}

			//SendToRasterizer
			{
				runner.Wait();
				ClosestGL::Primitive::PrimitiveReader<Primitive>
					reader{ ibClipped_.data(),ibClipped_.size() };
				rasterizer.EmitPrimitive(reader, vb_.data(), runner);
			}
		}
	};
}
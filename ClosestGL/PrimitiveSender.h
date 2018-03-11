#pragma once
#include <vector>
#include "Transformers.h"
#include "CVVClipper.h"
#include "PrimitiveReader.h"
namespace ClosestGL::RenderPipeline
{
	template<typename TVertex,typename TPrimitiveReader>
	class PrimitiveSender
	{
	private:
		std::vector<TVertex> vb_;
		std::vector<decltype(TPrimitiveReader::Read())> ibClipped_;	// Cache

	public:
		std::vector<TVertex>& GetVertexBuffer()
		{
			return vb_;
		}
		
		template<typename TParalelStrategy,typename TPrimitiveReader, typename TRasterizer>
		void Send(TPrimitiveReader& reader, TRasterizer rasterizer,TParalelStrategy& runner)
		{
			//CVVClip
			{
				ibClipped_.clear();
				Primitive::CVVClipper<TPrimitiveReader, TVertex>
					cvvCliper{ vb_.data(),&reader };

				cvvCliper.WriteToVertexList(ibClipped_);
			}

			//PerspectiveDivision
			{
				Primitive::PlaceTransform(
					[](){auto& vertex}
				{
					auto w = vertex.SVPosition.w;
					vertex.SVPosition /= w;
					vertex.SVPosition.w = w;
				}, vb_.data(),vb.size(),runner);
			}

			//SendToRasterizer
			{
				Primitive::PrimitiveReader<decltype(TPrimitiveReader::Read() >
					reader(ibClipped_);
				rasterizer.EmitPrimitive(reader, vb_.data(), runner);
			}
		}
	};
}
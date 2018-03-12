#include "stdafx.h"
#include <PrimitiveListReader.h>
#include <SingleThreadRunner.h>
#include <TriangleRasterizer.h>
using namespace ClosestGL;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(TriangleRasterizerTest)
	{
		TEST_METHOD(TestBasicllTriangle)
		{
			struct Vertex
			{
				Math::Vector4<float> SVPosition;

				static Vertex Lerp(float x, const Vertex& p1, const Vertex& p2)
				{
					return {
						Math::Lerp(x,p1.SVPosition,p2.SVPosition)
					};
				}
			};

			const Vertex mesh[] = 
			{
				{ { 0,0.5f,0,1 } },
				{ { -0.9f,0.75f,0,1 } },
				{ { 0.5,-0.9f,0,1 } }
			};

			const size_t indicis[] = { 0,1,2 };

			Primitive::PrimitiveListReader<3> preader{ indicis,3 };

			Tools::TestTex fb{ {800,600} };
			
			RenderPipeline::RenderTarget<1, Tools::TestCol, decltype(Tools::Blenders::NoBlend)>
				renderTarget{ Tools::Blenders::NoBlend,{&fb} };

			const auto PixelShader = [](const auto&)
			{
				return std::array<Tools::TestCol, 1> { Tools::TestCol{ 1,1,1,1 }};
			};

			RenderPipeline::PixelShader<decltype(renderTarget), decltype(PixelShader)>
				ps{ &renderTarget,PixelShader };

			RenderPipeline::TriangleRasterizer<decltype(ps), float> raster{ &ps };

			raster.EmitPrimitive(preader, mesh, 3, ParallelStrategy::SingleThreadRunner{});

			Tools::ViewSurface(fb);
		}
	};
}

#include "stdafx.h"
#include <MathCommon.h>
#include <RenderTarget.h>
#include <PixelShaderStage.h>
#include <LineRasterizer.h>
#include <PrimitiveListReader.h>
#include <MultiThreadRunner.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;
using namespace std::chrono_literals;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(LineRasterizerTest)
	{
	private:
		struct Vertex
		{
			Math::Vector4<float> SVPosition;
			Math::Vector4<float> Color;

			template<typename TLerpType>
			static Vertex Lerp(TLerpType x, const Vertex& p1, const Vertex& p2)
			{
				return Vertex
				{
					Math::Lerp(x,p1.SVPosition,p2.SVPosition),
					Math::Lerp(x,p1.Color,p2.Color)
				};
			}
		};

	public:
		TEST_METHOD(TestDrawLine)
		{
			const auto PS = [](const Vertex& psi,const ClosestGL::Math::Vector2<size_t>&)
			{
				std::this_thread::sleep_for(1ms);
				std::array<Math::Vector4<float>, 1> ret{ psi.Color };
				return ret;
			};

			Tools::TestTex tex{ {800,600} };
			RenderPipeline::RenderTarget
				<1, Tools::TestCol, decltype(Tools::Blenders::NoBlend)>
				rt(Tools::Blenders::NoBlend, { &tex });

			RenderPipeline::PixelShaderStage<decltype(rt), decltype(PS)>
				ps(&rt, PS);

			RenderPipeline::LineRasterizer<decltype(ps),float> lr(&ps);

			const Vertex vertex[] = 
			{
				{{1,0,0,1},{1,0,0,1}},
				{{0,1,0,1},{0,1,0,1}},
				{{0,0.5f,0,1},{0,0,1,0}}
			};
			
			const std::vector<size_t> ibo = { 0,1,0,2 };

			Primitive::PrimitiveListReader<2> preader(ibo.data(),ibo.size());

			ParallelStrategy::MultiThreadRunner 
				sr(std::thread::hardware_concurrency());

			lr.EmitPrimitive(preader, vertex,3, sr);

			Tools::ViewSurface(tex,4000);
		}
	};
}

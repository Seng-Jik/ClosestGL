#include "stdafx.h"
#include <RenderTarget.h>
#include <MultiThreadRunner.h>
#include <VectorCommon.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;
using namespace ClosestGL::Math;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(RenderTargetTest)
	{
	public:
		TEST_METHOD(TestMRT)
		{
			const auto NoBlend = [](Tools::TestCol src, auto) { return src; };
			ClosestGL::RenderPipeline::RenderTarget<4, Tools::TestCol, decltype(NoBlend)>
				fb
			{
				NoBlend,
				ClosestGL::Math::Vector2<std::size_t> { 1024,768 }
			};

			ClosestGL::ParallelStrategy::MultiThreadRunner
				runner(std::thread::hardware_concurrency());

			auto size = fb.GetSize();
			runner.Commit(0, size.y,
				[&fb, size](auto y, auto)
			{
				for (size_t x = 0; x < size.x; ++x)
				{
					std::array<Tools::TestCol, 4> cols;

					bool isBlack = false;
					if (x % 16 < 8) isBlack = !isBlack;
					if (y % 16 < 8) isBlack = !isBlack;

					cols[0] = isBlack ? Tools::TestCol{ 0, 0, 0, 0 } : Tools::TestCol{ 1, 1, 1, 1 };

					cols[1] = Distance(Vector2<size_t>{x, y}, Vector2<size_t>{ 0, 0 }) < 200 ?
						Tools::TestCol{ 1, 0, 0, 0 } : Tools::TestCol{ 0, 0, 0, 0 };

					cols[2] = Distance(Vector2<size_t>{x, y}, Vector2<size_t>{ 1024, 768 }) < 400 ?
						Tools::TestCol{ 0, 1, 0, 0 } : Tools::TestCol{ 0, 0, 0, 0 };

					cols[3] = Distance(Vector2<size_t>{x, y}, Vector2<size_t>{ 1024, 0 }) < 300 ?
						Tools::TestCol{ 1, 1, 0, 0 } : Tools::TestCol{ 0, 0, 0, 0 };

					fb.EmitPixel(cols, { x,y });
				}
			});

			auto SimpleBlender = [](auto src, auto dst) {
				return (src + dst) / 2.0f;
			};

			RenderPipeline::RenderTarget<1, Tools::TestCol, decltype(SimpleBlender)>
				fb2{ SimpleBlender,ClosestGL::Math::Vector2<std::size_t> { 1024,768 } };

			runner.Wait();

			runner.Commit(0, size.y,
				[&fb, &fb2, size](auto y, auto)
			{
				for (size_t x = 0; x < size.x; ++x)
				{
					const Vector2<size_t> pos{ x,y };

					for (int i = 0; i < fb.GetColorBufferCount(); ++i)
					{
						std::array<Tools::TestCol, 1> emit{ fb.GetTexture(i).AccessPixel(pos) };
						fb2.EmitPixel(emit, pos);
					}
				}
			});
			
			runner.Wait();

			Tools::ViewSurface(fb2.GetTexture(0));
		}
	};
}

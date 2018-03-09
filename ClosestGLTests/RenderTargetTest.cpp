#include "stdafx.h"
#include <RenderTarget.h>
#include <MultiThreadRunner.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(RenderTargetTest)
	{
	public:
		TEST_METHOD(TestMRT)
		{
			const auto NoBlend = [](std::uint8_t src, auto) { return src; };
			ClosestGL::RenderPipeline::RenderTarget<4, ClosestGL::Math::Vector4<uint8_t>, decltype(NoBlend)>
				fb
			{
				NoBlend,
				ClosestGL::Math::Vector2<std::size_t> { 1024,768 }
			};

			ClosestGL::ParallelStrategy::MultiThreadRunner 
				runner(std::thread::hardware_concurrency());

			auto size = fb.GetSize();
			runner.Commit(0,size.y,
				[&fb,size](auto y, auto)
			{
				for (size_t x = 0; x < size.x; ++x)
				{
					bool isBlack = false;
					if (x % 16 < 8) isBlack = !isBlack;
					if (x % 16 < 8) isBlack = !isBlack;
				}
			});
		}
	};
}

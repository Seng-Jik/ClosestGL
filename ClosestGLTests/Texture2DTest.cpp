#include "stdafx.h"
#include <Vector4.h>
#include <Texture2D.h>
#include <MultiThreadRunner.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Texture;
using namespace ClosestGL::Math;
using namespace std::chrono_literals;

namespace ClosestGLTests::TextureTest
{
	TEST_CLASS(Texture2DTest)
	{
	public:
		TEST_METHOD(TestTexture2D)
		{
			Tools::TestTex tex { {640, 480} };
			auto s = tex.GetSize();
			tex.Data();
			Assert::IsTrue(s == Vector2<size_t>{640, 480});

			{
				ClosestGL::ParallelStrategy::MultiThreadRunner 
					runner(std::thread::hardware_concurrency());
				tex.Shade(
					[](ClosestGL::Math::Vector2<size_t> pos)
					{
						auto r = pos.x / 640.0f;
						auto g = pos.y / 480.0f;
						return Tools::TestCol{ r,g,0 };
					},
					runner
				);

				Tools::ViewSurface(tex,1000);
			}
		}

		TEST_METHOD(TestTexture2DClear)
		{
			Tools::TestTex tex{ { 640, 480 } };
			auto s = tex.GetSize();
			tex.Data();
			Assert::IsTrue(s == Vector2<size_t>{640, 480});

			{
				ClosestGL::ParallelStrategy::MultiThreadRunner
					runner(std::thread::hardware_concurrency());

				tex.Clear(Tools::TestCol{ 1, 0, 0, 1 }, runner);

				Tools::ViewSurface(tex, 1000);
			}
		}
	};
}

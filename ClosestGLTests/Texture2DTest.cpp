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
			Texture2D<Vector4<uint8_t>> tex { {640, 480} };
			auto s = tex.GetSize();
			tex.Data();
			Assert::IsTrue(s == Vector2<size_t>{640, 480});

			{
				ClosestGL::ParallelStrategy::MultiThreadRunner 
					runner(std::thread::hardware_concurrency());
				tex.Shade(
					[](ClosestGL::Math::Vector2<size_t> pos)
					{
						auto r = pos.x / 640.0;
						auto g = pos.y / 480.0;
						return Vector4<uint8_t>
						{ uint8_t(r * 255), uint8_t(g * 255), 0, 255 };
					},
					runner
				);

				Tools::ViewSurface(tex,1000);
			}
		}

		TEST_METHOD(TestTexture2DClear)
		{
			Texture2D<Vector4<uint8_t>> tex{ { 640, 480 } };
			auto s = tex.GetSize();
			tex.Data();
			Assert::IsTrue(s == Vector2<size_t>{640, 480});

			{
				ClosestGL::ParallelStrategy::MultiThreadRunner
					runner(std::thread::hardware_concurrency());

				tex.Clear(Vector4<uint8_t>{ 255, 0, 0, 255 }, runner);

				Tools::ViewSurface(tex, 1000);
			}
		}
	};
}

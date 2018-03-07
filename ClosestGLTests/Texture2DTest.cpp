#include "stdafx.h"
#include "CppUnitTest.h"
#include <Vector4.h>
#include <Texture2D.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Texture;
using namespace ClosestGL::Math;

namespace ClosestGLTests::Texture2DTest
{
	TEST_CLASS(Texture2DTest)
	{
	public:
		TEST_METHOD(TestTexture2D)
		{
			Texture2D<Vector4<uint8_t>> tex { {1024, 768} };
			auto s = tex.GetSize();
			tex.Data();
			Assert::IsTrue(s == Vector2<size_t>{ 1024,768 });
		}
	};
}

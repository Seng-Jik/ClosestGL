#include "stdafx.h"
#include "CppUnitTest.h"

#include <MatrixTransform.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Math;

namespace ClosestGLTests::MathTest
{
	TEST_CLASS(TransformTest)
	{
	public:
		TEST_METHOD(TestScale)
		{
			{
				constexpr Vector2<int> vec{ 1, 2 };
				constexpr auto tsf = GetScaleMatrix(2, 1);
				constexpr auto result = tsf * vec;
				static_assert(result.x == 2);
				static_assert(result.y == 2);
			}

			{
				constexpr Vector3<int> vec{ 1, 2, 3 };
				constexpr auto tsf = GetScaleMatrix(6, 3, 2);
				constexpr auto result = tsf * vec;

				static_assert(result.x == 6);
				static_assert(result.y == 6);
				static_assert(result.z == 6);
			}

			{
				constexpr Vector4<int> vec{ 1, 2, 3, 4 };
				constexpr auto tsf = GetScaleMatrix(12, 6, 4, 3);
				constexpr auto result = tsf * vec;

				static_assert(result.x == 12);
				static_assert(result.y == 12);
				static_assert(result.z == 12);
				static_assert(result.w == 12);
			}
		}

		TEST_METHOD(TestTransform)
		{
			{
				constexpr Vector3<int> vec{ 0,0,1 };
				constexpr auto tsf = GetTransformMatrix(Vector2<int>{10, 15});
				constexpr auto result = tsf * vec;

				static_assert(result.x == 10);
				static_assert(result.y == 15);
				static_assert(result.z == 1);
			}

			{
				constexpr Vector4<int> vec{ 0,0,0,1 };
				constexpr auto tsf = GetTransformMatrix(Vector3<int>{10, 15, 20});
				constexpr auto result = tsf * vec;

				static_assert(result.x == 10);
				static_assert(result.y == 15);
				static_assert(result.z == 20);
				static_assert(result.w == 1);
			}
		}
	};
}


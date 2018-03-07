#include "stdafx.h"
#include "CppUnitTest.h"

#include <MatrixTransform.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Math;

namespace ClosestGLTests::MathTest
{
	TEST_CLASS(MatrixTransformTest)
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

		TEST_METHOD(TestRotateZ)
		{
			constexpr Vector2<float> v{1, 0};
			const Vector2<float> vr = GetZRotateMatrix(3.1415926f / 2.0f) * v;

			constexpr Vector3<double> v2{ 1, 0, 1 };
			const Vector3<double> vr2 = 
				Matrix3<double>(GetZRotateMatrix(3.1415926 / 2.0)) * v2;
		}

		TEST_METHOD(TestRotateY)
		{
			constexpr Vector3<float> v{ 1,1,0 };
			const Vector3<float> vr = GetYRotateMatrix(3.1415926f) * v;

			constexpr Vector4<double> v2{ 1,1,0,0 };
			const Vector4<double> vr2 = 
				Matrix4<double>(GetYRotateMatrix(3.1415926)) * v2;
		}

		TEST_METHOD(TestRotateX)
		{
			constexpr Vector3<float> v{ 1,1,0 };
			const Vector3<float> vr = GetXRotateMatrix(3.1415926f) * v;

			constexpr Vector4<double> v2{ 1,1,0,0 };
			const Vector4<double> vr2 = 
				Matrix4<double>(GetXRotateMatrix(3.1415926)) * v2;
		}
	};
}


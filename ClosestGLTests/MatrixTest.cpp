#include "stdafx.h"
#include "CppUnitTest.h"

#include <Matrix2.h>
#include <Matrix3.h>
#include <Matrix4.h>
#include <VectorCommon.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Math;

namespace ClosestGLTests::MathTest
{
	TEST_CLASS(MatrixTest)
	{
	public:

		TEST_METHOD(Matrix2MulVector2)
		{
			{
				constexpr Matrix2<int> mat
				{
					Vector2<int>{0,1},
					Vector2<int>{1,0}
				};

				constexpr auto vec = Vector2<int>{ 10, 20 };
				constexpr auto vec2 = mat * vec;
				static_assert(vec2 == Vector2<int>{ 20, 10 });
			}
			{
				constexpr auto vec = Vector2<int>{ 10, 20 };
				constexpr auto vec2 = Matrix2Identity<int> * vec;
				static_assert(vec2 == vec);
			}
		}

		TEST_METHOD(Matrix3MulVector3)
		{
			constexpr auto vec = Vector3<int>{ 10, 20,30 };
			constexpr auto vec2 = Matrix3Identity<int> * vec;
			static_assert(vec2 == vec);
		}

		TEST_METHOD(Matrix4MulVector4)
		{
			constexpr Matrix4<int> mat
			{
				Vector4<int>{0,0,0,1},
				Vector4<int>{0,0,1,0},
				Vector4<int>{0,1,0,0},
				Vector4<int>{1,0,0,0}
			};
			constexpr auto vec = Vector4<int>{ 10, 20, 30, 40 };
			constexpr auto vec2 = mat * vec;
			static_assert(vec2 == Vector4<int>{ 40, 30, 20, 10 });
		}

		TEST_METHOD(Matrix2Mul)
		{
			Assert::IsTrue(
				Matrix2Identity<int> * Matrix2Identity<int> == Matrix2Identity<int>);
		}

		TEST_METHOD(Matrix3Mul)
		{
			Assert::IsTrue(
				Matrix3Identity<int> * Matrix3Identity<int> == Matrix3Identity<int>);
		}

		TEST_METHOD(Matrix4Mul)
		{
			Assert::IsTrue(
				Matrix4Identity<int> * Matrix4Identity<int> == Matrix4Identity<int>);
		}
	};
}
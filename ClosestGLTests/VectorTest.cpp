#include "stdafx.h"
#include "CppUnitTest.h"

#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <VectorCommon.h>
#include <MathCommon.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Math;

namespace ClosestGLTests::MathTest
{		
	TEST_CLASS(VectorTest)
	{
	public:
		
		TEST_METHOD(TestVector2Add)
		{
			constexpr Vector2<int> a{ 1,2 }, b{ 3,4 };
			constexpr auto c = a + b;
			static_assert(c.x == 4);
			static_assert(c.y == 6);
		}

		TEST_METHOD(TestVector2Sub)
		{
			constexpr Vector2<int> a{ 1,2 }, b{ 3,4 };
			constexpr auto c = a - b;
			static_assert(c.x == -2);
			static_assert(c.y == -2);
		}

		TEST_METHOD(TestVector2MulNumber)
		{
			constexpr Vector2<int> a{ 1,2 };
			constexpr auto c = a * 5;
			static_assert(c.x == 5);
			static_assert(c.y == 10);
		}

		TEST_METHOD(TestVector2Dot)
		{
			constexpr Vector2<int> a{ 1,2 }, b{ 3,4 };
			constexpr auto c = Dot(a, b);
			static_assert(c == 11);
		}

		TEST_METHOD(TestVector2LengthPow2)
		{
			constexpr Vector2<int> a{ 1,2 };
			Assert::AreEqual(a.Length(),2);
		}

		TEST_METHOD(TestVector3Add)
		{
			constexpr Vector3<int> a{ 1,2,3 }, b{ 3,4,5 };
			constexpr auto c = a + b;
			static_assert(c.x == 4);
			static_assert(c.y == 6);
			static_assert(c.z == 8);
		}

		TEST_METHOD(TestVector3Sub)
		{
			constexpr Vector3<int> a{ 1,2,3 }, b{ 3,4,5 };
			constexpr auto c = a - b;
			static_assert(c.x == -2);
			static_assert(c.y == -2);
			static_assert(c.z == -2);
		}

		TEST_METHOD(TestVector3MulNumber)
		{
			constexpr Vector3<int> a{ 1,2,3 };
			constexpr auto c = a * 5;
			static_assert(c.x == 5);
			static_assert(c.y == 10);
			static_assert(c.z == 15);
		}

		TEST_METHOD(TestVector3Dot)
		{
			constexpr Vector3<int> a{ 1,2,3 }, b{ 3,4,5 };
			constexpr auto c = Dot(a, b);
			static_assert(c == 26);
		}

		TEST_METHOD(TestVector3LengthPow2)
		{
			constexpr Vector3<int> a{ 1,2,3 };
			Assert::AreEqual(a.Length(), 3);
		}

		TEST_METHOD(TestVector4Add)
		{
			constexpr Vector4<int> a{ 1,2,3,4 }, b{ 3,4,5,6 };
			constexpr auto c = a + b;
			static_assert(c.x == 4);
			static_assert(c.y == 6);
			static_assert(c.z == 8);
			static_assert(c.w == 10);
		}

		TEST_METHOD(TestVector4Sub)
		{
			constexpr Vector4<int> a{ 1,2,3,4 }, b{ 3,4,5,6 };
			constexpr auto c = a - b;
			static_assert(c.x == -2);
			static_assert(c.y == -2);
			static_assert(c.z == -2);
			static_assert(c.w == -2);
		}

		TEST_METHOD(TestVector4MulNumber)
		{
			constexpr Vector4<int> a{ 1,2,3,4 };
			constexpr auto c = a * 5;
			static_assert(c.x == 5);
			static_assert(c.y == 10);
			static_assert(c.z == 15);
			static_assert(c.w == 20);
		}

		TEST_METHOD(TestVector4Dot)
		{
			constexpr Vector4<int> a{ 1,2,3,4 }, b{ 3,4,5,6 };
			constexpr auto c = Dot(a, b);
			static_assert(c == 50);
		}

		TEST_METHOD(TestVector4LengthPow2)
		{
			constexpr Vector4<int> a{ 1,2,3,4 };
			Assert::AreEqual(a.Length(), 5);
		}

		TEST_METHOD(TestVectorsDistance)
		{
			constexpr Vector4<int> a{ 1,2,3,4 };
			constexpr Vector4<int> b{ 5,6,7,8 };
			const auto dis = Distance(a, b);
		}

		TEST_METHOD(TestNormalize)
		{
			constexpr Vector2<int> a{ 15,0 };
			const auto ax = Normalize(a) == Vector2<int>{1, 0};
			Assert::IsTrue(ax);

			constexpr Vector2<float> b{ 0,15 };
			const auto bnorm = Normalize(b);
			bool bx =
				bnorm.x > -0.01 && b.x <0.01 &&
				bnorm.y > 0.99 && bnorm.y < 1.01;

			Assert::IsTrue(bx);
		}

		TEST_METHOD(MiscOperators)
		{
			Vector2<int> a{ 1,2 };
			a -= Vector2<int>{0, 2};
			Assert::IsTrue(a == Vector2<int>{1, 0});

			a += Vector2<int>{1, 0};
			Assert::IsTrue(a == Vector2<int>{2, 0});

			a *= 2;
			Assert::IsTrue(a == Vector2<int>{4, 0});

			a /= 4;
			Assert::IsTrue(a == Vector2<int>{1, 0});
		}
	};
}
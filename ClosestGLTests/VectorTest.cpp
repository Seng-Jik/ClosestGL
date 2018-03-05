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
			const Vector2<int> a{ 1,2 }, b{ 3,4 };
			const auto c = a + b;
			Assert::AreEqual(c.x, 4);
			Assert::AreEqual(c.y, 6);
		}

		TEST_METHOD(TestVector2Sub)
		{
			const Vector2<int> a{ 1,2 }, b{ 3,4 };
			const auto c = a - b;
			Assert::AreEqual(c.x, -2);
			Assert::AreEqual(c.y, -2);
		}

		TEST_METHOD(TestVector2MulNumber)
		{
			const Vector2<int> a{ 1,2 };
			const auto c = a * 5;
			Assert::AreEqual(c.x, 5);
			Assert::AreEqual(c.y, 10);
		}

		TEST_METHOD(TestVector2Dot)
		{
			const Vector2<int> a{ 1,2 }, b{ 3,4 };
			const auto c = Dot(a, b);
			Assert::AreEqual(c, 11);
		}

		TEST_METHOD(TestVector2LengthPow2)
		{
			const Vector2<int> a{ 1,2 };
			Assert::AreEqual(a.Length(),2);
		}

		TEST_METHOD(TestVector3Add)
		{
			const Vector3<int> a{ 1,2,3 }, b{ 3,4,5 };
			const auto c = a + b;
			Assert::AreEqual(c.x, 4);
			Assert::AreEqual(c.y, 6);
			Assert::AreEqual(c.z, 8);
		}

		TEST_METHOD(TestVector3Sub)
		{
			const Vector3<int> a{ 1,2,3 }, b{ 3,4,5 };
			const auto c = a - b;
			Assert::AreEqual(c.x, -2);
			Assert::AreEqual(c.y, -2);
			Assert::AreEqual(c.z, -2);
		}

		TEST_METHOD(TestVector3MulNumber)
		{
			const Vector3<int> a{ 1,2,3 };
			const auto c = a * 5;
			Assert::AreEqual(c.x, 5);
			Assert::AreEqual(c.y, 10);
			Assert::AreEqual(c.z, 15);
		}

		TEST_METHOD(TestVector3Dot)
		{
			const Vector3<int> a{ 1,2,3 }, b{ 3,4,5 };
			const auto c = Dot(a, b);
			Assert::AreEqual(c, 26);
		}

		TEST_METHOD(TestVector3LengthPow2)
		{
			const Vector3<int> a{ 1,2,3 };
			Assert::AreEqual(a.Length(), 3);
		}

		TEST_METHOD(TestVector4Add)
		{
			const Vector4<int> a{ 1,2,3,4 }, b{ 3,4,5,6 };
			const auto c = a + b;
			Assert::AreEqual(c.x, 4);
			Assert::AreEqual(c.y, 6);
			Assert::AreEqual(c.z, 8);
			Assert::AreEqual(c.w, 10);
		}

		TEST_METHOD(TestVector4Sub)
		{
			const Vector4<int> a{ 1,2,3,4 }, b{ 3,4,5,6 };
			const auto c = a - b;
			Assert::AreEqual(c.x, -2);
			Assert::AreEqual(c.y, -2);
			Assert::AreEqual(c.z, -2);
			Assert::AreEqual(c.w, -2);
		}

		TEST_METHOD(TestVector4MulNumber)
		{
			const Vector4<int> a{ 1,2,3,4 };
			const auto c = a * 5;
			Assert::AreEqual(c.x, 5);
			Assert::AreEqual(c.y, 10);
			Assert::AreEqual(c.z, 15);
			Assert::AreEqual(c.w, 20);
		}

		TEST_METHOD(TestVector4Dot)
		{
			const Vector4<int> a{ 1,2,3,4 }, b{ 3,4,5,6 };
			const auto c = Dot(a, b);
			Assert::AreEqual(c, 50);
		}

		TEST_METHOD(TestVector4LengthPow2)
		{
			const Vector4<int> a{ 1,2,3,4 };
			Assert::AreEqual(a.Length(), 5);
		}

		TEST_METHOD(TestVectorsDistance)
		{
			const Vector4<int> a{ 1,2,3,4 };
			const Vector4<int> b{ 5,6,7,8 };
			const auto dis = Distance(a, b);
		}

		TEST_METHOD(TestNormalize)
		{
			const Vector2<int> a{ 15,0 };
			const auto ax = Normalize(a) == Vector2<int>{1, 0};
			Assert::IsTrue(ax);

			const Vector2<float> b{ 0,15 };
			const auto bnorm = Vector2<float>{0, 1};
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
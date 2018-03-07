#include "stdafx.h"
#include <Vector2.h>
#include <vector>
#include <FixedTransform.h>
#include <SingleThreadRunner.h>
#include <MultiThreadRunner.h>
#include "CppUnitTest.h"

using namespace std::chrono_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;


namespace ClosestGLTests::PrimitiveTest
{
	TEST_CLASS(FixedTransformTest)
	{
	private:

		struct NotVector
		{
			int a, b;
		};

		struct TransformToVector
		{

			int Fibo(int n)
			{
				if (n <= 2)
					return 1;
				else
					return Fibo(n - 1) + Fibo(n - 2);
			}

			Math::Vector2<int> operator() (const NotVector& v)
			{
				//假装这里有个时间很长的运算
				auto t = Fibo(6);
				volatile auto tv = t;
				return { v.a,v.b };
			}

			bool IsEquals(const NotVector& v, const Math::Vector2<int>& vec)
			{
				return v.a == vec.x && v.b == vec.y;
			}

			std::vector<NotVector> GenVector()
			{
				std::vector<NotVector> v;
				for (int i = 0; i < 500000; ++i)
					v.push_back({ i,500000 - i });
				return v;
			}
		};

	public:
		TEST_METHOD(TestFixedTransform)
		{
			TransformToVector transformer;
			auto input = transformer.GenVector();
			std::vector<Math::Vector2<int>> output{ input.size() };
			ParallelStrategy::SingleThreadRunner runner;
			Primitive::FixedTransform(transformer, input.data(), output.data(), input.size(), runner);

			//Verify
			for (size_t i = 0; i < input.size(); ++i)
				Assert::IsTrue(transformer.IsEquals(input[i], output[i]));
		}

		TEST_METHOD(TestFixedTransformParallel)
		{
			TransformToVector transformer;
			auto input = transformer.GenVector();
			std::vector<Math::Vector2<int>> output{ input.size() };
			ParallelStrategy::MultiThreadRunner runner(std::thread::hardware_concurrency());
			Primitive::FixedTransform(transformer, input.data(), output.data(), input.size(), runner);

			//Wait Result
			runner.Wait();

			//Verify
			for (size_t i = 0; i < input.size(); ++i)
				Assert::IsTrue(transformer.IsEquals(input[i], output[i]));
		}
	};
}

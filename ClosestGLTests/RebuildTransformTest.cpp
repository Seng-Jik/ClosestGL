#include "stdafx.h"
#include <Vector2.h>
#include <Vector4.h>
#include <vector>
#include <SingleThreadRunner.h>
#include <MultiThreadRunner.h>
#include <Transformers.h>

using namespace std::chrono_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Math;
using namespace ClosestGL;



namespace ClosestGLTests::PrimitiveTest
{
	TEST_CLASS(RebuildTransformTest)
	{
	private:

		struct RebuildTransformer
		{
			void operator () (const Vector4<int>& input, std::vector<Vector2<int>>& output)
			{
				output.push_back({ input.x,input.y });
				output.push_back({ input.z,input.w });
			}

			std::vector<Vector4<int>> GenVec()
			{
				std::vector<Vector4<int>> vec;
				for (int i = 0; i < 50000; ++i)
				{
					vec.push_back({ i,i + 1,i + 2,i + 3 });
				}
				return vec;
			}

			bool Verify(const Vector4<int>& v, const Vector2<int>& v1, const Vector2<int>& v2)
			{
				return
					v.x == v1.x &&
					v.y == v1.y &&
					v.z == v2.x &&
					v.w == v2.y;
			}
		};

	public:
		TEST_METHOD(TestRebuildTransform)
		{
			RebuildTransformer tsf;
			auto input = tsf.GenVec();

			std::vector<std::vector<Vector2<int>>> outputs;
			Primitive::RebuildTransform(
				tsf, 
				input.data(), 
				input.size(),
				outputs,
				ParallelStrategy::SingleThreadRunner{}	//必须是单线程，否则结果会被分为多个组
			);
			

			//Verify
			for (size_t i = 0; i < input.size(); ++i)
			{
				auto v = input[i];
				auto v1 = outputs[0][2*i];
				auto v2 = outputs[0][2*i+1];

				Assert::IsTrue(tsf.Verify(v, v1, v2));
			}
		}

		TEST_METHOD(TestRebuildTransformParallel)
		{
			RebuildTransformer tsf;
			auto input = tsf.GenVec();

			ParallelStrategy::MultiThreadRunner runner(std::thread::hardware_concurrency());
			std::vector < std::vector<Vector2<int>>> output;

			Primitive::RebuildTransform(
				tsf,
				input.data(),
				input.size(),
				output,
				runner
			);

			runner.Wait();
		}
	};
}

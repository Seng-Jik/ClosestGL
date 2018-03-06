#include "stdafx.h"
#include "CppUnitTest.h"

#include <MultiThreadRunner.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::ParallelStrategy;

namespace ClosestGLTests::ParallelStrategyTest
{
	TEST_CLASS(MultiThreadRunnerTest)
	{
	public:
		TEST_METHOD(TestMultiThreadFor)
		{
			MultiThreadRunner runner(30);
			std::mutex t;
			std::vector<std::tuple<int, unsigned>> mark;

			int i = 0;
			for (int j = 0; j < 5; ++j)
			{
				
				runner.Commit(0, 100,
					[&i, &t, &mark](unsigned index, unsigned threadID) {
					t.lock();
					i++;
					mark.push_back({ i,threadID });
					t.unlock();
					
				});
			}

			runner.Wait();
		}
	};
}
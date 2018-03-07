#include "stdafx.h"

#include <SingleThreadRunner.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::ParallelStrategy;

namespace ClosestGLTests::ParallelStrategyTest
{
	TEST_CLASS(SingleThreadRunnerTest)
	{
	public:
		TEST_METHOD(TestSingleThreadFor)
		{
			SingleThreadRunner runner;
			int i = 0;
			runner.Commit(0,100,
				[&i](size_t t, size_t threadID) {
				i++;
				Assert::AreEqual(threadID, size_t(0));
			});

			runner.Wait();

			Assert::AreEqual(i, 100);
		}
	};
}
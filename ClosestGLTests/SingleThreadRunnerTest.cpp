#include "stdafx.h"
#include "CppUnitTest.h"

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
				[&i](unsigned t,unsigned threadID) {
				i++;
				Assert::AreEqual(threadID, 0u);
			});

			runner.Wait();

			Assert::AreEqual(i, 100);
		}
	};
}
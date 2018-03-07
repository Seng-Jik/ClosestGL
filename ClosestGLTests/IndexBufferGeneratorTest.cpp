#include "stdafx.h"
#include <IndexBufferGenerator.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;


namespace ClosestGLTests::PrimitiveTest
{
	TEST_CLASS(IndexBufferGeneratorTest)
	{
	public:
		TEST_METHOD(TestIndexBufferGenerator)
		{
			auto gented = Primitive::Generate(8000);

			for (size_t i = 0; i < 8000; ++i)
				Assert::AreEqual(i, gented[i]);
		}
	};
}
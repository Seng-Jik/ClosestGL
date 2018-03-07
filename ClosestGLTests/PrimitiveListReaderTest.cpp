#include "stdafx.h"
#include <vector>
#include <PrimitiveListReader.h>
#include <string>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL::Primitive;


namespace ClosestGLTests::PrimitiveTest
{
	TEST_CLASS(PrimitiveListReaderTest)
	{
	public:
		TEST_METHOD(TestListReader)
		{
			std::vector<size_t> ibo = 
			{
				1,2,3,
				2,3,4,
				3,4,5,
				4,5
			};

			PrimitiveListReader<3> reader(ibo.data(), ibo.size());

			while (reader.CanRead())
			{
				auto pri = reader.Read();

				std::string msg = "Triangle:" +
					std::to_string(pri[0]) + "," +
					std::to_string(pri[1]) + "," +
					std::to_string(pri[2]) + '\n';

				Logger::WriteMessage(msg.c_str());
			}
		}
	};
}

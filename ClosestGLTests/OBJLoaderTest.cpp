#include "stdafx.h"
#include "OBJLoader.h"
#include <Vector4.h>
#include <PrimitiveListReader.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClosestGLTests::Tools
{
	TEST_CLASS(OBJLoaderTest)
	{
	private:
		struct Vertex
		{
			ClosestGL::Math::Vector4<float> SVPosition;

			void Print() const
			{
				std::stringstream strs;
				strs <<
					"Vertex:" <<
					SVPosition.x << ',' <<
					SVPosition.y << ',' <<
					SVPosition.z << ',' <<
					SVPosition.w << std::endl;
				Logger::WriteMessage(strs.str().c_str());
			}
		};
	public:
		TEST_METHOD(TestLoadOBJ)
		{
			auto obj = LoadModelOBJWireFrameOnly<Vertex>("Cube");

			for (const auto& p : std::get<0>(obj))
			{
				p.Print();
			}

			auto& ibo = std::get<1>(obj);
			ClosestGL::Primitive::PrimitiveListReader<3> pr(ibo.data(),ibo.size());

			while (pr.CanRead())
			{
				auto vtx = pr.Read();
				std::stringstream strs;
				strs << 
					"Face:" <<
					vtx[0] << ',' << 
					vtx[1] << ',' << 
					vtx[2] << std::endl;

				Logger::WriteMessage(strs.str().c_str());
			}
			
		}
	};
}
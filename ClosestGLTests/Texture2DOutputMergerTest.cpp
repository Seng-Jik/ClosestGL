#include "stdafx.h"

#include <Texture2DOutputPass.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;

namespace ClosestGLTests::OutputMergerTest
{
	TEST_CLASS(Texture2DOutputMergerTest)
	{
	public:
		TEST_METHOD(TestTexture2DOutputMerger)
		{
			Tools::TestTex tex{ { 640,480} };
			OutputMerger::Texture2DOutputPass<Tools::TestCol> om{ tex };

			const auto s = om.GetMainColorBuffer().GetSize();
			for (size_t x = 0; x < s.x; ++x)
			{
				for (size_t y = 0; y < s.y; ++y)
				{
					bool isBlack = false;

					if (x % 16 >= 8) isBlack = !isBlack;
					if (y % 16 >= 8) isBlack = !isBlack;

					Utils::PixelInformation<Tools::TestCol, int> pi
					{
						{x,y},
						isBlack ? Tools::TestCol{ 0,0,0,0,} : Tools::TestCol{255,255,255,255},
						0
					};

					om.EmitPixel(pi);
				}
			}

			Tools::ViewSurface(tex,1000);
		}
	};
}
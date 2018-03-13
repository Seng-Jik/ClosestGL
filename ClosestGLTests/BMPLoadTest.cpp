#include "stdafx.h"
#include "BMPLoader.h"

namespace ClosestGLTests::Tools
{
	TEST_CLASS(BMPLoadTest)
	{
	private:
	public:
		TEST_METHOD(TestBMPLoad)
		{
			auto bmp = Tools::LoadBMPFile("Zelda");
			Tools::ViewSurface(bmp);
		}
	};
}

#pragma once
#include <Texture2D.h>
#include <Vector4.h>
namespace ClosestGLTests::Tools
{
	using TestCol = ClosestGL::Math::Vector4<float>;
	using TestTex = ClosestGL::Texture::Texture2D<TestCol>;
	void ViewSurface(TestTex& sur,uint64_t time = 5000);
}

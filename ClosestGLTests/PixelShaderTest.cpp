#include "stdafx.h"
#include <PixelShaderStage.h>
#include <RenderTarget.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace ClosestGL;
using namespace ClosestGL::Math;
using namespace ClosestGL::Texture;
using namespace ClosestGL::RenderPipeline;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(PixelShaderTest)
	{
	public:
		TEST_METHOD(TestPixelShader)
		{
			Tools::TestTex tex{ {640,480} };
			RenderTarget<1, Tools::TestCol, decltype(Tools::Blenders::AddtiveBlender)>
				rt{ Tools::Blenders::AddtiveBlender,{&tex} };

			struct PSInput
			{
				Math::Vector4<float> color;
			};

			auto PS1 = [](const PSInput& psi,const auto&)
			{
				std::array<Tools::TestCol, 1> result{ psi.color };
				return result;
			};

			PixelShaderStage<decltype(rt), decltype(PS1)>
				ps{ &rt,PS1 };

			PSInput psi
			{
				{1,0,0,1}
			};

			for (size_t y = 0; y < tex.GetSize().y; ++y)
			{
				for (size_t x = 0; x < tex.GetSize().x; ++x)
				{
					ps.EmitPixel(psi, Vector2<size_t>{ x,y });
				}
			}

			PSInput psi2
			{
				{0,1,0.5,1}
			};

			for (size_t y = 0; y < tex.GetSize().y; ++y)
			{
				for (size_t x = 0; x < tex.GetSize().x; ++x)
				{
					ps.EmitPixel(psi2, Vector2<size_t>{ x, y });
				}
			}

			Tools::ViewSurface(tex);
		}
	};
}

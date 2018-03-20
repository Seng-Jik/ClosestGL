#pragma once
#include <Vector4.h>
#include "SurfaceViewer.h"
#include "Blenders.h"
#include <LineRasterizer.h>
#include <RenderTarget.h>
#include <PixelShader.h>
#include <array>
#include <functional>
#include <Matrix4.h>
#include <MathCommon.h>
#include <utility>
namespace ClosestGLTests::Tools
{
	namespace WireFrameViewer
	{
		struct Vertex
		{
			ClosestGL::Math::Vector4<float> SVPosition;

			static Vertex Lerp(float x, const Vertex& p1, const Vertex& p2)
			{
				return {
					ClosestGL::Math::Lerp(x,p1.SVPosition,p2.SVPosition)
				};
			}
		};

		struct PS
		{
			std::array<TestCol, 1> operator() (const Vertex& v)
			{
				float col = std::clamp((1 / v.SVPosition.z) * 70,0.0f,1.0f);
				return { { col,col,col,1} };
			}
		};

		using RenderTarget =
			ClosestGL::RenderPipeline::RenderTarget
			<1, TestCol,decltype(Blenders::NoBlend)>;

		using PixelShader = ClosestGL::RenderPipeline::PixelShaderStage
			<RenderTarget, PS>;

		std::vector<size_t> ConvertTranglesToLines(const std::vector<size_t>&);
	}

	using ModelRenderer = std::function<void(const ClosestGL::Math::Matrix4<float>& view)>;

	void ViewModel(TestTex& tex, const ModelRenderer& renderer,uint64_t time = 5000);
}
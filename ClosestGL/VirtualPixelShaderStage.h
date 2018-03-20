#pragma once
#include <array>
#include <functional>
#include "PixelShaderStage.h"

namespace ClosestGL::RenderPipeline
{
	template<
		typename TRenderTarget,
		typename ColorType,
		int ColorBuffers,
		typename VertexAttributesType>
	class VirtualPixelShaderStage
	{
	private:
		using PixelShaderFunc = 
			std::function<std::array<ColorType, ColorBuffers>(const VertexAttributesType&)>;

		PixelShaderFunc ps_;
		PixelShaderStage<TRenderTarget,PixelShaderFunc> psStage_;

	public:
		VirtualPixelShaderStage(TRenderTarget* renderTarget) :
			ps_(renderTarget, &ps_)
		{}
	};
}

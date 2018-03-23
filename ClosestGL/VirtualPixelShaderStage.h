#pragma once
#include <array>
#include <functional>

namespace ClosestGL::RenderPipeline
{
	/* 虚像素着色器
	 * 可以在运行时修改着色器函数。
	 */
	template<
		typename TRenderTarget,
		typename ColorType,
		int ColorBuffers,
		typename TPixelAttributes>
	class VirtualPixelShaderStage
	{
	private:
		using PixelShaderFunc = 
			std::function<std::array<ColorType, ColorBuffers>(const TPixelAttributes&)>;

		PixelShaderFunc ps_;
		TRenderTarget* target_;
	public:
		VirtualPixelShaderStage(TRenderTarget* renderTarget) :
			target_{ renderTarget }
		{}

		void SetPixelShader(const PixelShaderFunc& shader)
		{
			ps_ = shader;
		}

		void EmitPixel(const TPixelAttributes& data, const Math::Vector2<size_t>& pos)
		{
			target_->EmitPixel(ps_(data,pos), pos);
		}

		auto GetRenderTargetSize() const
		{
			return target_->GetRenderTargetSize();
		}
	};
}

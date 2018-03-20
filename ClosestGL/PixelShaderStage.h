#pragma once
#include "Vector2.h"
namespace ClosestGL::RenderPipeline
{
	/* 像素着色器阶段 
	 * 
	 * 模板参数：
	 * TRenderTarget - 连接到的RenderTarget的类型
	 * TPixelShader  - 使用的像素着色器的类型
	 * TData         - 像素着色器输入数据的类型
	 *
	 * 公开成员函数：
	 * 构造函数
	 *     rt - 传入RenderTarget的指针，PixelShader对象的生命周期必须小于等于rt。
	 *     ps - 传入像素着色器
	 * EmitPixel - 发送一个像素
	 *     data - 像素着色器输入参数
	 *     pos  - 坐标，坐标系在RenderTarget的纹理空间，以像素为单位。
	 */
	template<typename TRenderTarget,typename TPixelShader>
	class PixelShaderStage
	{
	private:
		TPixelShader ps_;
		TRenderTarget* const target_;
	public:
		PixelShaderStage(TRenderTarget* rt, const TPixelShader& ps):
			ps_{ps},
			target_{rt}
		{}

		template<typename TPixelAttributes>
		void EmitPixel(const TPixelAttributes& data,const Math::Vector2<size_t>& pos)
		{
			target_->EmitPixel(ps_(data),pos);
		}

		auto GetRenderTargetSize() const
		{
			return target_->GetRenderTargetSize();
		}
	};
}

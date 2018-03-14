#pragma once
#include "Texture2D.h"

namespace ClosestGL::RenderPipeline
{
	/* 深度测试 
	 * 
	 * 模板参数：
	 *     TNextStage - 下个阶段的类型
	 *     TDepthType - 深度缓存的类型
	 * 
	 * 成员函数：
	 *     构造函数
	 *            nextStage - 下个阶段的对象的指针，生命周期必须大于等于本对象。

	 *     GetDepthBuffer       - 获取深度缓存
	 *     ClearDepthBuffer     - 清空深度缓存
	 *          runner  - 多线程执行策略
	 *     EmitPixel            - 提交像素
	 *     GetRenderTargetSize  - 获取渲染目标大小
	 */
	template<typename TNextStage, typename TDepthType>
	class DepthTest
	{
	private:
		TNextStage * nextStage_;
		Texture::Texture2D<TDepthType>* depthBuffer_;

	public:
		auto GetRenderTargetSize() const
		{
			return nextStage_->GetRenderTargetSize();
		}

		template<typename TPixelAttributes>
		void EmitPixel(const TPixelAttributes& data, const Math::Vector2<size_t>& pos)
		{
			auto& dst = depthBuffer_->AccessPixelUnsafe(pos);
			const TDepthType src = 1 / data.SVPosition.w;

			if (src > dst)
			{
				dst = src;
				nextStage_->EmitPixel(data, pos);
			}
		}

		template<typename TRunner>
		void ClearDepthBuffer(TRunner& runner)
		{
			depthBuffer_->Clear(0,runner);
		}

		auto& GetDepthBuffer()
		{
			return depthBuffer_;
		}

		DepthTest(TNextStage* nextStage, Texture::Texture2D<TDepthType>* depthBuffer):
			nextStage_{ nextStage },
			depthBuffer_{ depthBuffer }
		{}


	};
}

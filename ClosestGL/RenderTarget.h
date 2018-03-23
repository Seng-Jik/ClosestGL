#pragma once
#include "Vector2.h"
#include "Texture2D.h"
#include <optional>
#include <array>

namespace ClosestGL::RenderPipeline
{	
	/* 渲染目标
	 * 渲染管线在这里终止，渲染目标会把计算得出的颜色传入到2D纹理上。
	 * 注意，该类里没有任何安全检查措施，任何非常规操作都是未定义的。
	 * 请严格按照说明来使用本类。
	 * 
	 * 模板参数：
	 * ColorBufferCount - 颜色缓存数量，最少为1张颜色缓存
	 * ColorType        - 颜色类型，它其中的每一个标量范围都必须在0~1之间
	 * Blender          - 混合器，这是一个函数，输入源颜色和目标颜色，输出混合的最终颜色。
	 *                    混合器的函数原形：ColorType Blender(ColorType src,ColorType dst)
	 *
	 * 公开的成员函数：
	 * GetColorBufferCount - 获取颜色缓存数量
	 * 构造函数
	 *     b            - 传入混合器实例
	 *     colorBuffers - 传入颜色缓存列表，传入的数量必须和ColorBufferCount一致，且各个纹理长宽均相同。
	 *                    RenderTarget的生命周期必须小于等于colorBuffers内所有纹理的生命周期
	 * GetTexture         - 获取第n个颜色缓存
	 * EmitPixel          - 发送一个像素到RenderTarget
	 *     colors - 传入的颜色列表，此数组长度必须和ColorBufferCount一致。
	 *     pos    - 像素位置，坐标系在颜色缓存纹理空间，以像素为单位。
	 * GetSize    - 获取RenderTarget的大小
	 */
	template<int ColorBufferCount,typename ColorType,typename Blender>
	class RenderTarget
	{
	private:
		using Texture = Texture::Texture2D<ColorType>;

		std::array<Texture*, ColorBufferCount> cbs_;
		Blender blender_;
		Math::Vector2<size_t> size_;

	public:

		constexpr static int GetColorBufferCount()
		{
			return ColorBufferCount;
		}

		RenderTarget(const Blender& b,std::initializer_list<Texture*> colorBuffers):
			blender_{ b }
		{
			int t = 0;
			for (auto p = colorBuffers.begin(); p != colorBuffers.end(); ++p, ++t)
				cbs_[t] = *p;

			size_ = cbs_[0]->GetSize();
		}

		Texture& GetTexture(int n)
		{
			return *cbs_[n];
		}

		void SetTexture(int n, Texture* tex)
		{
			cbs_[n] = tex;
		}

		void EmitPixel(const std::array<ColorType, ColorBufferCount>& colors, const Math::Vector2<size_t>& pos)
		{
			for (int i = 0; i < ColorBufferCount; ++i)
			{
				auto& dst = (*cbs_[i]).AccessPixelUnsafe(pos);
				dst = blender_(colors[i], dst);
			}
		}

		auto GetRenderTargetSize() const
		{
			return size_;
		}
	};
}
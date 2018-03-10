#pragma once
#include <vector>
#include "Vector2.h"

namespace ClosestGL::Texture
{

	/* 2D纹理 
	 * 需传入颜色的类型ColorType
	 * 每个像素在内存中都是紧密排列的。
	 */

	template<typename ColorType>
	class Texture2D
	{
	private:
		std::vector<ColorType> color_;
		const Math::Vector2<size_t> size_;

	public:

		/* 传入纹理大小，构造一个2D纹理。 */
		Texture2D(Math::Vector2<size_t> size) :
			size_{ size },
			color_{ size.x * size.y }
		{}

		/* 获取纹理大小 */
		Math::Vector2<size_t> GetSize() const { return size_; };

		/* 获取指向第一个像素的指针 */
		ColorType* Data() { return color_.data(); }

		/* 计算坐标pos位置对应的像素的偏移量。 */
		size_t CalcOffset(Math::Vector2<size_t> pos)
		{
			return pos.y * size_.x + pos.x;
		}

		/* 访问在坐标pos位置的像素 */
		ColorType& AccessPixel(Math::Vector2<size_t> pos)
		{
			return color_.at(CalcOffset(pos));
		}

		/* 不安全的访问在坐标pos位置的像素 */
		ColorType& AccessPixelUnsafe(Math::Vector2<size_t> pos)
		{
			return color_[CalcOffset(pos)];
		}

		/* 对每个像素进行着色 
		 * ts - 纹理着色器，需要具有以下函数原形：
		 *    ColorType ts(Vector2<size_t> pos)
		 *        返回值 - 颜色计算结果
		 *        pos   - 当前计算的坐标
		 * runner - 并行执行策略（参加SingleThreadRunner）
		 */
		template<typename TextureShader,typename Runner>
		void Shade(TextureShader& ts,Runner& runner)
		{
			runner.Commit(0, size_.y,
				[this,ts](size_t y,size_t) {
					for (size_t x = 0; x < size_.x; ++x)
					{
						Math::Vector2<size_t> pos{ x, y };
						AccessPixelUnsafe(pos) = ts(pos);
					}
				}
			);
		}

		/* 使用一个颜色填充纹理
		 * color - 用于填充纹理的颜色
		 * runner - 并行执行策略（参加SingleThreadRunner）
		 */
		template<typename Runner>
		void Clear(ColorType color, Runner& runner)
		{
			Shade(
				[color](auto)
				{
					return color;
				}, 
				runner
			);
		}
	};
}
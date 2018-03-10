#pragma once
#include <vector>
#include "Vector2.h"

namespace ClosestGL::Texture
{

	/* 2D���� 
	 * �贫����ɫ������ColorType
	 * ÿ���������ڴ��ж��ǽ������еġ�
	 */

	template<typename ColorType>
	class Texture2D
	{
	private:
		std::vector<ColorType> color_;
		const Math::Vector2<size_t> size_;

	public:

		/* ���������С������һ��2D���� */
		Texture2D(Math::Vector2<size_t> size) :
			size_{ size },
			color_{ size.x * size.y }
		{}

		/* ��ȡ�����С */
		Math::Vector2<size_t> GetSize() const { return size_; };

		/* ��ȡָ���һ�����ص�ָ�� */
		ColorType* Data() { return color_.data(); }

		/* ��������posλ�ö�Ӧ�����ص�ƫ������ */
		size_t CalcOffset(Math::Vector2<size_t> pos)
		{
			return pos.y * size_.x + pos.x;
		}

		/* ����������posλ�õ����� */
		ColorType& AccessPixel(Math::Vector2<size_t> pos)
		{
			return color_.at(CalcOffset(pos));
		}

		/* ����ȫ�ķ���������posλ�õ����� */
		ColorType& AccessPixelUnsafe(Math::Vector2<size_t> pos)
		{
			return color_[CalcOffset(pos)];
		}

		/* ��ÿ�����ؽ�����ɫ 
		 * ts - ������ɫ������Ҫ�������º���ԭ�Σ�
		 *    ColorType ts(Vector2<size_t> pos)
		 *        ����ֵ - ��ɫ������
		 *        pos   - ��ǰ���������
		 * runner - ����ִ�в��ԣ��μ�SingleThreadRunner��
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

		/* ʹ��һ����ɫ�������
		 * color - ��������������ɫ
		 * runner - ����ִ�в��ԣ��μ�SingleThreadRunner��
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
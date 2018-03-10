#pragma once
#include "Vector2.h"
#include "Texture2D.h"
#include <optional>
#include <array>

namespace ClosestGL::RenderPipeline
{	
	/* ��ȾĿ��
	 * ��Ⱦ������������ֹ����ȾĿ���Ѽ���ó�����ɫ���뵽2D�����ϡ�
	 * ע�⣬������û���κΰ�ȫ����ʩ���κηǳ����������δ����ġ�
	 * ���ϸ���˵����ʹ�ñ��ࡣ
	 * 
	 * ģ�������
	 * ColorBufferCount - ��ɫ��������������Ϊ1����ɫ����
	 * ColorType        - ��ɫ���ͣ������е�ÿһ��������Χ��������0~1֮��
	 * Blender          - �����������һ������������Դ��ɫ��Ŀ����ɫ�������ϵ�������ɫ��
	 *                    ������ĺ���ԭ�Σ�ColorType Blender(ColorType src,ColorType dst)
	 *
	 * �����ĳ�Ա������
	 * GetColorBufferCount - ��ȡ��ɫ��������
	 * ���캯��
	 *     b            - ��������ʵ��
	 *     colorBuffers - ������ɫ�����б���������������ColorBufferCountһ�£��Ҹ������������ͬ��
	 *                    RenderTarget���������ڱ���С�ڵ���colorBuffers�������������������
	 * GetTexture         - ��ȡ��n����ɫ����
	 * EmitPixel          - ����һ�����ص�RenderTarget
	 *     colors - �������ɫ�б������鳤�ȱ����ColorBufferCountһ�¡�
	 *     pos    - ����λ�ã�����ϵ����ɫ��������ռ䣬������Ϊ��λ��
	 * GetSize    - ��ȡRenderTarget�Ĵ�С
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
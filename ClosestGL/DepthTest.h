#pragma once
#include "Texture2D.h"

namespace ClosestGL::RenderPipeline
{
	/* ��Ȳ��� 
	 * 
	 * ģ�������
	 *     TNextStage - �¸��׶ε�����
	 *     TDepthType - ��Ȼ��������
	 * 
	 * ��Ա������
	 *     ���캯��
	 *            nextStage - �¸��׶εĶ����ָ�룬�������ڱ�����ڵ��ڱ�����

	 *     GetDepthBuffer       - ��ȡ��Ȼ���
	 *     ClearDepthBuffer     - �����Ȼ���
	 *          runner  - ���߳�ִ�в���
	 *     EmitPixel            - �ύ����
	 *     GetRenderTargetSize  - ��ȡ��ȾĿ���С
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

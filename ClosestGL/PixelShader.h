#pragma once
#include "Vector2.h"
namespace ClosestGL::RenderPipeline
{
	/* ������ɫ���׶� 
	 * 
	 * ģ�������
	 * TRenderTarget - ���ӵ���RenderTarget������
	 * TPixelShader  - ʹ�õ�������ɫ��������
	 * TData         - ������ɫ���������ݵ�����
	 *
	 * ������Ա������
	 * ���캯��
	 *     rt - ����RenderTarget��ָ�룬PixelShader������������ڱ���С�ڵ���rt��
	 *     ps - ����������ɫ��
	 * EmitPixel - ����һ������
	 *     data - ������ɫ���������
	 *     pos  - ���꣬����ϵ��RenderTarget������ռ䣬������Ϊ��λ��
	 */
	template<typename TRenderTarget,typename TPixelShader>
	class PixelShader
	{
	private:
		TPixelShader ps_;
		TRenderTarget* const target_;
	public:
		PixelShader(TRenderTarget* rt, const TPixelShader& ps):
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

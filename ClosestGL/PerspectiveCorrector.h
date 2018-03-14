#pragma once
#include "MathCommon.h"

namespace ClosestGL::RenderPipeline::PerspectiveCorrector
{
	/* ͸�ӽ�����
	 *
	 * ʹ�÷�����
	 * 1.�ڶ�����ɫ���򼸺���ɫ���ｨ��BeforePerspectiveDivision����
	 *   ���캯�������Ѿ�Ӧ����ȫ������任�Ķ������ꡣ
	 * 2.�Էº����ķ�ʽ�ڶ�����ɫ���򼸺���ɫ���ڵ��ö��󣬴���Ҫ�����Ķ������ԡ�
	 * 3.��BeforePerspectiveDivision������InPixelShader����
	 *   �����Ƕ����е�InPixelShader�������Active������BeforePerspectiveDivision����
	 * 4.��InPixelShader������붥����ɫ������򼸺���ɫ����������ڶ�������ṹ���Lerp����
	 *   ������ClosestGL::Math::Lerp������InPixelShader������в�ֵ��
	 * 5.������InPixelShader�Ķ������դ������դ������ö�������ṹ���Lerp����
	 *   InPixelShader������ڴ˴�����ֵ��
	 * 6.��������ɫ����������UV����������Ҫ�����Ķ������ԣ���InPixelShader�ķº�����ʽ���е��ã�
	 *   InPixelShader�º���������Ҫ�������Ķ������ԣ����ر������Ķ������ԡ�
	 */

	template<typename LerpType>
	class BeforePerspectiveDivision
	{
	private:
		const LerpType rhw_;
	public:
		template<typename Position>
		BeforePerspectiveDivision(const Position& transformed):
			rhw_ { 1 / transformed.w }
		{}

		BeforePerspectiveDivision(const BeforePerspectiveDivision<LerpType>&) = delete;

		template<typename Attr>
		auto operator () (const Attr& attr) const
		{
			return attr * rhw_;
		}

		inline auto GetRHW() const
		{
			return rhw_;
		}
	};

	template<typename LerpType>
	class InPixelShader
	{
	private:
		LerpType rhw_ = 1;
	public:
		InPixelShader() = default;
		InPixelShader(LerpType rhw) :
			rhw_{ rhw } {}

		void Active(const BeforePerspectiveDivision<LerpType>& bpd)
		{
			rhw_ = bpd.GetRHW();
		}

		InPixelShader(const BeforePerspectiveDivision<LerpType>& bpd)
		{
			Active(bpd);
		}

		inline auto GetRHW() const
		{
			return rhw_;
		}

		template<typename Attr>
		auto operator () (const Attr& attr) const
		{
			return attr * (1 / rhw_);
		}
	};
}

namespace ClosestGL::Math
{
	template<typename LerpType>
	RenderPipeline::PerspectiveCorrector::InPixelShader<LerpType> Lerp(
		LerpType x,
		RenderPipeline::PerspectiveCorrector::InPixelShader<LerpType> p1,
		RenderPipeline::PerspectiveCorrector::InPixelShader<LerpType> p2
	)
	{
		const auto lerped = Lerp(x, p1.GetRHW(), p2.GetRHW());
		return RenderPipeline::PerspectiveCorrector::InPixelShader<LerpType>(lerped);
	}
}
#pragma once
#include "MathCommon.h"

namespace ClosestGL::RenderPipeline::PerspectiveCorrector
{
	/* 透视矫正器
	 *
	 * 使用方法：
	 * 1.在顶点着色器或几何着色器里建立BeforePerspectiveDivision对象，
	 *   构造函数传入已经应用了全部顶点变换的顶点坐标。
	 * 2.以仿函数的方式在顶点着色器或几何着色器内调用对象，传入要矫正的顶点属性。
	 * 3.从BeforePerspectiveDivision对象构造InPixelShader对象，
	 *   或者是对已有的InPixelShader对象调用Active，传入BeforePerspectiveDivision对象，
	 * 4.将InPixelShader对象存入顶点着色器输出或几何着色器输出，并在顶点输出结构体的Lerp函数
	 *   上利用ClosestGL::Math::Lerp函数对InPixelShader对象进行插值。
	 * 5.将存有InPixelShader的对象传入光栅器，光栅器会调用顶点输出结构体的Lerp函数
	 *   InPixelShader对象会在此处被插值。
	 * 6.在像素着色器，对纹理UV或者其它需要修正的顶点属性，以InPixelShader的仿函数形式进行调用，
	 *   InPixelShader仿函数传入需要被修正的顶点属性，返回被修正的顶点属性。
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
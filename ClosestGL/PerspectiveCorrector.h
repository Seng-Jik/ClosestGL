#pragma once

namespace ClosestGL::RenderPipeline::PerspectiveCorrector
{
	template<typename LerpType>
	class BeforePerspectiveDivision
	{
	private:
		const LerpType w_, rhw_;
	public:
		template<typename Position>
		BeforePerspectiveDivision(const Position& transformed):
			w_{ transformed.w },
			rhw_ { 1 / transformed.w }
		{}

		BeforePerspectiveDivision(const BeforePerspectiveDivision<LerpType>&) = delete;

		template<typename Attr>
		auto operator () (const Attr& attr) const
		{
			return attr * rhw_;
		}
	};
}
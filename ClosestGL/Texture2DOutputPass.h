#pragma once
#include "Texture2D.h"
#include "Vector2.h"
#include "PixelInformation.h"

namespace ClosestGL::OutputMerger
{
	template<typename ColorType>
	class Texture2DOutputPass
	{
	private:
		Texture::Texture2D<ColorType>* mainColorBuffer_;

	public:
		Texture2DOutputPass(Texture::Texture2D<ColorType>& mainColorBuffer):
			mainColorBuffer_{ &mainColorBuffer }
		{}

		auto& GetMainColorBuffer()
		{
			return *mainColorBuffer_;
		}

		template<typename DepthType>
		void EmitPixel(const Utils::PixelInformation<ColorType,DepthType>& pi)
		{
			mainColorBuffer_->AccessPixel(pi.position) = pi.color;
		}
	};
}
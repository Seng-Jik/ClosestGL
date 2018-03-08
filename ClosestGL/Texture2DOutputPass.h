#pragma once
#include "Texture2D.h"
#include "Vector2.h"

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

		auto& GetColorBuffer()
		{
			return *mainColorBuffer_;
		}

		void EmitPixel(Math::Vector2<size_t> pos,const ColorType& col)
		{
			mainColorBuffer_->AccessPixel(pos) = col;
		}
	};
}
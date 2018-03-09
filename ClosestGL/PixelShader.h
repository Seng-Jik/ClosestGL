#pragma once

namespace ClosestGL::RenderPipeline
{
	template<typename TRenderTarget,typename TPixelShader,typename TData>
	class PixelShader
	{
	private:
		TPixelShader* ps_;
		TRenderTarget* target_;
	public:
		PixelShader(TRenderTarget& rt, TPixelShader& ps):
			ps_{&ps},
			target_{&rt}
		{}

		void EmitPixel(const TData& data)
		{
			target_.EmitPixel(TPixelShader(data),data.SVPosition);
		}
	};
}

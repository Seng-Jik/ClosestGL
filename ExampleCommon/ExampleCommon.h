#pragma once
#include <Window.h>
#include <Texture2D.h>
#include <Surface.h>
#include <algorithm>
#include <../SDLClasses/include/Vector2.h>

namespace ExampleCommon
{
	//更新纹理到SDL窗口
	template<typename Color>
	void UpdateWindow(SDL::Window& window, ClosestGL::Texture::Texture2D<Color>& tex)
	{
		const int pitch = window.GetWindowSurface().GetPitch();
		uint8_t* pixels = (uint8_t*)window.GetWindowSurface().GetPixelPointer();
		const auto size = window.GetWindowSurface().GetSize();

		for (int y = 0; y < size.y; ++y)
		{
			for (int x = 0; x < size.x; ++x)
			{
				auto color = tex.AccessPixelUnsafe({ size_t(x), size_t(y) });
				uint8_t* px = (pixels + y * pitch) + x * 4;
				px[2] = uint8_t(std::min(1.0f, color.x) * 255);
				px[1] = uint8_t(std::min(1.0f, color.y) * 255);
				px[0] = uint8_t(std::min(1.0f, color.z) * 255);
				px[3] = 255;
			}
		}

		window.UpdateWindowSurface();
	}

	//从BMP加载Texture
	template <typename Color,typename Runner>
	auto LoadBMP24(const std::string& path,Runner& runner)
	{
		SDL::Surface sur{ path };
		const uint8_t* pxptr = (uint8_t*)sur.GetPixelPointer();
		const auto pitch = sur.GetPitch();
		const auto size = sur.GetSize();

		Texture::Texture2D<Color> tex{ {(size_t)size.x,(size_t)size.y} };
		tex.Shade(
			[pxptr,pitch](const auto& pos)
		{
			const uint8_t* px = pxptr + pos.y * pitch + pos.x * 3;
			return Color{
				px[0] / 255.0f,
				px[1] / 255.0f,
				px[2] / 255.0f,
				0
			};
		},runner);
		
		return tex;
	}
}

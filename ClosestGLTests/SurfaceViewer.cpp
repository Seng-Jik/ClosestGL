#include "stdafx.h"
#include "SurfaceViewer.h"

void ClosestGLTests::Tools::ViewSurface(TestTex & sur, uint64_t time)
{
	SDL::SDLInstance sdl;

	auto s = sur.GetSize();

	SDL::Window window
	{
		"ClosestGL Demo",
		SDL::Rect<int>{ SDL::Window::Center,SDL::Window::Center,int(s.x),int(s.y) },
		SDL::Window::WindowFlag::Null
	};



	auto p = sdl.GetTicks();
	while (true)
	{
		window.GetWindowSurface().Shade(
			[&sur](int x, int y, auto&, auto&)
		{
			auto col = sur.AccessPixelUnsafe({ size_t(x),size_t(y) });
			return SDL::Color<uint8_t>{ uint8_t(col.x * 255), uint8_t(col.y * 255), uint8_t(col.z * 255), uint8_t(col.w * 255) };
		}
		);

		window.UpdateWindowSurface();

		sdl.ProcessEvents();
		if (sdl.QuitRequested() || sdl.GetTicks() - p > time) break;
	}
	
}

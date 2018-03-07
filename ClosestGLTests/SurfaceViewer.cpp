#include "stdafx.h"
#include "SurfaceViewer.h"

void ClosestGLTests::Tools::ViewSurface(TestTex & sur, uint64_t time)
{
	SDL::SDLInstance sdl;

	auto[w, h] = sur.GetSize();

	SDL::Window window
	{
		"ClosestGL Demo",
		SDL::Rect<int>{ SDL::Window::Center,SDL::Window::Center,int(w),int(h) },
		SDL::Window::WindowFlag::Null
	};



	auto p = sdl.GetTicks();
	while (true)
	{
		window.GetWindowSurface().Shade(
			[&sur](int x, int y, auto&, auto&)
		{
			auto[r, g, b, a] = sur.AccessPixelUnsafe({ size_t(x),size_t(y) });
			return SDL::Color<uint8_t>{ r, g, b, a };
		}
		);

		window.UpdateWindowSurface();

		sdl.ProcessEvents();
		if (sdl.QuitRequested() || sdl.GetTicks() - p > time) break;
		sdl.Delay(64);
	}
	
}

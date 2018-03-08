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
			return SDL::Color<uint8_t>{ col.x, col.y, col.z, col.w };
		}
		);

		window.UpdateWindowSurface();

		sdl.ProcessEvents();
		if (sdl.QuitRequested() || sdl.GetTicks() - p > time) break;
		sdl.Delay(64);
	}
	
}

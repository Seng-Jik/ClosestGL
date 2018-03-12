#include "stdafx.h"
#include "SurfaceViewer.h"
#include "../SDL2/include/SDL.h"

void BlitToSurface(SDL_Surface* sur, ClosestGLTests::Tools::TestTex& tex);

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
		BlitToSurface(window.GetWindowSurface().GetPtrToSDL_Surface(), sur);

		window.UpdateWindowSurface();

		sdl.ProcessEvents();
		if (sdl.QuitRequested() || sdl.GetTicks() - p > time) break;
	}
	
}

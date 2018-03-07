#include "stdafx.h"
#include "SurfaceViewer.h"

void ClosestGLTests::Tools::ViewSurface()
{
	SDL::SDLInstance sdl;
	SDL::Window window
	{
		"ClosestGL Demo",
		SDL::Rect<int>{ SDL::Window::Center,SDL::Window::Center,1024,768 },
		SDL::Window::WindowFlag::Null
	};
}

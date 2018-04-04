#include <SDLInstance.h>
#include <Window.h>
#include <../SDLClasses/include/Vector4.h>
#include <Texture2D.h>
#include <iostream>
#include <algorithm>
#include <SingleThreadRunner.h>
#include <../ClosestGL/Vector4.h>
#include <../SDLClasses/include/Vector2.h>
#include <ExampleCommon.h>
#include <SDLError.h>

using namespace ClosestGL;
using namespace ExampleCommon;

using Color = Math::Vector4<float>;




int __cdecl main()
{
	constexpr Math::Vector2<int> ScreenSize{ 640,480 };

	SDL::SDLInstance sdl;
	SDL::Window window
	{ 
		"SDFFont Demo",
		SDL::Rect<int>
		{
			SDL::Window::Center,
			SDL::Window::Center,
			ScreenSize.x,
			ScreenSize.y
		},
		SDL::Window::WindowFlag::Null
	};


	ParallelStrategy::SingleThreadRunner runner;
	

	Texture::Texture2D<Color> renderTarget{ {640,480} };

	auto font = LoadBMP24<Color>("msyh0.bmp",runner);
	

	while (!sdl.QuitRequested())
	{
		//renderTarget.Clear({ 1,1,1,0 }, runner);

		renderTarget.Shade([&font](auto pos)
		{
			if (pos.x < font.GetSize().x && pos.y < font.GetSize().y)
				return font.ReadPixelUnsafe(pos);
			else
				return Color{ 0,0,0,0 };
		},runner);

		UpdateWindow(window, renderTarget);
	}

	return 0;
}

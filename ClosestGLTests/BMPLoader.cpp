#include "stdafx.h"
#include "BMPLoader.h"
#include "..\SDL2\include\SDL.h"
using namespace ClosestGLTests;

Tools::TestTex ClosestGLTests::Tools::LoadBMPFile(const std::string & path)
{
	SDL::Surface sur("Assets\\" + path + ".bmp");
	auto size = sur.GetSize();

	SDL_Surface* pSur = sur.GetPtrToSDL_Surface();

	Tools::TestTex ret{ {size_t(size.x),size_t(size.y)} };

	for (int y = 0; y < pSur->h; ++y)
	{
		for (int x = 0; x < pSur->w; ++x)
		{
			auto offset = y * pSur->pitch + x * pSur->format->BytesPerPixel;
			auto px = ((Uint8*)pSur->pixels) + offset;
			auto col = *(Uint32*)px;

			Uint8 r, g, b;
			SDL_GetRGB(col, pSur->format, &r, &g, &b);

			ret.AccessPixelUnsafe({ size_t(x),size_t(y) }) =
				Tools::TestCol
			{
				r / 255.0f,
				g / 255.0f,
				b / 255.0f,
				1
			};
		}
	}

	return ret;
}

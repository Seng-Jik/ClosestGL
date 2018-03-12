#include "stdafx.h"
#include "ModelViewer.h"
#include <Vector3.h>
#include <MatrixTransform.h>
#include "../SDL2/include/SDL.h"

void BlitToSurface(SDL_Surface* sur, ClosestGLTests::Tools::TestTex& tex)
{
	for (int y = 0; y < sur->h; ++y) {
		for (int x = 0; x < sur->w; ++x) {
			auto px = static_cast<Uint8*>(sur->pixels);
			px += sur->pitch * y;
			px += sur->format->BytesPerPixel * x;

			auto newCol = tex.AccessPixelUnsafe(ClosestGL::Math::Vector2<size_t>{size_t(x), size_t(y)});
			const uint8_t r = uint8_t(newCol.x * 255);
			const uint8_t g = uint8_t(newCol.y * 255);
			const uint8_t b = uint8_t(newCol.z * 255);
			px[0] = b;
			px[1] = g;
			px[2] = r;
		}
	}
}


void ClosestGLTests::Tools::ViewModel(TestTex & sur, const ModelRenderer & renderer, uint64_t time)
{
	SDL::SDLInstance sdl;

	auto s = sur.GetSize();

	SDL::Window window
	{
		"",
		SDL::Rect<int>{ SDL::Window::Center,SDL::Window::Center,int(s.x),int(s.y) },
		SDL::Window::WindowFlag::Null
	};

	ClosestGL::Math::Vector3<float> transform{ 0,0,0 }, rotate{ 0,0,0 };
	using Matrix4 = ClosestGL::Math::Matrix4<float>;
	using Matrix3 = ClosestGL::Math::Matrix3<float>;

	SDL::Mouse mouse;
	SDL::Keyboard keyboard;

	bool killWithTimeOut = true;
	auto p = sdl.GetTicks();
	auto lastMouse = mouse.GetMouseState();

	int fpsCounter = 0;
	auto fpsTimer = sdl.GetTicks();
	while (true)
	{
		fpsCounter++;
		if (sdl.GetTicks() - fpsTimer >= 1000)
		{
			fpsTimer = sdl.GetTicks();
			window.SetWindowTitle("FPS:" + std::to_string(fpsCounter));
			
			fpsCounter = 0;
		}

		auto trans = ClosestGL::Math::GetTransformMatrix(transform);
		auto rotX = Matrix4(ClosestGL::Math::GetXRotateMatrix(rotate.x));
		auto rotY = Matrix4(ClosestGL::Math::GetYRotateMatrix(rotate.y));
		auto rotZ = Matrix4(Matrix3(ClosestGL::Math::GetZRotateMatrix(rotate.z)));

		auto rot = rotX * (rotY * rotZ);
		auto model = trans*rot;

		renderer(model);

		BlitToSurface(window.GetWindowSurface().GetPtrToSDL_Surface(), sur);



		window.UpdateWindowSurface();

		sdl.ProcessEvents();

		{
			auto mouseState = mouse.GetMouseState();
			if (mouseState.leftButton) 
			{
				killWithTimeOut = false;
				rotate.y -= (mouseState.position.x - lastMouse.position.x) / 200.0f;
				rotate.x += (mouseState.position.y - lastMouse.position.y) / 200.0f;
			}

			lastMouse = mouseState;
		}

		{
			if (keyboard.KeyPressed("Z"))
			{
				killWithTimeOut = false;
				rotate.z += 0.05f;
			}

			if (keyboard.KeyPressed("X"))
			{
				killWithTimeOut = false;
				rotate.z -= 0.05f;
			}

			if (keyboard.KeyPressed("W"))
			{
				killWithTimeOut = false;
				transform.z += 1;
			}

			if (keyboard.KeyPressed("S"))
			{
				killWithTimeOut = false;
				transform.z -= 1;
			}

			if (keyboard.KeyPressed("A"))
			{
				killWithTimeOut = false;
				transform.x -= 1;
			}

			if (keyboard.KeyPressed("D"))
			{
				killWithTimeOut = false;
				transform.x += 1;
			}
		}

		if (sdl.QuitRequested()) break;

		if (killWithTimeOut && sdl.GetTicks() - p > time)
			break;
	}
}

#include <Transformers.h>
#include <PrimitiveListReader.h>
#include <SingleThreadRunner.h>
std::vector<size_t> ClosestGLTests::Tools::WireFrameViewer::ConvertTranglesToLines(const std::vector<size_t>& ibo)
{
	//·Ö×é
	ClosestGL::Primitive::PrimitiveListReader<3> lr(ibo.data(), ibo.size());

	using Primitive = decltype(lr.Read());

	std::vector<Primitive> pris;
	std::vector<std::vector<size_t>> resultSet;
	while (lr.CanRead())
		pris.push_back(lr.Read());

	ClosestGL::Primitive::RebuildTransform(
		[](const Primitive& input, std::vector<size_t>& output)
	{
		output.push_back(input[0]);
		output.push_back(input[1]);
		output.push_back(input[0]);
		output.push_back(input[2]);
		output.push_back(input[1]);
		output.push_back(input[2]);
	}, pris.data(), pris.size(), resultSet, ClosestGL::ParallelStrategy::SingleThreadRunner{});
	return resultSet.at(0);
}

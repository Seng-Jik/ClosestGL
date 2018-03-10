#include "stdafx.h"
#include "ModelViewer.h"
#include <Vector3.h>
#include <MatrixTransform.h>


void ClosestGLTests::Tools::ViewModel(TestTex & sur, const ModelRenderer & renderer, uint64_t time)
{
	SDL::SDLInstance sdl;

	auto s = sur.GetSize();

	SDL::Window window
	{
		"ClosestGL Demo",
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
	while (true)
	{
		auto trans = ClosestGL::Math::GetTransformMatrix(transform);
		auto rotX = Matrix4(ClosestGL::Math::GetXRotateMatrix(rotate.x));
		auto rotY = Matrix4(ClosestGL::Math::GetYRotateMatrix(rotate.y));
		auto rotZ = Matrix4(Matrix3(ClosestGL::Math::GetZRotateMatrix(rotate.z)));

		auto rot = rotX * (rotY * rotZ);
		auto model = trans*rot;

		renderer(model);

		window.GetWindowSurface().Shade(
			[&sur](int x, int y, auto&, auto&)
		{
			auto col = sur.AccessPixelUnsafe({ size_t(x),size_t(y) });
			return SDL::Color<uint8_t>{ uint8_t(col.x * 255), uint8_t(col.y * 255), uint8_t(col.z * 255), uint8_t(col.w * 255) };
		}
		);

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

#include <RebuildTransform.h>
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

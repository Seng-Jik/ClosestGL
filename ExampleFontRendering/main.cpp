#include <SDLInstance.h>
#include <Window.h>
#include <../SDLClasses/include/Vector4.h>
#include <iostream>
#include <algorithm>
#include <../ClosestGL/Vector4.h>
#include <../SDLClasses/include/Vector2.h>
#include <ExampleCommon.h>
#include <SDLError.h>
#include <map>
#include <fstream>
#include <sstream>
#include <ClosestGL.h>
#include <type_traits>
#include <string>
#include <Keyboard.h>
using namespace ClosestGL;
using namespace ExampleCommon;

using Color = Math::Vector4<float>;

enum class FontMode
{
	Normal,
	Outline,
	Shadow
};

std::vector<std::wstring> Split(std::wstring str, wchar_t ch)
{
	size_t pos;
	std::vector<std::wstring> vec;
	while ((pos = str.find(ch)) != std::wstring::npos)
	{
		vec.push_back(str.substr(0, pos));
		str = str.substr(pos + 1, str.length() - pos - 1);
	}
	vec.push_back(str);
	return vec;
}

std::map<wchar_t, Math::Vector4<float>> LoadFontIndex(Math::Vector2<size_t> size)
{
	std::map<wchar_t, Math::Vector4<float>> map;

	const std::wstring index[]
	{
		L"C	faceID = 0	x = 0	y = 0	w = 45	h = 64",
		L"G	faceID = 0	x = 45	y = 0	w = 48	h = 64",
		L"L	faceID = 0	x = 93	y = 0	w = 38	h = 64",
		L"e	faceID = 0	x = 131	y = 0	w = 40	h = 64",
		L"l	faceID = 0	x = 171	y = 0	w = 26	h = 64",
		L"o	faceID = 0	x = 197	y = 0	w = 43	h = 64",
		L"s	faceID = 0	x = 0	y = 64	w = 35	h = 64",
		L"t	faceID = 0	x = 35	y = 64	w = 31	h = 64",
		L"体	faceID = 0	x = 66	y = 64	w = 60	h = 64",
		L"向	faceID = 0	x = 126	y = 64	w = 60	h = 64",
		L"场	faceID = 0	x = 186	y = 64	w = 60	h = 64",
		L"字	faceID = 0	x = 0	y = 128	w = 60	h = 64",
		L"染	faceID = 0	x = 60	y = 128	w = 60	h = 64",
		L"渲	faceID = 0	x = 120	y = 128	w = 60	h = 64",
		L"量	faceID = 0	x = 180	y = 128	w = 60	h = 64"
	};

	for (const auto& ch : index)
	{
		wchar_t chName = ch[0];

		const auto ppos = ch.rfind(L"x = ");
		auto position = ch.substr(ppos, ch.length() - ppos);

		position.erase(
			std::remove_if(position.begin(), position.end(), [](auto ch)
			{
				return
					ch == L'x' ||
					ch == L'y' ||
					ch == L'w' ||
					ch == L'h' ||
					ch == L' ' ||
					ch == L'=';
			}),
			position.end()
		);

		auto poses = Split(position, '\t');

		map[chName] = 
		{
			std::stof(poses[0]) / size.x,
			std::stof(poses[1]) / size.y,
			std::stof(poses[2]) / size.x,
			std::stof(poses[3]) / size.y
		};
	}

	return map;
}

void DrawChar(
	Math::Vector4<float> dstRect,
	Math::Vector4<float> uvRect,
	const Texture::Texture2D<Color>& tex,
	Texture::Texture2D<Color>& renderTargetTex,
	FontMode mode,
	float width)
{
	struct Vertex
	{
		Math::Vector4<float> SVPosition;
		Math::Vector2<float> UV;

		static Vertex Lerp(float t,const Vertex& v1, const Vertex& v2)
		{
			return Vertex{
				Math::Lerp(t,v1.SVPosition,v2.SVPosition),
				Math::Lerp(t,v1.UV,v2.UV)
			};
		}
	};

	const std::array<Vertex,4> rect =
	{
		Vertex{ { dstRect.x,dstRect.y,1,1 },{ uvRect.x,uvRect.y + uvRect.w } },
		Vertex{ { dstRect.x,dstRect.y + dstRect.w,1,1 },{ uvRect.x,uvRect.y } },
		Vertex{ { dstRect.x + dstRect.z,dstRect.y + dstRect.w,1,1 },{ uvRect.x + uvRect.z,uvRect.y } },
		Vertex{ { dstRect.x + dstRect.z,dstRect.y ,1,1 },{ uvRect.x + uvRect.z,uvRect.y + uvRect.w } },
	};

	size_t indicis[] = { 0,1,2,0,3,2 };

	const auto Blender = [](auto src, auto dst) {return ExampleCommon::BlendColorNormal(src, dst); };
	RenderPipeline::RenderTarget<1, Color, decltype(Blender)>
		renderTarget{ Blender,{ &renderTargetTex } };

	if (mode == FontMode::Shadow)
	{
		const auto ShadowPixelShader = [&tex, mode, width](const Vertex& v, const auto&)
		{
			Texture::Sampler::Sampler2D<
				std::remove_reference<decltype(tex)>::type,
				decltype(Texture::Sampler::UVNormalizer::UV2DClamp),
				decltype(Texture::Sampler::Fliters::Bilinear)>
				sampler{
				&tex,
				Texture::Sampler::UVNormalizer::UV2DClamp,
				Texture::Sampler::Fliters::Bilinear };

			const float distance = sampler.Sample(v.UV).x;
			const float power = std::clamp(distance - width,0.0f,1.0f) / (1 - width);

			return std::array<Color, 1>{ {0, 0, 0, power * 2} };
		};

		auto shadowRect = rect;
		for (size_t i = 0;i < shadowRect.size();++i)
			shadowRect[i].SVPosition += Math::Vector4<float>{0.01f, -0.02f, 0, 0};

		RenderPipeline::PixelShaderStage<decltype(renderTarget), decltype(ShadowPixelShader)>
			ps{ &renderTarget,ShadowPixelShader };
		RenderPipeline::TriangleRasterizer<decltype(ps), float>
			raster{ &ps };

		Primitive::PrimitiveListReader<3> reader{ indicis,sizeof(indicis) / sizeof(size_t) };

		raster.EmitPrimitive(reader, shadowRect.data(), shadowRect.size(), ParallelStrategy::SingleThreadRunner{});
	}

	const auto PixelShader = [&tex,mode, width](const Vertex& v,const auto&)
	{
		Texture::Sampler::Sampler2D<
			std::remove_reference<decltype(tex)>::type,
			decltype(Texture::Sampler::UVNormalizer::UV2DClamp),
			decltype(Texture::Sampler::Fliters::Bilinear)>
			sampler{
			&tex,
			Texture::Sampler::UVNormalizer::UV2DClamp,
			Texture::Sampler::Fliters::Bilinear};

		const float distance = sampler.Sample(v.UV).x;

		Color col{ 0,0,0,0 };

		if (mode == FontMode::Outline)
		{
			//width ~ width + 0.2f
			float power = (std::clamp(distance, width - 0.2f, width) - (width - 0.2f)) / 0.2f;

			col = { 1, 0.5f, 0.6f, power };
		}

		const float power = std::clamp((distance - width) / 0.05f, 0.0f, 1.0f);
		col = ExampleCommon::BlendColorNormal(Color{ 0, 0, 0, power }, col);
		

		return std::array<Color, 1>{ col };
	};


	RenderPipeline::PixelShaderStage<decltype(renderTarget), decltype(PixelShader)>
		ps{ &renderTarget,PixelShader };
	RenderPipeline::TriangleRasterizer<decltype(ps), float>
		raster{ &ps };

	Primitive::PrimitiveListReader<3> reader{ indicis,sizeof(indicis) / sizeof(size_t) };

	raster.EmitPrimitive(reader, rect.data(), rect.size(), ParallelStrategy::SingleThreadRunner{});
}

void DrawText(
	const std::wstring& text,
	const std::map<wchar_t,Math::Vector4<float>>& fontIndicis,
	const Texture::Texture2D<Color>& tex,
	Texture::Texture2D<Color>& renderTarget,
	float startX,float size, FontMode mode,float width)
{
	float x = startX;

	for (auto ch : text)
	{
		const auto uvRect = fontIndicis.at(ch);
		const Math::Vector4<float> pos
		{
			x,-0.5F,size*uvRect.z/2.5F,size*uvRect.w/2
		};

		x += size*uvRect.z / 2.5F;

		DrawChar(pos, uvRect, tex, renderTarget, mode,width);
	}
}

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
	auto fontIndicis = LoadFontIndex(font.GetSize());
	
	SDL::Keyboard keyboard;

	float size = 2;
	float startX = -0.75f;
	float width = 0.45f;
	FontMode mode = FontMode::Normal;
	while (!sdl.QuitRequested())
	{
		renderTarget.Clear({ 1,1,1,1 }, runner);

		DrawText(L"ClosestGL向量场字体渲染", fontIndicis, font, renderTarget, startX, size, mode, width);

		if (keyboard.KeyPressed("W"))
			size += 0.01f;
		if (keyboard.KeyPressed("S"))
			size -= 0.01f;
		if (keyboard.KeyPressed("A"))
			startX += 0.01f;
		if (keyboard.KeyPressed("D"))
			startX -= 0.01f;

		if (keyboard.KeyPressed("Q"))
			width += 0.001f;
		if (keyboard.KeyPressed("E"))
			width -= 0.001f;

		if (keyboard.KeyPressed("Z"))
			mode = FontMode::Normal;
		if (keyboard.KeyPressed("X"))
			mode = FontMode::Outline;
		if (keyboard.KeyPressed("C"))
			mode = FontMode::Shadow;

		UpdateWindow(window, renderTarget);
	}

	return 0;
}

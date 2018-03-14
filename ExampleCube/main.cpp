
/* ClosestGL Example - Cube
 * �����Ʒ��Mini3D��https://github.com/skywind3000/mini3d
 */

#include <ClosestGL.h>
#include <SDLInstance.h>
#include <Window.h>
#include <Mouse.h>
#include <Keyboard.h>
#include "..\SDLClasses\include\Vector4.h"

using namespace ClosestGL;

//�����������������
using Color = Math::Vector4<float>;
using Depth = float;


//��������SDL����
void UpdateWindow(SDL::Window& window, Texture::Texture2D<Color>& tex)
{
	window.GetWindowSurface().Shade(
		[&tex](int x, int y, auto&, auto)
	{
		auto color = tex.AccessPixelUnsafe({ size_t(x), size_t(y) });
		return SDL::Color<uint8_t>{
			uint8_t(color.x * 255),
			uint8_t(color.y * 255),
			uint8_t(color.z * 255),
			255
		};
	});

	window.UpdateWindowSurface();
}

//������������
struct VertexIn
{
	Math::Vector4<float> Position;
	Math::Vector2<float> TexCoord;
	Color Color;
};

constexpr std::array<VertexIn, 8> Mesh
{
	VertexIn{ { 1, -1,  1, 1 },{ 0, 0 },{ 1.0f, 0.2f, 0.2f } },
	VertexIn{ { -1, -1,  1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 0.2f } },
	VertexIn{ { -1,  1,  1, 1 },{ 1, 1 },{ 0.2f, 0.2f, 1.0f } },
	VertexIn{ { 1,  1,  1, 1 },{ 1, 0 },{ 1.0f, 0.2f, 1.0f } },
	VertexIn{ { 1, -1, -1, 1 },{ 0, 0 },{ 1.0f, 1.0f, 0.2f } },
	VertexIn{ { -1, -1, -1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 1.0f } },
	VertexIn{ { -1,  1, -1, 1 },{ 1, 1 },{ 1.0f, 0.3f, 0.3f } },
	VertexIn{ { 1,  1, -1, 1 },{ 1, 0 },{ 0.2f, 1.0f, 0.3f } },
};

constexpr std::array<size_t,36> Indicis
{
	0,1,2,0,3,2,
	4,5,6,4,7,6,
	0,4,5,0,1,5,
	1,5,6,1,2,6,
	2,6,7,2,3,7,
	3,7,4,3,0,4
};


//�����������
struct VertexOut
{
	Math::Vector4<float> SVPosition;
	Color Color;
	Math::Vector2<float> TexCoord;

	//͸��������
	RenderPipeline::PerspectiveCorrector::InPixelShader<float>
		PerspectiveCorrector;

	//��ֵ����
	static VertexOut Lerp(float x,const VertexOut& p1, const VertexOut& p2)
	{
		return{
			Math::Lerp(x,p1.SVPosition,p2.SVPosition),
			Math::Lerp(x,p1.Color,p2.Color),
			Math::Lerp(x,p1.TexCoord,p2.TexCoord),
			Math::Lerp(x,p1.PerspectiveCorrector,p2.PerspectiveCorrector)
		};
	}
};

int main()
{
	constexpr Math::Vector2<size_t> screenSize{ 800,600 };

	//���߳�ִ�в���
	ParallelStrategy::SingleThreadRunner runner;

	//׼����������Ͳ�����
	Texture::Texture2D<Color>
		texture{ {256,256} };

	texture.Shade([](auto pos) {
		bool white = false;
		if (pos.x % 32 > 16) white = !white;
		if (pos.y % 32 > 16) white = !white;
		return white ? 
			Color{ 1,1,1,1 } : 
			Color{ 63 / 255.0f,188 / 255.0f,239 / 255.0f };
	}, runner);

	Texture::Sampler::Sampler2D<
		decltype(texture),
		decltype(Texture::Sampler::UVNormalizer::UV2DClamp),
		decltype(Texture::Sampler::Fliters::Bilinear)>
		sampler(
			&texture,
			Texture::Sampler::UVNormalizer::UV2DClamp,
			Texture::Sampler::Fliters::Bilinear);


	//׼����ȾĿ��
	Texture::Texture2D<Color> 
		colorBuffer{ screenSize };
	Texture::Texture2D<Depth>
		depthBuffer{ screenSize };

	const auto blender = 
		[](auto src, auto dst) {return src; };

	RenderPipeline::RenderTarget<1, Color, decltype(blender)>
		renderTarget{ blender,{&colorBuffer} };

	//��Ⱦ�������õ���Ⱦ����
	const auto pixelShaderFuncTex = 
		[&sampler](const VertexOut& v)
	{
		return std::array<Color, 1>
		{
			sampler.Sample(v.PerspectiveCorrector(v.TexCoord))
		};
	};

	RenderPipeline::PixelShader<decltype(renderTarget), decltype(pixelShaderFuncTex)>
		pixelShaderTex(&renderTarget, pixelShaderFuncTex);

	RenderPipeline::DepthTest<decltype(pixelShaderTex), Depth>
		depthTestTex{ &pixelShaderTex,&depthBuffer };

	RenderPipeline::TriangleRasterizer<decltype(depthTestTex), float>
		rasterTex{ &depthTestTex };

	//��Ⱦ��ɫ�����õ���Ⱦ����
	const auto pixelShaderFuncColor = 
		[](const VertexOut& v)
	{
		return std::array<Color, 1>
		{
			v.Color
		};
	};

	RenderPipeline::PixelShader<decltype(renderTarget), decltype(pixelShaderFuncColor)>
		pixelShaderColor{ &renderTarget,pixelShaderFuncColor };

	RenderPipeline::DepthTest<decltype(pixelShaderColor), float>
		depthTestCol{ &pixelShaderColor,&depthBuffer };

	RenderPipeline::TriangleRasterizer<decltype(depthTestCol), float>
		rasterCol{ &depthTestCol };

	//��Ⱦ�߿��õ���Ⱦ����
	const auto pixelShaderFuncWireFrame = 
		[](const auto&)
	{
		return std::array<Color, 1>
		{
			Color{0,0,0,0}
		};
	};

	RenderPipeline::PixelShader<decltype(renderTarget), decltype(pixelShaderFuncWireFrame)>
		pixelShaderWireFrame{ &renderTarget,pixelShaderFuncWireFrame };

	RenderPipeline::LineRasterizer<decltype(pixelShaderWireFrame)
	
	//׼��ͶӰ����
	const auto projection =
		Math::GetPerspectiveMatrixLH<float>(
			3.1415926f * 0.5f,
			screenSize.x / float(screenSize.y),
			1.0f, 
			500.0f);

	//׼����ͼ����
	auto view =
		Math::GetLookAtMatrix<float>(
			{ 3,0,0 },
			{ 1,0,0 },
			{ 0,0,1 });
	
	//׼������
	SDL::SDLInstance sdl;
	SDL::Window window(
		"ClosestGL Example - Cube",
		SDL::Rect<int32_t>{
			SDL::Window::Center,
			SDL::Window::Center,
			int(screenSize.x),
			int(screenSize.y)
		},
		SDL::Window::WindowFlag::Null);

	//�ѱ任��Vertex
	std::array<VertexOut, Mesh.size()> vOut;

	//��ת�Ƕ�
	float rotX = 0.35f, rotY = 0.9f;

	//����豸
	SDL::Mouse mouse;
	auto mousePos = mouse.GetMouseState().position;

	//�����豸
	SDL::Keyboard keyboard;

	enum class RenderMode : int
	{
		Texture = 0,Color = 1,WireFrame = 2
	};

	RenderMode renderMode = RenderMode::Texture;

	//��ѭ��
	while (!sdl.QuitRequested())
	{
		//���Ʊ���ɫ
		colorBuffer.Shade([screenSize](auto pos) {
			float col = 1 - pos.y / float(screenSize.y);
			return Color{ col,col,col,1 };
		}, runner);

		//�����Ȼ���
		depthBuffer.Clear(0, runner);

		//�������
		const auto world = 
			Math::Matrix4<float>
			(Math::GetYRotateMatrix(rotY) * Math::Matrix3<float>(Math::GetZRotateMatrix(rotX)));

		//�任����
		const auto transform = (projection * view) * world;

		//������ɫ��
		Primitive::FixedTransform(
			[&transform](const VertexIn& v)
		{
			auto pos = transform * v.Position;

			RenderPipeline::PerspectiveCorrector::BeforePerspectiveDivision<float>
				uvfix(pos);

			return VertexOut {
				pos,
				uvfix(v.Color),
				uvfix(v.TexCoord),
				RenderPipeline::PerspectiveCorrector::InPixelShader<float>
					{ uvfix }
			};
		}, Mesh.data(), vOut.data(), Mesh.size(), runner);

		switch (renderMode)
		{
		case RenderMode::Color: {
			Primitive::PrimitiveListReader<3> reader{ Indicis.data(),Indicis.size() };
			rasterCol.EmitPrimitive(reader, vOut.data(), vOut.size(), runner);	//����ɫ�ʷ���
			break;
		}
		case RenderMode::Texture: {
			Primitive::PrimitiveListReader<3> reader{ Indicis.data(),Indicis.size() };
			rasterTex.EmitPrimitive(reader, vOut.data(), vOut.size(), runner);	//����������
			break;
		}
		case RenderMode::WireFrame:
			break;
		}

		UpdateWindow(window, colorBuffer);

		//���̲���
		{
			//����Z�л���Ⱦ��ʽ
			if(keyboard.KeyPressed("Z"))
				renderMode = RenderMode((((int)renderMode) + 1) % 3);
		}


		//������
		{
			//�ƶ�
			auto mouseState = mouse.GetMouseState();
			if (mouseState.leftButton)
			{
				rotX -= (mouseState.position.x - mousePos.x) * 0.025f;
				rotY += (mouseState.position.y - mousePos.y) * 0.025f;
			}
			mousePos = mouseState.position;
		}
	}

	return 0;
}

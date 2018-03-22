
/* ClosestGL Example - Cube
 * �����Ʒ��Mini3D��https://github.com/skywind3000/mini3d
 */

#include <optional>
#include <ClosestGL.h>
#include <SDLInstance.h>
#include <Window.h>
#include <Mouse.h>
#include <random>
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
			uint8_t(std::min(1.0f, color.x) * 255),
			uint8_t(std::min(1.0f, color.y) * 255),
			uint8_t(std::min(1.0f, color.z) * 255),
			255
		};
	});

	window.UpdateWindowSurface();
}

//������������
struct VertexIn
{
	Math::Vector4<float> Position;
	Color Color;
	Math::Vector2<float> TexCoord;	//������DrawPlane��������
};

std::array<VertexIn, 8> Mesh
{
	VertexIn{ { 1, -1,  1, 1 },{ 1.0f, 0.2f, 0.2f } },
	VertexIn{ { -1, -1,  1, 1 },{ 0.2f, 1.0f, 0.2f } },
	VertexIn{ { -1,  1,  1, 1 },{ 0.2f, 0.2f, 1.0f } },
	VertexIn{ { 1,  1,  1, 1 },{ 1.0f, 0.2f, 1.0f } },
	VertexIn{ { 1, -1, -1, 1 },{ 1.0f, 1.0f, 0.2f } },
	VertexIn{ { -1, -1, -1, 1 },{ 0.2f, 1.0f, 1.0f } },
	VertexIn{ { -1,  1, -1, 1 },{ 1.0f, 0.3f, 0.3f } },
	VertexIn{ { 1,  1, -1, 1 },{ 0.2f, 1.0f, 0.3f } },
};

//�ı�����������
constexpr std::array<size_t,36> Indicis
{
	0,1,2,3,
	7,6,5,4,
	0,4,5,1,
	1,5,6,2,
	2,6,7,3,
	3,7,4,0
};


//�����������
struct VertexOut
{
	Math::Vector4<float> SVPosition;
	Color Color;
	Math::Vector2<float> TexCoord;
	Math::Vector4<float> Normal;
	Math::Vector4<float> WorldPosition;
	Math::Vector4<float> ScreenSpeed;
	Math::Vector3<float> Tangent,BiTangent;

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
			Math::Lerp(x,p1.Normal,p2.Normal),
			Math::Lerp(x,p1.WorldPosition,p2.WorldPosition),
			Math::Lerp(x,p1.ScreenSpeed,p2.ScreenSpeed),
			Math::Lerp(x,p1.Tangent,p2.Tangent),
			Math::Lerp(x,p1.BiTangent,p2.BiTangent),
			Math::Lerp(x,p1.PerspectiveCorrector,p2.PerspectiveCorrector)
		};
	}
};

//����һ��ƽ��
template<typename TRasterizer,typename TRunner>
void DrawPlane(
	TRasterizer& raster,
	std::array<size_t, 4> quad,
	const Math::Matrix4<float>& vp,
	const Math::Matrix4<float>& world,
	const Math::Matrix4<float>& lastFrameMVP,
	TRunner& runner)
{
	//����
	auto u = Mesh[quad[1]].Position - Mesh[quad[0]].Position;
	auto v = Mesh[quad[2]].Position - Mesh[quad[0]].Position;
	Math::Vector3<float> u3{ u.x,u.y,u.z };
	Math::Vector3<float> v3{ v.x,v.y,v.z };
	auto normal3 = Normalize(Math::Cross(u3, v3));
	auto normal = world * Math::Vector4<float>{ normal3.x, normal3.y, normal3.z, 1 };

	//����ƽ���õ�VertexShader
	const auto vertexShader = [&vp,&world, normal,u3,v3,&lastFrameMVP](const VertexIn& v)
	{
		auto pos = (vp * world) * v.Position;
		
		auto lastFramePos = lastFrameMVP * v.Position;

		RenderPipeline::PerspectiveCorrector::BeforePerspectiveDivision<float>
			uvfix(pos);

		return VertexOut{
			pos,
			uvfix(v.Color),
			uvfix(v.TexCoord),
			uvfix(normal),
			uvfix(world * v.Position),
			uvfix(lastFramePos - pos),
			uvfix(u3),
			uvfix(v3),
			RenderPipeline::PerspectiveCorrector::InPixelShader<float>
			{ uvfix }
		};
	};

	//����UV
	Mesh[quad[0]].TexCoord = { 0,0 };
	Mesh[quad[1]].TexCoord = { 0,1 };
	Mesh[quad[2]].TexCoord = { 1,1 };
	Mesh[quad[3]].TexCoord = { 1,0 };

	std::vector<VertexOut> outs;

	//Ӧ��VertexShader
	for (int i = 0; i < 4; ++i)
	{
		outs.push_back(vertexShader(Mesh[quad[i]]));
	}

	size_t ibo[] = { 0,1,2,0,3,2 };

	Primitive::PrimitiveListReader<3> 
		reader{ ibo,6 };
	
	//����ɫ�ʷ���
	raster.EmitPrimitive(reader, outs.data(), outs.size(), runner);
}

//����
float Lambert(
	Math::Vector4<float> normal,
	Math::Vector4<float> lightDirection)
{
	return
		std::max(
			0.0f,
			Math::Dot(
				Math::Normalize(normal),
				Math::Normalize(lightDirection)));
}

float BlinPhong(
	Math::Vector4<float> normal,
	Math::Vector4<float> lightDirection,
	Math::Vector4<float> viewDirection,
	float specPower)
{
	const auto diff = Lambert(normal, lightDirection);
	Math::Vector4<float> halfVector = Math::Normalize(lightDirection + viewDirection);
	const auto spec = std::powf(
		std::max(
			0.0f, 
			Math::Dot(
				halfVector,
				Normalize(normal)))
		, specPower);

	return diff + spec;
}

//������ͼ
Math::Vector3<float> NormalMap(
	Math::Vector3<float> baseNormal,
	Math::Vector3<float> tangent,
	Math::Vector3<float> bitangent,
	Math::Vector3<float> mappedNormal)
{
	mappedNormal *= 2.0f;
	mappedNormal -= Math::Vector3<float>{1.0f, 1.0f, 1.0f};

	Math::Matrix3<float> mat3
	{
		tangent,
		bitangent,
		baseNormal
	};

	return mat3 * mappedNormal;
}

//�˶�ģ��
void MotionBlur(
	Texture::Texture2D<Color>& renderBuffer,
	const Texture::Texture2D<Color>& colorBuffer, 
	const Texture::Texture2D<Color>& speedBuffer)
{
	Texture::Sampler::Sampler2D<
		Texture::Texture2D<Color>,
		decltype(Texture::Sampler::UVNormalizer::UV2DClamp),
		decltype(Texture::Sampler::Fliters::Bilinear)>
		colorSampler(
			&colorBuffer,
			Texture::Sampler::UVNormalizer::UV2DClamp,
			Texture::Sampler::Fliters::Bilinear);

	ParallelStrategy::SingleThreadRunner runner;
	renderBuffer.Shade(
		[&](auto pos)
	{
		const auto speed = speedBuffer.ReadPixelUnsafe(pos);
		Math::Vector2<float> uv
		{
			float(pos.x) / float(colorBuffer.GetSize().x),
			float(pos.y) / float(colorBuffer.GetSize().y)
		};

		Color col{ 0.0f,0.0f,0.0f,0.0f };

		if (
			speed.x > 0.01f ||
			speed.y > 0.01f || 
			speed.x < -0.01f ||
			speed.y < -0.01f)
		{
			constexpr float weight[4] =
			{ 0.4f, 0.3f, 0.2f,0.1f};
			constexpr int samples = 8;
			for (int i = 0; i < samples; ++i)
			{
				col += colorSampler.Sample(uv) * 0.5f * weight[i / 2];
				uv -= Math::Vector2<float>{ speed.x, speed.y } *0.01f;
			}
		}
		else
			col = colorBuffer.ReadPixelUnsafe(pos);

		auto orgCol = renderBuffer.ReadPixelUnsafe(pos);
		return col * 0.65f + orgCol * 0.35f;
	}, runner);

	runner.Wait();
}

int main()
{
	constexpr Math::Vector2<size_t> screenSize{ 800,600 };

	//ת���ı����������浽��������������
	std::vector<size_t> triIndicis;
	{
		Primitive::PrimitiveListReader<4> 
			reader{ Indicis.data(),Indicis.size() };

		while (reader.CanRead())
		{
			auto p = reader.Read();
			triIndicis.push_back(p[0]);
			triIndicis.push_back(p[1]);
			triIndicis.push_back(p[2]);
			triIndicis.push_back(p[0]);
			triIndicis.push_back(p[3]);
			triIndicis.push_back(p[2]);
		}
	}

	//ת���������������浽����������
	std::vector<size_t> lineIndicis;
	{
		Primitive::PrimitiveListReader<3> 
			reader{ triIndicis.data(),triIndicis.size() };

		while (reader.CanRead())
		{
			auto p = reader.Read();
			lineIndicis.push_back(p[0]);
			lineIndicis.push_back(p[1]);
			lineIndicis.push_back(p[0]);
			lineIndicis.push_back(p[2]);
			lineIndicis.push_back(p[1]);
			lineIndicis.push_back(p[2]);
		}
	}

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

	//׼��������ͼ�Ͳ�����
	Texture::Texture2D<Color>
		normalMap{ {256,256} };

	normalMap.Shade([](auto pos) {
		Math::Vector3<float> normal{ 0,0,1 };
		if (pos.x % 16 == 0) normal.x -= 1;
		if (pos.y % 16 == 0) normal.y += 1;
		normal = Normalize(normal);
		
		normal /= 2.0f;
		normal += Math::Vector3<float>{0.5f, 0.5f, 0.5f};

		return Color{
			normal.x,
			normal.y,
			normal.z,
			1
		};
	}, runner);
	

	Texture::Sampler::Sampler2D<
		decltype(normalMap),
		decltype(Texture::Sampler::UVNormalizer::UV2DClamp),
		decltype(Texture::Sampler::Fliters::Bilinear)>
		normSampler(
			&normalMap,
			Texture::Sampler::UVNormalizer::UV2DClamp,
			Texture::Sampler::Fliters::Bilinear);

	//׼����ȾĿ��
	Texture::Texture2D<Color> 
		colorBuffer{ screenSize };
	Texture::Texture2D<Color>
		speedBuffer{ screenSize };
	Texture::Texture2D<Depth>
		depthBuffer{ screenSize };

	const auto blender = 
		[](auto src, auto dst) {return src; };

	RenderPipeline::RenderTarget<2, Color, decltype(blender)>
		renderTarget{ blender,{&colorBuffer,&speedBuffer} };

	//�۾�λ��
	Math::Vector4<float> viewPosition;


	//��Ⱦģʽ
	enum class RenderMode : int
	{
		Texture = 0, Color = 1, WireFrame = 2
	};
	RenderMode renderMode = RenderMode::Texture;

	//�Ƿ�������
	bool lighting = true;

	//�Ƿ���������ͼ
	bool normalMapping = true;

	//�Ƿ����˶�ģ��
	bool motionBlur = true;

	//��Ⱦ�����õ���Ⱦ����

	//ʹ������
	const auto pixelShaderFunc = 
		[&sampler,&viewPosition,&lighting,&normSampler,&normalMapping,&renderMode](const VertexOut& v)
	{

		auto uv = v.PerspectiveCorrector(v.TexCoord);

		auto col =
			renderMode == RenderMode::Color ?
			v.PerspectiveCorrector(v.Color) :
			sampler.Sample(uv);
		
		Math::Vector3<float> normal;
		{
			auto n = v.PerspectiveCorrector(v.Normal);
			normal.x = n.x;
			normal.y = n.y;
			normal.z = n.z;
		}

		if (normalMapping)
		{
			auto norm4 = normSampler.Sample(uv);
			Math::Vector3<float> norm3{
				norm4.x,
				norm4.y,
				norm4.z
			};
			normal = NormalMap(
				normal,
				v.PerspectiveCorrector(v.Tangent),
				v.PerspectiveCorrector(v.BiTangent),
				norm3);
		}

		if (lighting)
		{
			//��������
			const auto worldPosition = v.PerspectiveCorrector(v.WorldPosition);

			//�ƹ���������
			constexpr Math::Vector4<float> lightPosition{ 1.5,1,0 };

			//���ն�
			const auto lx = BlinPhong(
				Math::Vector4<float>{ normal.x, normal.y, normal.z, 1},
				worldPosition - lightPosition,
				worldPosition - viewPosition,
				70.0f) * 0.75f + 0.25f;

			col *= lx;
		}

		return std::array<Color, 2> { col,v.PerspectiveCorrector(v.ScreenSpeed) };
	};

	RenderPipeline::PixelShaderStage<decltype(renderTarget),decltype(pixelShaderFunc)>
		pixelShader(&renderTarget,pixelShaderFunc);

	RenderPipeline::DepthTestStage<decltype(pixelShader), Depth>
		depthTest{ &pixelShader,&depthBuffer };

	RenderPipeline::TriangleRasterizer<decltype(depthTest), float>
		raster{ &depthTest };
	

	//��Ⱦ�߿��õ���Ⱦ����
	const auto pixelShaderFuncWireFrame = 
		[](const auto&)
	{
		return std::array<Color, 2>
		{
			Color{ 0,0,0,0 },
			Color{ 0,0,0,0 }
		};
	};

	RenderPipeline::PixelShaderStage<decltype(renderTarget), decltype(pixelShaderFuncWireFrame)>
		pixelShaderWireFrame{ &renderTarget,pixelShaderFuncWireFrame };

	RenderPipeline::LineRasterizer<decltype(pixelShaderWireFrame), float>
		rasterWireFrame{ &pixelShaderWireFrame };
	
	//׼��ͶӰ����
	const auto projection =
		Math::GetPerspectiveMatrixLH<float>(
			3.1415926f * 0.5f,
			screenSize.x / float(screenSize.y),
			1.0f, 
			500.0f);
	
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

	//��ת�Ƕ�
	float rotX = 0.35f, rotY = 0.9f, eyeX = 3;

	//����豸
	SDL::Mouse mouse;
	auto mousePos = mouse.GetMouseState().position;

	//�����豸
	SDL::Keyboard keyboard;

	//ÿһ֡����һ֡����
	std::optional<Math::Matrix4<float>> lastFrameMVP;

	//ʹ���˶�ģ�����������
	Texture::Texture2D<Color> motionBlured{ screenSize };

	//��ѭ��
	while (!sdl.QuitRequested())
	{
		//���Ʊ���ɫ
		colorBuffer.Shade([screenSize](auto pos) {
			float col = 1 - pos.y / float(screenSize.y);
			return Color{ col,col,col,1 };
		}, runner);

		//�����Ȼ���
		depthTest.ClearDepthBuffer(runner);

		//����ٶȻ���
		speedBuffer.Clear(Math::Vector4<float>{0.0f, 0.0f, 0.0f, 0.0f}, runner);


		//�������
		const auto world = 
			Math::Matrix4<float>
			(Math::GetYRotateMatrix(rotY) * Math::Matrix3<float>(Math::GetZRotateMatrix(rotX)));

		viewPosition.x = eyeX;
		viewPosition.y = 0;
		viewPosition.z = 0;
		viewPosition.w = 1;

		//׼����ͼ����
		auto view =
			Math::GetLookAtMatrix<float>(
				{ viewPosition.x,viewPosition.y,viewPosition.z },
				{ 1,0,0 },
				{ 0,0,1 });

		//�任����
		const auto vp = projection * view;

		if (!lastFrameMVP.has_value())
			lastFrameMVP = vp * world;

		switch (renderMode)
		{
		case RenderMode::Color:
		case RenderMode::Texture:{
			Primitive::PrimitiveListReader<4> quads{ Indicis.data(),Indicis.size() };
			while (quads.CanRead())
			{
				auto quad = quads.Read();	//ȡ��ÿһ���ı���ͼԪ
				DrawPlane(raster, quad, vp, world, *lastFrameMVP, runner);
			}
			break;
		}
		case RenderMode::WireFrame: {

			//�����߿��VertexOut
			struct VertexOut
			{
				Math::Vector4<float> SVPosition;
				
				static VertexOut Lerp(float x, const VertexOut& p1, const VertexOut& p2)
				{
					return{
						Math::Lerp(x,p1.SVPosition,p2.SVPosition)
					};
				}
			};

			std::vector<VertexOut> vOut{ Mesh.size() };

			//Ӧ��VertexShader������vOut
			Primitive::FixedTransform(
				[&vp,&world](const VertexIn& v)
			{
				return VertexOut{
					(vp * world) * v.Position
				};
			},Mesh.data(),vOut.data(),Mesh.size(),runner);

			//���߶εķ�ʽ�����դ��
			Primitive::PrimitiveListReader<2> reader{ lineIndicis.data(),lineIndicis.size() };
			rasterWireFrame.EmitPrimitive(reader, vOut.data(), vOut.size(), runner);
			break;
		}
		}

		lastFrameMVP = vp * world;

		if (motionBlur)
		{
			MotionBlur(motionBlured, colorBuffer, speedBuffer);

			UpdateWindow(window, motionBlured);
		}
		else
			UpdateWindow(window, colorBuffer);

		//���̲���
		{
			//����ZXC�л���Ⱦ��ʽ
			if (keyboard.KeyPressed("Z"))
				renderMode = RenderMode::Texture;
			
			if (keyboard.KeyPressed("X"))
				renderMode = RenderMode::Color;
			
			if (keyboard.KeyPressed("C"))
				renderMode = RenderMode::WireFrame;

			//����VB�����ع���
			if (keyboard.KeyPressed("V")) lighting = true;
			if (keyboard.KeyPressed("B")) lighting = false;

			//��סW��S���ƶ�
			if (keyboard.KeyPressed("W"))
				eyeX -= 0.01f;
			if (keyboard.KeyPressed("S"))
				eyeX += 0.01f;

			//����K��L�������˶�ģ��
			if (keyboard.KeyPressed("K"))
				motionBlur = true;
			if (keyboard.KeyPressed("L"))
				motionBlur = false;

			//����N��M�Դ򿪺͹رշ�����ͼ
			if (keyboard.KeyPressed("N"))
				normalMapping = true;
			if (keyboard.KeyPressed("M"))
				normalMapping = false;
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

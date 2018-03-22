
/* ClosestGL Example - Cube
 * 仿制云风的Mini3D：https://github.com/skywind3000/mini3d
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

//定义基本的数据类型
using Color = Math::Vector4<float>;
using Depth = float;


//更新纹理到SDL窗口
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

//顶点输入数据
struct VertexIn
{
	Math::Vector4<float> Position;
	Color Color;
	Math::Vector2<float> TexCoord;	//以下由DrawPlane函数计算
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

//四边形索引缓存
constexpr std::array<size_t,36> Indicis
{
	0,1,2,3,
	7,6,5,4,
	0,4,5,1,
	1,5,6,2,
	2,6,7,3,
	3,7,4,0
};


//顶点输出数据
struct VertexOut
{
	Math::Vector4<float> SVPosition;
	Color Color;
	Math::Vector2<float> TexCoord;
	Math::Vector4<float> Normal;
	Math::Vector4<float> WorldPosition;
	Math::Vector4<float> ScreenSpeed;
	Math::Vector3<float> Tangent,BiTangent;

	//透视修正器
	RenderPipeline::PerspectiveCorrector::InPixelShader<float>
		PerspectiveCorrector;

	//插值函数
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

//绘制一个平面
template<typename TRasterizer,typename TRunner>
void DrawPlane(
	TRasterizer& raster,
	std::array<size_t, 4> quad,
	const Math::Matrix4<float>& vp,
	const Math::Matrix4<float>& world,
	const Math::Matrix4<float>& lastFrameMVP,
	TRunner& runner)
{
	//求法线
	auto u = Mesh[quad[1]].Position - Mesh[quad[0]].Position;
	auto v = Mesh[quad[2]].Position - Mesh[quad[0]].Position;
	Math::Vector3<float> u3{ u.x,u.y,u.z };
	Math::Vector3<float> v3{ v.x,v.y,v.z };
	auto normal3 = Normalize(Math::Cross(u3, v3));
	auto normal = world * Math::Vector4<float>{ normal3.x, normal3.y, normal3.z, 1 };

	//绘制平面用的VertexShader
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

	//计算UV
	Mesh[quad[0]].TexCoord = { 0,0 };
	Mesh[quad[1]].TexCoord = { 0,1 };
	Mesh[quad[2]].TexCoord = { 1,1 };
	Mesh[quad[3]].TexCoord = { 1,0 };

	std::vector<VertexOut> outs;

	//应用VertexShader
	for (int i = 0; i < 4; ++i)
	{
		outs.push_back(vertexShader(Mesh[quad[i]]));
	}

	size_t ibo[] = { 0,1,2,0,3,2 };

	Primitive::PrimitiveListReader<3> 
		reader{ ibo,6 };
	
	//绘制色彩方块
	raster.EmitPrimitive(reader, outs.data(), outs.size(), runner);
}

//光照
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

//法线贴图
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

//运动模糊
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

	//转换四边形索引缓存到三角形索引缓存
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

	//转换三角形索引缓存到线索引缓存
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

	//单线程执行策略
	ParallelStrategy::SingleThreadRunner runner;

	//准备棋盘纹理和采样器
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

	//准备法线贴图和采样器
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

	//准备渲染目标
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

	//眼睛位置
	Math::Vector4<float> viewPosition;


	//渲染模式
	enum class RenderMode : int
	{
		Texture = 0, Color = 1, WireFrame = 2
	};
	RenderMode renderMode = RenderMode::Texture;

	//是否开启光照
	bool lighting = true;

	//是否开启法线贴图
	bool normalMapping = true;

	//是否开启运动模糊
	bool motionBlur = true;

	//渲染方块用的渲染管线

	//使用纹理
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
			//世界坐标
			const auto worldPosition = v.PerspectiveCorrector(v.WorldPosition);

			//灯光世界坐标
			constexpr Math::Vector4<float> lightPosition{ 1.5,1,0 };

			//光照度
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
	

	//渲染线框用的渲染管线
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
	
	//准备投影矩阵
	const auto projection =
		Math::GetPerspectiveMatrixLH<float>(
			3.1415926f * 0.5f,
			screenSize.x / float(screenSize.y),
			1.0f, 
			500.0f);
	
	//准备窗体
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

	//旋转角度
	float rotX = 0.35f, rotY = 0.9f, eyeX = 3;

	//鼠标设备
	SDL::Mouse mouse;
	auto mousePos = mouse.GetMouseState().position;

	//键盘设备
	SDL::Keyboard keyboard;

	//每一帧的上一帧矩阵
	std::optional<Math::Matrix4<float>> lastFrameMVP;

	//使用运动模糊的输出纹理
	Texture::Texture2D<Color> motionBlured{ screenSize };

	//主循环
	while (!sdl.QuitRequested())
	{
		//绘制背景色
		colorBuffer.Shade([screenSize](auto pos) {
			float col = 1 - pos.y / float(screenSize.y);
			return Color{ col,col,col,1 };
		}, runner);

		//清空深度缓存
		depthTest.ClearDepthBuffer(runner);

		//清空速度缓存
		speedBuffer.Clear(Math::Vector4<float>{0.0f, 0.0f, 0.0f, 0.0f}, runner);


		//世界矩阵
		const auto world = 
			Math::Matrix4<float>
			(Math::GetYRotateMatrix(rotY) * Math::Matrix3<float>(Math::GetZRotateMatrix(rotX)));

		viewPosition.x = eyeX;
		viewPosition.y = 0;
		viewPosition.z = 0;
		viewPosition.w = 1;

		//准备视图矩阵
		auto view =
			Math::GetLookAtMatrix<float>(
				{ viewPosition.x,viewPosition.y,viewPosition.z },
				{ 1,0,0 },
				{ 0,0,1 });

		//变换矩阵
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
				auto quad = quads.Read();	//取出每一个四边形图元
				DrawPlane(raster, quad, vp, world, *lastFrameMVP, runner);
			}
			break;
		}
		case RenderMode::WireFrame: {

			//用于线框的VertexOut
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

			//应用VertexShader并存入vOut
			Primitive::FixedTransform(
				[&vp,&world](const VertexIn& v)
			{
				return VertexOut{
					(vp * world) * v.Position
				};
			},Mesh.data(),vOut.data(),Mesh.size(),runner);

			//以线段的方式传入光栅器
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

		//键盘操作
		{
			//按下ZXC切换渲染方式
			if (keyboard.KeyPressed("Z"))
				renderMode = RenderMode::Texture;
			
			if (keyboard.KeyPressed("X"))
				renderMode = RenderMode::Color;
			
			if (keyboard.KeyPressed("C"))
				renderMode = RenderMode::WireFrame;

			//按下VB来开关光照
			if (keyboard.KeyPressed("V")) lighting = true;
			if (keyboard.KeyPressed("B")) lighting = false;

			//按住W和S来移动
			if (keyboard.KeyPressed("W"))
				eyeX -= 0.01f;
			if (keyboard.KeyPressed("S"))
				eyeX += 0.01f;

			//按下K或L来开关运动模糊
			if (keyboard.KeyPressed("K"))
				motionBlur = true;
			if (keyboard.KeyPressed("L"))
				motionBlur = false;

			//按下N和M以打开和关闭法线贴图
			if (keyboard.KeyPressed("N"))
				normalMapping = true;
			if (keyboard.KeyPressed("M"))
				normalMapping = false;
		}


		//鼠标操作
		{
			//移动
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

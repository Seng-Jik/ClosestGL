#include "stdafx.h"
#include <MathCommon.h>
#include <RenderTarget.h>
#include <PixelShader.h>
#include <SingleThreadRunner.h>
#include <LineRasterizer.h>
#include <PrimitiveListReader.h>
#include "OBJLoader.h"
#include <MultiThreadRunner.h>
#include "ModelViewer.h"
#include <FixedTransform.h>
#include <MatrixTransform.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;
using namespace std::chrono_literals;
using namespace ClosestGLTests::Tools::WireFrameViewer;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(ModelWireFrameTest)
	{
	private:
		void ViewModel(const char* path,float scale)
		{
			auto obj = Tools::LoadModelOBJ<Vertex>(path);
			auto& vbo = std::get<0>(obj);
			auto ibo = ConvertTranglesToLines(std::get<1>(obj));

			//ParallelStrategy::SingleThreadRunner runner;
			ParallelStrategy::MultiThreadRunner runner(std::thread::hardware_concurrency());
			Tools::TestTex tex{ {1024,768} };
			RenderTarget rt{ Tools::Blenders::NoBlend,{&tex} };
			PixelShader ps{ &rt,PS{} };
			LineRasterizer raster = { &ps };
			Primitive::PrimitiveListReader<2> lineReader{ ibo.data(), ibo.size() };
			

			Tools::ViewModel(tex,
				[&vbo,&ibo,&raster,&tex,&lineReader,&runner, scale](const auto& view) {

				std::vector<Vertex> transformed{ vbo.size() };

				ClosestGL::Primitive::FixedTransform
				([scale,&view](const Vertex& v) {
					auto Scale = 
						ClosestGL::Math::Matrix4<float>
						(ClosestGL::Math::GetScaleMatrix(scale, scale, scale));

					auto MoveToCenter = 
						ClosestGL::Math::Matrix4<float>
						(ClosestGL::Math::GetTransformMatrix(
							ClosestGL::Math::Vector3<float>{0.5f,0.5f, 0}
					));

					auto MoveToScreen = MoveToCenter * Scale;

					return Vertex{
						MoveToScreen * (view * v.SVPosition)
					};
				}, vbo.data(), transformed.data(), vbo.size(), runner);
				
				tex.Clear(Tools::TestCol{ 0,0,0,0 },runner);
				lineReader.Reset();

				runner.Wait();
				raster.EmitPrimitive(lineReader, transformed.data(),runner);
				runner.Wait();
			});
		}
	public:
		TEST_METHOD(TestCubeWireFrame)
		{
			ViewModel("Models\\Cube.obj",0.5f);
		}

		TEST_METHOD(TestCandyWireFrame)
		{
			ViewModel("Models\\Candy.obj",0.01f);
		}
	};
}

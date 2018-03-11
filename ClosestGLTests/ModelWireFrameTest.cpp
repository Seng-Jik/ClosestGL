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
#include <Transformers.h>
#include <MatrixTransform.h>
#include <OpenMPRunner.h>
#include <CVVClipper.h>
#include <PrimitiveReader.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ClosestGL;
using namespace std::chrono_literals;
using namespace ClosestGLTests::Tools::WireFrameViewer;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(ModelWireFrameTest)
	{
	private:
		void ViewModel(const char* path,float distance)
		{
			auto obj = Tools::LoadModelOBJ<Vertex>(path);
			auto& vbo = std::get<0>(obj);
			auto ibo = ConvertTranglesToLines(std::get<1>(obj));

			//ParallelStrategy::SingleThreadRunner runner;
			//ParallelStrategy::MultiThreadRunner runner(std::thread::hardware_concurrency());
			ParallelStrategy::OpenMPRunner runner;
			Tools::TestTex tex{ {1024,768} };
			RenderTarget rt{ Tools::Blenders::NoBlend,{&tex} };
			PixelShader ps{ &rt,PS{} };
			LineRasterizer raster = { &ps };

			auto size = tex.GetSize();
			float aspect = size.x / float(size.y);

			const auto projection =
				ClosestGL::Math::GetPerspectiveMatrixLH(3.1415926f / 2.0f, aspect, 1.0f, 5000.0f);

			const auto view = Math::GetLookAtMatrix(
				Math::Vector3<float>{0, 0, distance},
				Math::Vector3<float>{0, 0, -1},
				Math::Vector3<float>{0, 1, 0}
			);

			const auto projectionView = projection * view;

			std::vector<Vertex> transformed{ vbo.size() };

			Primitive::PrimitiveListReader<2> tr{ibo.data(), ibo.size()};
			Primitive::CVVClipper<Primitive::PrimitiveListReader<2>, Vertex>
				lineReader{ transformed.data() ,&tr };

			std::vector<std::array<size_t,2>> clippedPrmitive;

			Tools::ViewModel(tex,
				[projectionView,&vbo,&ibo,&raster,&tex,&lineReader,&runner, projection,&transformed,&clippedPrmitive](const auto& world) {		
				auto transform = projectionView * world;

				ClosestGL::Primitive::FixedTransform
				([transform](const Vertex& v) {
					auto pos = transform * v.SVPosition;
					return Vertex{ pos };
				}, vbo.data(), transformed.data(), vbo.size(), runner);
				
				tex.Clear(Tools::TestCol{ 0,0,0,0 },runner);
				runner.Wait();

				lineReader.Reset();

				clippedPrmitive.clear();
				lineReader.WriteToVertexList(clippedPrmitive);

				ClosestGL::Primitive::PlaceTransform(
				[](Vertex& v)
				{
					v.SVPosition /= v.SVPosition.w;
				},transformed.data(),transformed.size(),runner);
				runner.Wait();

				Primitive::PrimitiveReader<std::array<size_t, 2>> reader(clippedPrmitive.data(), clippedPrmitive.size());


				raster.EmitPrimitive(reader, transformed.data(),runner);
				runner.Wait();
			});
		}
	public:
		TEST_METHOD(TestCubeWireFrame)
		{
			ViewModel("Models\\Cube.obj",2.5f);
		}

		TEST_METHOD(TestCandyWireFrame)
		{
			ViewModel("Models\\Candy.obj",48);
		}

		TEST_METHOD(TestAliceRoomWireFrame)
		{
			ViewModel("Models\\Alice's Room.obj", 1600);
		}

		TEST_METHOD(TestTHDMZStage1)
		{
			ViewModel("Models\\THDMZStage1.obj", 1600);
		}
	};
}

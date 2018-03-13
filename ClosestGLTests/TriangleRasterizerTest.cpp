#include "stdafx.h"
#include <PrimitiveListReader.h>
#include <SingleThreadRunner.h>
#include <TriangleRasterizer.h>
#include <Texture2D.h>
#include <Sampler2D.h>
#include <MatrixTransform.h>
#include <UVNormalizer.h>
#include <Transformers.h>

using namespace ClosestGL;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(TriangleRasterizerTest)
	{
		TEST_METHOD(TestColoredTriangle)
		{
			struct Vertex
			{
				Math::Vector4<float> SVPosition;
				Math::Vector4<float> Color;

				static Vertex Lerp(float x, const Vertex& p1, const Vertex& p2)
				{
					return {
						Math::Lerp(x,p1.SVPosition,p2.SVPosition),
						Math::Lerp(x,p1.Color,p2.Color)
					};
				}
			};

			const Vertex mesh[] = 
			{
				{ { 0,0.5f,0,1 },{1,0,0,1} },
				{ { -0.9f,0.75f,0,1 },{0,1,0,1} },
				{ { 0.5,-0.9f,0,1 },{0,0,1,1} }
			};

			const size_t indicis[] = { 0,1,2 };

			Primitive::PrimitiveListReader<3> preader{ indicis,3 };

			Tools::TestTex fb{ {800,600} };
			
			RenderPipeline::RenderTarget<1, Tools::TestCol, decltype(Tools::Blenders::NoBlend)>
				renderTarget{ Tools::Blenders::NoBlend,{&fb} };

			const auto PixelShader = [](const Vertex& v)
			{
				return std::array<Tools::TestCol, 1> { v.Color };
			};

			RenderPipeline::PixelShader<decltype(renderTarget), decltype(PixelShader)>
				ps{ &renderTarget,PixelShader };

			RenderPipeline::TriangleRasterizer<decltype(ps), float> raster{ &ps };

			raster.EmitPrimitive(preader, mesh, 3, ParallelStrategy::SingleThreadRunner{});

			Tools::ViewSurface(fb);
		}

		TEST_METHOD(TestTexturedTriangle)
		{
			struct Vertex
			{
				Math::Vector4<float> SVPosition;

				static Vertex Lerp(float x, const Vertex& p1, const Vertex& p2)
				{
					return {
						Math::Lerp(x,p1.SVPosition,p2.SVPosition)
					};
				}
			};

			Tools::TestTex texture{ {512,512} };
			texture.Shade([](const auto& rtuv)
			{
				bool b = false;
				if (rtuv.x % 32 >= 16) b = !b;
				if (rtuv.y % 32 >= 16) b = !b;
				return b ? Tools::TestCol{0, 0, 0, 1} : Tools::TestCol{1, 1, 1, 1};
			}, ParallelStrategy::SingleThreadRunner{});

			Texture::Sampler::Sampler2D
				<decltype(texture), decltype(Texture::Sampler::UVNormalizer::UV2DRepeat)>
				sampler(&texture, Texture::Sampler::UVNormalizer::UV2DRepeat);

			const Vertex mesh[] =
			{
				{ { 0,0.5f,0,1 } },
				{ { -0.9f,0.75f,0,1 } },
				{ { 0.5,-0.9f,0,1 } }
			};

			const size_t indicis[] = { 0,1,2 };

			Primitive::PrimitiveListReader<3> preader{ indicis,3 };

			Tools::TestTex fb{ { 800,600 } };
			

			RenderPipeline::RenderTarget<1, Tools::TestCol, decltype(Tools::Blenders::NoBlend)>
				renderTarget{ Tools::Blenders::NoBlend,{ &fb } };

			const auto PixelShader = [&sampler](const Vertex& v)
			{
				return std::array<Tools::TestCol, 1> 
				{
					sampler.Sample(Math::Vector2<float>{
						v.SVPosition.x,
						v.SVPosition.y
					})
				};
			};

			RenderPipeline::PixelShader<decltype(renderTarget), decltype(PixelShader)>
				ps{ &renderTarget,PixelShader };

			RenderPipeline::TriangleRasterizer<decltype(ps), float> raster{ &ps };

			raster.EmitPrimitive(preader, mesh, 3, ParallelStrategy::SingleThreadRunner{});

			Tools::ViewSurface(fb,200000);
		}

		TEST_METHOD(TestTexturedQuad)
		{
			struct Vertex
			{
				Math::Vector4<float> SVPosition;

				static Vertex Lerp(float x, const Vertex& p1, const Vertex& p2)
				{
					return {
						Math::Lerp(x,p1.SVPosition,p2.SVPosition)
					};
				}
			};

			Tools::TestTex texture{ { 512,512 } };
			texture.Shade([](const auto& rtuv)
			{
				bool b = false;
				if (rtuv.x % 32 >= 16) b = !b;
				if (rtuv.y % 32 >= 16) b = !b;
				return b ? Tools::TestCol{ 0, 0, 0, 1 } : Tools::TestCol{ 1, 1, 1, 1 };
			}, ParallelStrategy::SingleThreadRunner{});

			Texture::Sampler::Sampler2D
				<decltype(texture), decltype(Texture::Sampler::UVNormalizer::UV2DRepeat)>
				sampler(&texture, Texture::Sampler::UVNormalizer::UV2DRepeat);

			const std::vector<Vertex> mesh =
			{
				{ { 0,0.5f,0,1 } },
				{ { -0.9f,0.75f,0,1 } },
				{ { 0.5,-0.9f,0,1 } }
			};

			const std::vector<size_t> indicis = { 0,1,2 };

			Primitive::PrimitiveListReader<3> preader{ indicis.data(),3 };

			Tools::TestTex fb{ { 800,600 } };


			RenderPipeline::RenderTarget<1, Tools::TestCol, decltype(Tools::Blenders::NoBlend)>
				renderTarget{ Tools::Blenders::NoBlend,{ &fb } };

			const auto PixelShader = [&sampler](const Vertex& v)
			{
				return std::array<Tools::TestCol, 1>
				{
					sampler.Sample(Math::Vector2<float>{
						v.SVPosition.x,
						v.SVPosition.y
					})
				};
			};

			RenderPipeline::PixelShader<decltype(renderTarget), decltype(PixelShader)>
				ps{ &renderTarget,PixelShader };

			RenderPipeline::TriangleRasterizer<decltype(ps), float> raster{ &ps };

			const auto aspect = 800 / 600.0f;
			const auto projection =
				ClosestGL::Math::GetPerspectiveMatrixLH(3.1415926f / 2.0f, aspect, 1.0f, 5000.0f);

			const auto view = Math::GetLookAtMatrix(
				Math::Vector3<float>{0, 0, 100},
				Math::Vector3<float>{0, 0, -1},
				Math::Vector3<float>{0, 1, 0}
			);

			const auto vp = projection * view;
			std::vector<Vertex> transformed{ mesh.size() };

			Tools::ViewModel(fb, 
				[vp,&raster,&preader,&mesh,&fb,&transformed]
			(const auto& world)
			{
				fb.Clear({ 0,0,0,0 },ParallelStrategy::SingleThreadRunner{});

				const auto transform = vp * world;
				Primitive::FixedTransform(
					[&transform](const Vertex& v)
				{
					return Vertex{
						transform  * v.SVPosition
					};
				}, mesh.data(), transformed.data(), mesh.size(), ParallelStrategy::SingleThreadRunner{});

				preader.Reset();
				raster.EmitPrimitive(
					preader,
					transformed.data(),
					mesh.size(), 
					ParallelStrategy::SingleThreadRunner{});
			});
		}
	};
}

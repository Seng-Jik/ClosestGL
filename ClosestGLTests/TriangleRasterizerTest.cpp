#include "stdafx.h"
#include <PrimitiveListReader.h>
#include <SingleThreadRunner.h>
#include <TriangleRasterizer.h>
#include <Texture2D.h>
#include <Sampler2D.h>
#include <MatrixTransform.h>
#include <UVNormalizer.h>
#include <Transformers.h>
#include <PerspectiveCorrector.h>
#include <TextureFliters.h>

using namespace ClosestGL;
using namespace ClosestGL::RenderPipeline::PerspectiveCorrector;

namespace ClosestGLTests::RenderPipelineTest
{
	TEST_CLASS(TriangleRasterizerTest)
	{
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
				<decltype(texture), decltype(Texture::Sampler::UVNormalizer::UV2DRepeat),
				decltype(Texture::Sampler::Fliters::Nearest)>
				sampler(&texture, Texture::Sampler::UVNormalizer::UV2DRepeat,Texture::Sampler::Fliters::Nearest);

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

			RenderPipeline::PixelShaderStage<decltype(renderTarget), decltype(PixelShader)>
				ps{ &renderTarget,PixelShader };

			RenderPipeline::TriangleRasterizer<decltype(ps), float> raster{ &ps };

			raster.EmitPrimitive(preader, mesh, 3, ParallelStrategy::SingleThreadRunner{});

			Tools::ViewSurface(fb,200000);
		}

		TEST_METHOD(TestTexturedQuad)
		{
			struct VertexShaderOut
			{
				Math::Vector4<float> SVPosition;
				Math::Vector2<float> UV;
				InPixelShader<float> perp;

				static VertexShaderOut Lerp(float x, const VertexShaderOut& p1, const VertexShaderOut& p2)
				{
					return {
						Math::Lerp(x,p1.SVPosition,p2.SVPosition),
						Math::Lerp(x,p1.UV,p2.UV),
						Math::Lerp(x,p1.perp,p2.perp)
					};
				}
			};

			struct VertexShaderIn
			{
				Math::Vector4<float> SVPosition;
				Math::Vector2<float> UV;
			};

			Tools::TestTex texture{ { 1024,1024 } };
			texture.Shade([](const auto& rtuv)
			{
				bool b = false;
				if (rtuv.x % 64 >= 32) b = !b;
				if (rtuv.y % 64 >= 32) b = !b;
				return b ? Tools::TestCol{ 0, 0, 0, 1 } : Tools::TestCol{ 1, 1, 1, 1 };
			}, ParallelStrategy::SingleThreadRunner{});

			Texture::Sampler::Sampler2D
				<decltype(texture), decltype(Texture::Sampler::UVNormalizer::UV2DClamp),
				decltype(Texture::Sampler::Fliters::Bilinear)>
				sampler(
					&texture,
					Texture::Sampler::UVNormalizer::UV2DClamp,
					Texture::Sampler::Fliters::Bilinear);

			const std::vector<VertexShaderIn> mesh =
			{
				{ { -0.5F,-0.5F,0,1 }	,{ 0,0 } },
				{ { -0.5F,0.5F,0,1 }	,{ 0,1 } },
				{ { 0.5F,0.5F,0,1 }		,{ 1,1 } },
				{ { 0.5F,-0.5F,0,1 }	,{ 1,0 } }
			};

			const std::vector<size_t> indicis = { 0,1,2,0,3,2 };

			Primitive::PrimitiveListReader<3> preader{ indicis.data(),indicis.size() };

			Tools::TestTex fb{ { 1024,760 } };


			RenderPipeline::RenderTarget<1, Tools::TestCol, decltype(Tools::Blenders::NoBlend)>
				renderTarget{ Tools::Blenders::NoBlend,{ &fb } };

			const auto PixelShader = [&sampler](const VertexShaderOut& v)
			{
				return std::array<Tools::TestCol, 1>
				{
					sampler.Sample(Math::Vector2<float>{
						v.perp(v.UV)
					})
				};
			};

			RenderPipeline::PixelShaderStage<decltype(renderTarget), decltype(PixelShader)>
				ps{ &renderTarget,PixelShader };

			RenderPipeline::TriangleRasterizer<decltype(ps), float> raster{ &ps };

			const auto aspect = 800 / 600.0f;
			const auto projection =
				ClosestGL::Math::GetPerspectiveMatrixLH(3.1415926f / 2.0f, aspect, 0.01f, 5000.0f);

			const auto view = Math::GetLookAtMatrix(
				Math::Vector3<float>{0, 0, 0.7F},
				Math::Vector3<float>{0, 0, -1},
				Math::Vector3<float>{0, 1, 0}
			);

			const auto vp = projection * view;
			std::vector<VertexShaderOut> transformed{ mesh.size() };

			Tools::ViewModel(fb, 
				[vp,&raster,&preader,&mesh,&fb,&transformed]
			(const auto& world)
			{
				fb.Clear({ 0,0,0,0 },ParallelStrategy::SingleThreadRunner{});

				const auto transform = vp * world;
				Primitive::FixedTransform(
					[&transform](const VertexShaderIn& v)
				{
					const auto pos = transform * v.SVPosition;

					BeforePerspectiveDivision<float> fixer(pos);
					InPixelShader<float> fixerInPS;
					fixerInPS.Active(fixer);

					return VertexShaderOut{
						pos,
						fixer(v.UV),
						fixerInPS
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

# ClosestGL

## 一个简陋的软件渲染工具库

### 已有的功能
1.基本数学库
2.并行执行策略
3.CVV剪裁
4.IndexBuffer生成器
5.透视除法缓存
6.图元列表读取器、图元Strip读取器、直接图元阅读器
7.固定变换器（用于顶点着色器）、重建变换器（用于几何着色器）、就地变换器
8.直线光栅器
9.像素着色器
10.渲染目标和混合器
11.Texture2D
12.线程安全队列

### 光栅渲染管线
![RenderPipeline](RenderPipeline.png)

### 光栅渲染管线效果
![WireFrameRendering](WireFrameRendering.gif)
![CandyWireFrameRendering](CandyWireFrameRendering.gif)
![CVVClip1](CVVClip1.gif)
![CVVClip2](CVVClip2.gif)

### 使用说明
直接使用本库是不需要任何依赖的。
如果需要执行其中的测试例子（ClosestGLTest项目），需要SDL2库。
把SDL2库解压到解决方案目录下，重命名为“SDL2”即可。

### 如何直线光栅渲染管线

```cpp
#include <Texture2D.h>
#include <Vector4.h>
#include <RenderTarget.h>

using namespace ClosestGL;

//..省略
	
struct Vertex							//顶点数据格式
{
	Math::Vector4<float> SVPosition;	//必须有的变量，SVPosition坐标
};

using ColorType = Math::Vector4<float>;	//指定颜色类型

auto NoBlend = [](auto src,auto dst) { return src; };	//准备一个混合器

auto PixelShader = [](const Vertex& attribute)			//准备一个像素着色器
{
	std::array<ColorType,1> col = { ColorType{1,1,1,1} };	//准备像素着色器的输出数据
															//由于渲染目标可能有多个纹理，所以输出多个数据
															//数量必须和渲染目标内纹理个数相匹配
	return col;												//返回颜色列表
}

Texture::Texture2D<ColorType> framebuffer { {1024,768} };	//建立1024*768的帧缓存

RenderPipeline::RenderTarget renderTarget<1,ColorType,decltype(NoBlend)>
	rt(NoBlend, {&frameBuffer});	//创建一个渲染目标，包含一个帧缓存，使用NoBlend混合器
	
RenderPipeline::PixelShader<decltype(rt),decltype(PixelShader)>
	ps(&renderTarget,PixelShader);	//创建像素着色器实例，并且将颜色输出导引到渲染目标上。
	
RenderPipeline::LineRasterizer<decltype(ps),float> lr(&ps);	//以float精度创建导引到ps着色器的直线光栅器lr

const Vertex vertex[] = 		//三个顶点的顶点缓存
{
	{{1,0,0,1}},
	{{0,1,0,1}},
	{{0,0,1,1}}
};

const size_t ibo[] = {0,1,2};	//LineStrip形式的索引缓存

Primitive::PrimitiveStripReader<2> preader(ibo,3);	//创建图元阅读器，光栅器需要从此处读取索引缓存

ParallelStrategy::MultiThreadRunner runner{3};	//三个线程的并行执行策略

lr.EmitPrimitive(preader, vertex, 3,runner);	//提交顶点缓存和图元阅读器

runner.Wait();			//等待渲染完成

//可以从frameBuffer对象提取渲染结果了


//..省略


```













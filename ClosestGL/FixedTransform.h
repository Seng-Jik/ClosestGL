#pragma once

namespace ClosestGL::Primitive
{
	/* 固定变换器 */

	/* 用于执行一对一的变换，可用于渲染管线中的顶点着色器部分。
	 * transformer用于传入变换器，变换器的函数原形应当为：
	 *     TOutput transformer(TInput input)
	 *     变换器的输入为TInput类型的输入数据input，返回TOutput类型的输出数据。
	 * inputBuffer用于传入指向输入数据缓存的指针。
	 * outputBuffer用于传入指向输出数据缓存的指针，需要保证outputBuffer指向的空间有足够的大小
	 * 来保存输出数据。
	 * count传入数据的数量，则inputBuffer应该有count个TInput，outputBuffer指向的空间至少可
	 * 以保存count个TOutput。
	 * runner，用于传入并行执行策略（参见SingleThreadRunner）。
	 */

	template<typename TTransform,typename TInput,typename TOutput,typename TParallelStrategy>
	void FixedTransform(
		TTransform& transformer,
		const TInput* inputBuffer, 
		TOutput* outputBuffer,
		size_t count, 
		TParallelStrategy& runner)
	{
		runner.Commit(0,count,
			[inputBuffer, outputBuffer, &transformer](size_t index,auto) {
				outputBuffer[index] = transformer(inputBuffer[index]);
			}
		);
	}
}

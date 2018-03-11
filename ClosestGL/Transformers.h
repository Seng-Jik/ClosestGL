#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	/* 固定变换器

	 * 用于执行一对一的变换，可用于渲染管线中的顶点着色器部分。
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

	/* 就地变换器
	 * 
	 * 在io位置就地进行变换
	 *     transformer - 变换器，原形如下
	 *         void transformer(TIO& io)
	 *     io          - 指向数据的指针
	 *     count       - 数据的数量
	 *     runner      - 并发执行策略
	 */
	template<typename TTransform,typename TIO,typename TParallelStrategy>
	void PlaceTransform(
		TTransform& transformer,
		TIO* io,
		size_t count,
		TParallelStrategy& runner)
	{
		runner.Commit(0,count,
			[io, &transformer](size_t i,auto)
		{
			transformer(io[i]);
		});
	}

	/* 重建变换器 */

	/* 用于读取一组数据，并重新生成一组数据。
	* transformer - 重建变换器，需要具有以下函数签名：
	*     void transformer(const TInput& input,std::vector<TOutput>& output)
	*        input  - 输入数据
	*        output - 输出容器，可以向其中push多个数据
	* inputBuffer - 输入缓存
	* count       - 输入缓存中的数据的数量
	* outputs     - 输出的集合，为了性能考量，会把输出拆分为多分，此处为输出的集合，传入之前需要clear
	* runner      - 并发执行策略（参见SingleThreadRunner）
	*/
	template<typename TOutput, typename TTransform, typename TInput, typename TParallelStrategy>
	void RebuildTransform(
		TTransform& transformer,
		const TInput* inputBuffer,
		size_t count,
		std::vector<std::vector<TOutput>>& outputs,
		TParallelStrategy& runner)
	{

		outputs.resize(runner.ParallelSize());

		for (auto& i : outputs)
			i.reserve(count / runner.ParallelSize() + 1);

		runner.Commit(0, count,
			[&outputs, &transformer, inputBuffer](size_t index, size_t threadID)
		{
			auto& output = outputs[threadID];
			transformer(inputBuffer[index], output);
		}
		);
	}
}

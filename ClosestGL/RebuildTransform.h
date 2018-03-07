#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
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
			[&outputs,&transformer, inputBuffer](size_t index, size_t threadID)
			{
				auto& output = outputs[threadID];
				transformer(inputBuffer[index], output);
			}
		);
	}
}

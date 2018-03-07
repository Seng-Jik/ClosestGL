#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	template<typename TOutput, typename TTransform, typename TInput, typename TParallelStrategy>
	void RebuildTransform(
		TTransform& transformer,
		TInput* inputBuffer,
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

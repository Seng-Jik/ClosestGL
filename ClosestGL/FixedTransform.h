#pragma once

namespace ClosestGL::Primitive
{
	template<typename TTransform,typename TInput,typename TOutput,typename TParallelStrategy>
	void FixedTransform(
		TTransform& transformer,
		TInput* inputBuffer, 
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

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

		struct ForAction
		{
			TInput* inputBuffer;
			TOutput* outputBuffer;
			TTransform* transformer;

			void operator() (size_t index, size_t) const
			{
				outputBuffer[index] = (*transformer)(inputBuffer[index]);
			}
		};

		ForAction action{ inputBuffer, outputBuffer, &transformer };

		runner.Commit(0,count,action);
	}
}

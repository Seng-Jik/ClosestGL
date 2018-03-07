#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	/* �ؽ��任�� */

	/* ���ڶ�ȡһ�����ݣ�����������һ�����ݡ� 
	 * transformer - �ؽ��任������Ҫ�������º���ǩ����
	 *     void transformer(const TInput& input,std::vector<TOutput>& output)
	 *        input  - ��������
	 *        output - �������������������push�������
	 * inputBuffer - ���뻺��
	 * count       - ���뻺���е����ݵ�����
	 * outputs     - ����ļ��ϣ�Ϊ�����ܿ��������������Ϊ��֣��˴�Ϊ����ļ��ϣ�����֮ǰ��Ҫclear
	 * runner      - ����ִ�в��ԣ��μ�SingleThreadRunner��
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

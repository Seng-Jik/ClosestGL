#pragma once
#include <vector>

namespace ClosestGL::Primitive
{
	/* �̶��任��

	 * ����ִ��һ��һ�ı任����������Ⱦ�����еĶ�����ɫ�����֡�
	 * transformer���ڴ���任�����任���ĺ���ԭ��Ӧ��Ϊ��
	 *     TOutput transformer(TInput input)
	 *     �任��������ΪTInput���͵���������input������TOutput���͵�������ݡ�
	 * inputBuffer���ڴ���ָ���������ݻ����ָ�롣
	 * outputBuffer���ڴ���ָ��������ݻ����ָ�룬��Ҫ��֤outputBufferָ��Ŀռ����㹻�Ĵ�С
	 * ������������ݡ�
	 * count�������ݵ���������inputBufferӦ����count��TInput��outputBufferָ��Ŀռ����ٿ�
	 * �Ա���count��TOutput��
	 * runner�����ڴ��벢��ִ�в��ԣ��μ�SingleThreadRunner����
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

	/* �͵ر任��
	 * 
	 * ��ioλ�þ͵ؽ��б任
	 *     transformer - �任����ԭ������
	 *         void transformer(TIO& io)
	 *     io          - ָ�����ݵ�ָ��
	 *     count       - ���ݵ�����
	 *     runner      - ����ִ�в���
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
			[&outputs, &transformer, inputBuffer](size_t index, size_t threadID)
		{
			auto& output = outputs[threadID];
			transformer(inputBuffer[index], output);
		}
		);
	}
}

#pragma once

namespace ClosestGL::Primitive
{
	/* �̶��任�� */

	/* ����ִ��һ��һ�ı任����������Ⱦ�����еĶ�����ɫ�����֡�
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
}

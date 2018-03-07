#pragma once

namespace ClosestGL::ParallelStrategy
{
	/* ���߳�ִ�в��� */

	/* �߳�ִ�в��ԣ�
	 * ����ClosestGL�еĸ��ֿ��Բ��л����㷨��ִ�з�ʽ��
	 * ִ�в��Ե�Commit���������ɷ����񣬲��������أ�����ȴ�ִ�н�����
	 * Wait�������ڵȴ�������ɡ�
	 * Finished�������ڲ�ѯ�����Ƿ��Ѿ���ɡ�
	 * ParallelSize���ڼ��˲����õ����߳�����
	 */

	/* ���Ա��ɷ������ݣ� 
	 * �ɷ�forѭ�����Զ�Commit����first��end��action��������
	 *     begin - ��ʼʱ��ѭ������
	 *     end   - ����ʱ��ѭ���������ñ������ᱻ���뵽action�ڣ�
	 *     action- ѭ���壬ѭ������Ҫ�������µĺ���ԭ�Σ�
	 *         void action(size_t index,size_t threadID)
	 *             index    - ��ʱ��ѭ������
	 *             threadID - ִ�д˴�ѭ������̵߳�ID����0~ִ�в��Ե�ParallelSize֮��)
	 */
	class SingleThreadRunner
	{
	public:
		inline constexpr void Wait() const {};

		inline constexpr bool Finished() const { return true; }

		inline constexpr size_t ParallelSize() const { return 1; }

		template<typename ForAction>
		inline void Commit(
			size_t first,
			size_t end,
			const ForAction& action
		)
		{
			for (size_t i = first; i < end; ++i)
				action(i, 0);
		}
	};
}

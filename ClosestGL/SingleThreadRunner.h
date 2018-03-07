#pragma once

namespace ClosestGL::ParallelStrategy
{
	/* 单线程执行策略 */

	/* 线程执行策略：
	 * 定义ClosestGL中的各种可以并行化的算法的执行方式。
	 * 执行策略的Commit函数用于派发任务，并立即返回，不会等待执行结束。
	 * Wait函数用于等待任务完成。
	 * Finished函数用于查询任务是否已经完成。
	 * ParallelSize用于检查此策略用到的线程数。
	 */

	/* 可以被派发的内容： 
	 * 派发for循环可以对Commit传入first、end和action三个参数
	 *     begin - 开始时的循环变量
	 *     end   - 结束时的循环变量（该变量不会被带入到action内）
	 *     action- 循环体，循环体需要具有以下的函数原形：
	 *         void action(size_t index,size_t threadID)
	 *             index    - 此时的循环变量
	 *             threadID - 执行此次循环体的线程的ID（在0~执行策略的ParallelSize之间)
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

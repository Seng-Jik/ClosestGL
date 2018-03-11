#pragma once
#include <omp.h>

namespace ClosestGL::ParallelStrategy
{
	/* OpenMP�Ķ��߳�ִ���� */
	class OpenMPRunner
	{
	public:
		inline constexpr void Wait() const {};

		inline constexpr bool Finished() const { return true; }

		inline size_t ParallelSize() const { return omp_get_num_threads(); }

		template<typename ForAction>
		inline void Commit(
			size_t first,
			size_t end,
			const ForAction& action
		)
		{
			int e = int(end);
#pragma omp parallel for
			for (int i = int(first); i < e; ++i)
			{
				auto threadID = omp_get_thread_num();
				action(i, threadID);
			}
		}

		template<typename Action>
		inline void Commit(const Action& action)
		{
			action();
		}
	};
}

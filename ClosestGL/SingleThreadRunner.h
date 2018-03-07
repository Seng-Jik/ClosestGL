#pragma once

namespace ClosestGL::ParallelStrategy
{
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

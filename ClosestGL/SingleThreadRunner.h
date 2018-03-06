#pragma once
#include "ParallelStrategyCommon.h"

namespace ClosestGL::ParallelStrategy
{
	class SingleThreadRunner
	{
	public:
		inline constexpr void Wait() const {};

		inline constexpr bool Finished() const { return true; }

		inline void Commit(
			unsigned first,
			unsigned end,
			const ForAction& action
		)
		{
			for (unsigned i = first; i < end; ++i)
				action(i, 0);
		}
	};
}

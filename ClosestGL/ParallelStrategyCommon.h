#pragma once
#include <functional>

namespace ClosestGL::ParallelStrategy
{
	using ForAction = std::function<void(size_t i, size_t threadID)>;
}

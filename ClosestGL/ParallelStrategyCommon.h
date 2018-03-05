#pragma once
#include <functional>

namespace ClosestGL::ParallelStrategy
{
	using ForAction = std::function<void(unsigned i, unsigned threadID)>;
}

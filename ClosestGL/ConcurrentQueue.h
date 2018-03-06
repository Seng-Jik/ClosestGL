#pragma once
#include <queue>
#include <mutex>
#include <utility>
#include <optional>

namespace ClosestGL::Utils
{
	template<typename T>
	class ConcurrentQueue
	{
	private:
		std::queue<T> queue_;
		std::mutex lock_;
	public:
		void Push(T&& obj)
		{
			auto g = Lock();
			queue_.push(obj);
		}

		void PushUnsafe(T&& obj)
		{
			queue_.push(obj);
		}

		std::unique_lock<std::mutex> Lock()
		{
			return std::unique_lock<std::mutex>(lock_);
		}

		std::optional<T> Pop()
		{
			auto g = Lock();

			std::optional<T> obj;
			if (!queue_.empty())
			{
				obj = std::move(queue_.front());
				queue_.pop();
			}
			return obj;
		}

		bool Empty()
		{
			auto g = Lock();
			return queue_.empty();
		}
	};
}

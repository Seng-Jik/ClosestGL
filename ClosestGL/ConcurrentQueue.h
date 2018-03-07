#pragma once
#include <queue>
#include <mutex>
#include <utility>
#include <optional>

namespace ClosestGL::Utils
{
	/* 线程安全队列 */

	template<typename T>
	class ConcurrentQueue
	{
	private:
		std::queue<T> queue_;
		mutable std::mutex lock_;
	public:

		/* 压入对象 */
		void Push(T&& obj)
		{
			auto g = Lock();
			queue_.push(obj);
		}

		/* 不加锁的情况下压入对象 */
		void PushUnsafe(T&& obj)
		{
			queue_.push(obj);
		}

		/* 获取并锁定锁 */
		std::unique_lock<std::mutex> Lock() const
		{
			return std::unique_lock<std::mutex>(lock_);
		}

		/* 弹出对象 */
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

		/* 检查是否为空 */
		bool Empty() const
		{
			auto g = Lock();
			return queue_.empty();
		}
	};
}

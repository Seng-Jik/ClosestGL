#pragma once
#include <queue>
#include <mutex>
#include <utility>
#include <optional>

namespace ClosestGL::Utils
{
	/* �̰߳�ȫ���� */

	template<typename T>
	class ConcurrentQueue
	{
	private:
		std::queue<T> queue_;
		mutable std::mutex lock_;
	public:

		/* ѹ����� */
		void Push(T&& obj)
		{
			auto g = Lock();
			queue_.push(obj);
		}

		/* �������������ѹ����� */
		void PushUnsafe(T&& obj)
		{
			queue_.push(obj);
		}

		/* ��ȡ�������� */
		std::unique_lock<std::mutex> Lock() const
		{
			return std::unique_lock<std::mutex>(lock_);
		}

		/* �������� */
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

		/* ����Ƿ�Ϊ�� */
		bool Empty() const
		{
			auto g = Lock();
			return queue_.empty();
		}
	};
}

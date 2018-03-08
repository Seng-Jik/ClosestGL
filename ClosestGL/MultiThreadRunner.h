#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include "ConcurrentQueue.h"

namespace ClosestGL::ParallelStrategy
{
	/* 高性能并发执行策略 */

	class MultiThreadRunner
	{
	private:

		using Task = std::function<void(size_t)>;

		struct Thread
		{
			std::thread th;
			std::atomic_bool free;

			inline static void TaskRunner(
				size_t threadID,
				std::atomic_bool& isFree,
				const std::atomic_bool& stopRequested,
				ClosestGL::Utils::ConcurrentQueue<Task>& taskQueue)
			{
				while (true)
				{
					isFree = false;
					auto task = taskQueue.Pop();
					if (!task.has_value())
					{
						isFree = true;
						if (stopRequested)
							break;
						else
							std::this_thread::yield();
					}
					else
						(*task)(threadID);
				}
			}

			inline Thread(
				size_t threadID,
				ClosestGL::Utils::ConcurrentQueue<Task>& taskQueue,
				const std::atomic_bool& stopRequested):
				th{ TaskRunner,threadID,std::ref(free),std::ref(stopRequested),std::ref(taskQueue) },
				free{ true } 
			{}
		};


		ClosestGL::Utils::ConcurrentQueue<Task> taskQueue_;
		std::atomic_bool stopRequested_;
		std::vector<std::unique_ptr<Thread>> threads_;
	public:

		inline bool Finished() const
		{
			bool allFinished = true;
			if (taskQueue_.Empty())
			{
				for (const auto& i : threads_)
				{
					if (!i->free)
					{
						allFinished = false;
						break;
					}
				}
			}
			else
				allFinished = false;
			return allFinished;
		}

		inline void Wait()
		{
			while (true)
			{
				const bool allFinished = Finished();

				if (allFinished)
					break;
				else
					std::this_thread::yield();
			}
		};

		template<typename ForAction>
		inline void Commit(
			size_t first,
			const size_t end,
			const ForAction& action
		)
		{
			size_t taskEveryThread = (end - first) / threads_.size() / 2;

			if (taskEveryThread == 0)
				taskEveryThread = end - first;

			{
				auto lock = taskQueue_.Lock();
				while (true)
				{
					size_t endThere = first + taskEveryThread;
					if (endThere >= end) endThere = end;

					Task task
					{
						[first,endThere,action](size_t threadID)
						{
							for (size_t index = first; index < endThere; index++)
								action(index, threadID);
						}
					};

					taskQueue_.PushUnsafe(std::move(task));

					first = endThere;
					if (first == end)
						break;
				}
			}
		}

		inline size_t ParallelSize() const { return threads_.size(); }

		/* threadCount - 使用的线程数 */
		inline MultiThreadRunner(size_t threadCount) :
			threads_{},
			stopRequested_{ false }
		{
			for (size_t id = 0; id < threadCount; ++id)
			{
				threads_.emplace_back(std::make_unique<Thread>(id, taskQueue_, stopRequested_));
			}
		}

		inline ~MultiThreadRunner()
		{
			Wait();
			stopRequested_ = true;
			for (auto& t : threads_)
				t->th.join();
		}
	};
}

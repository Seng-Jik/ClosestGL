#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include "ConcurrentQueue.h"
#include "ParallelStrategyCommon.h"
#include "Number.h"

namespace ClosestGL::ParallelStrategy
{
	class MultiThreadRunner
	{
	private:
		struct ForTask
		{
			unsigned first, end;
			ForAction action;
		};

		struct Thread
		{
			std::thread th;
			std::atomic_bool free;

			inline static void TaskRunner(
				unsigned threadID,
				std::atomic_bool& isFree,
				const std::atomic_bool& stopRequested,
				ClosestGL::Utils::ConcurrentQueue<ForTask>& taskQueue)
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
						for (auto i = task->first; i < task->end; ++i)
							(*task).action(i, threadID);
				}
			}

			inline Thread(
				unsigned threadID,
				ClosestGL::Utils::ConcurrentQueue<ForTask>& taskQueue,
				const std::atomic_bool& stopRequested):
				th{ TaskRunner,threadID,std::ref(free),std::ref(stopRequested),std::ref(taskQueue) },
				free{ true } 
			{}
		};


		ClosestGL::Utils::ConcurrentQueue<ForTask> taskQueue_;
		std::atomic_bool stopRequested_;
		std::vector<std::unique_ptr<Thread>> threads_;
	public:

		inline void Wait()
		{
			while (true)
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

				if (allFinished)
					break;
				else
					std::this_thread::yield();
			}
		};

		inline bool Finished() const 
		{
			for (const auto& i : threads_)
				if (!i->free)
					return false;
			return true;
		}

		inline void Commit(
			unsigned first,
			const unsigned end,
			const ForAction& action
		)
		{
			const size_t taskEveryThread = (end - first) / threads_.size();

			{
				auto lock = taskQueue_.Lock();
				while (true)
				{
					unsigned endThere = first + taskEveryThread;
					if (endThere >= end) endThere = end;

					taskQueue_.PushUnsafe(ForTask{ first,endThere,action });

					first = endThere;
					if (first == end)
						break;
				}
			}
		}

		inline MultiThreadRunner(unsigned threadCount) :
			threads_{},
			stopRequested_{ false }
		{
			for (unsigned id = 0; id < threadCount; ++id)
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

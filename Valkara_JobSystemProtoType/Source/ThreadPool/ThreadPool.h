#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include "../ThreadSafeQueue/LockingThreadSafeQueue.h"

// TODO: Maybe supports priority buckets or worker affinity (Thread Pinning)
class ThreadPool
{
	std::vector<std::thread> workers;
	LockingThreadSafeQueue<std::function<void()>> m_taskQueue;
	std::atomic<bool> m_run{ false };

public:

	// On creation, the thread pool will spawn a number of threads equal to threadCount
	ThreadPool(size_t threadCount);

	// on destruction, the thread pool will stop all threads and wait for them to finish
	~ThreadPool();

	// returns the number of tasks in the queue
	size_t Size() { return m_taskQueue.Size(); }

	// Enqueue a task to the thread pool
	void Enqueue(std::function<void()> task);

	// Enqueue lambdas, std::bind, or function pointer
	template<typename task>
	void Enqueue(task&& T)
	{
		m_taskQueue.Push(std::function<void()>(std::forward<task>(T)));
	}
};


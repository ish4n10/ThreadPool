﻿#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t nr_workers) {
	stop = false;
	for (auto i{ 0 }; i < nr_workers; i++) {
		workers.emplace_back(&ThreadPool::worker, this);
	}
}
	/*
	// Instead of taking a value_type, it takes a variadic list of arguments, 
	so that means that you can now perfectly forward the arguments and construct directly 
	an object into a container without a temporary at all.
	*/
	
void ThreadPool::worker() {
	for (;;) {
		std::function<void()> cur_task;
		{
			std::unique_lock<std::mutex> lock(mutex);
			cv.wait(lock, [this]() {
				return stop || !queue.empty();
			});

			if (stop && queue.empty()) {
				break;
			}

			if (queue.empty())
				continue;

			cur_task = queue.front();
			queue.pop();
			// grab the fx from queue
		}
		cur_task();
	}
}



ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(mutex);
		stop = true;
	}

	cv.notify_all();
	for (auto& worker : workers) {
		worker.join();
	}
}
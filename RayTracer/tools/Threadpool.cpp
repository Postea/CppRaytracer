#include "Threadpool.h"

#include <thread>

namespace util {

Threadpool::Threadpool(size_t n) : alive(true) {
	// Create the specified number of threads
	threads.reserve(n);
	for (int i = 0; i < n; ++i) {
		threads.emplace_back(std::bind(&Threadpool::threading, this));
	}
}

Threadpool::~Threadpool() {
	// std::unique_lock<std::mutex> lock(m);

	alive = false;
	cv.notify_all();

	for (auto& thread : threads) thread.join();
}

void Threadpool::queueTask(const std::function<void(void)>& task) {
	std::unique_lock<std::mutex> lock(m);
	q.emplace(task);
	lock.unlock();
	cv.notify_one();
}

void Threadpool::threading() {
	while (true) {
		std::unique_lock<std::mutex> lock(m);
		while (alive && q.empty()) {
			cv.wait(lock);
		}
		if (q.empty()) {
			return;
		}
		auto task = std::move(q.front());
		q.pop();
		lock.unlock();
		task();
	}
}
}  // namespace util

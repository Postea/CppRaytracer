#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace util {

class Threadpool {
   public:
	// Constructor, Destructor
	Threadpool(size_t n);
	~Threadpool();
	// Add a task to the queue
	void queue_task(const std::function<void(void)>& task);

   private:
	void threading();
	std::vector<std::thread> threads;
	bool alive;
	std::queue<std::function<void(void)>> q;
	std::condition_variable cv;
	std::mutex m;
};
}  // namespace util
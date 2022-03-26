#pragma once

#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <vector>

namespace util {

class Threadpool {
   public:
	// Constructor, Destructor
	Threadpool(size_t n, std::string fname, std::string formula);
	~Threadpool();
	// Add a task to the queue
	void queueTask(const std::function<std::vector<int64_t>(void)>& task);

   private:
	void threading(size_t i);
	std::vector<std::thread> threads;
	std::vector<size_t> thread_names;
	std::vector<std::tuple<size_t, int64_t, int64_t, int64_t>> storage;
	std::string fname;
	std::string formula;
	clock_t clkStart;

	int task_n = 0;
	bool alive;
	std::queue<std::function<std::vector<int64_t>(void)>> q;
	std::condition_variable cv;
	std::mutex m;
};
}  // namespace util
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace util {

class Threadpool {
   public:
	// Constructor, Destructor
	Threadpool(size_t n);
	~Threadpool();
	// Add a task to the queue
	void queueTask(std::function<void(void)> task);

   protected:
	void threading();

   private:
	std::vector<std::thread> threads;
	bool alive;
	std::queue<std::function<void(void)>> q;
	std::condition_variable cv;
	std::mutex m;
};
}  // namespace util
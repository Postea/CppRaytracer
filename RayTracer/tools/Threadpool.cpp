#include "Threadpool.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

namespace util {

Threadpool::Threadpool(size_t n, std::string fname)
    : alive(true), fname(fname) {
	// Create the specified number of threads
	for (int i = 0; i < n; i++) {
		thread_names.push_back(i);
	}
	for (int i = 0; i < n; i++) {
		threads.emplace_back(
		    std::bind(&Threadpool::threading, this, thread_names[i]));
	}
}

Threadpool::~Threadpool() {
	alive = false;
	cv.notify_all();

	for (auto& thread : threads) thread.join();

	std::cout << task_n << " queued tasks" << std::endl;

	int temp = 0;
	int64_t rays = 0;
	for (auto x : storage) {
		temp += std::get<1>(x);
		rays += std::get<2>(x);
		std::cout << "Thread " << std::get<0>(x) << " casted " << std::get<2>(x)
		          << " rays, in " << std::get<1>(x) << " tasks" << std::endl;
	}
	std::ofstream res;
	res.open("results/results.txt", std::ios_base::app);
	res << fname << " " << rays << std::endl;

	assert(task_n == temp);
}

void Threadpool::queueTask(
    const std::function<std::vector<int64_t>(void)>& task) {
	std::unique_lock<std::mutex> lock(m);
	++task_n;
	q.emplace(task);
	lock.unlock();
	cv.notify_one();
}

void Threadpool::threading(size_t i) {
	int64_t task_n = 0;
	int64_t n_rays = 0;
	std::ofstream logFile;
	logFile.open("results/logFile" + std::to_string(i) + ".txt");
	while (true) {
		std::unique_lock<std::mutex> lock(m);
		while (alive && q.empty()) {
			cv.wait(lock);
		}
		if (q.empty()) {
			storage.push_back({i, task_n, n_rays});
			lock.unlock();
			logFile << std::endl;
			logFile.close();
			return;
		}
		auto task = std::move(q.front());
		q.pop();
		lock.unlock();
		++task_n;
		auto x = task();
		for (auto xx : x) {
			if (xx == -1)
				logFile << ";";
			else {
				logFile << xx << " ";
				n_rays += xx;
			}
		}
		logFile << std::endl;
	}
}
}  // namespace util

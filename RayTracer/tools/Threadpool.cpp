#include "Threadpool.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#define writeLogs false

namespace util {

Threadpool::Threadpool(size_t n, std::string fname, std::string formula)
    : alive(true), fname(fname), formula(formula) {
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
	int64_t prim_rays = 0;
	int64_t dire_rays = 0;
	for (auto x : storage) {
		temp += std::get<1>(x);
		prim_rays += std::get<2>(x);
		dire_rays += std::get<3>(x);
		std::cout << "Thread " << std::get<0>(x) << " casted ("
		          << std::get<2>(x) << " " << std::get<3>(x) << ") rays, in "
		          << std::get<1>(x) << " tasks" << std::endl;
	}
	std::ofstream res;
	res.open("results/results.txt", std::ios_base::app);
	res << fname << " " << prim_rays << " " << dire_rays << " " << formula
	    << std::endl;

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
	int64_t prim_rays = 0;
	int64_t dire_rays = 0;
	std::ofstream logFile;
#if writeLogs
	logFile.open("results/logFile" + std::to_string(i) + ".txt");
#endif
	while (true) {
		std::unique_lock<std::mutex> lock(m);
		while (alive && q.empty()) {
			cv.wait(lock);
		}
		if (q.empty()) {
			storage.push_back({i, task_n, prim_rays, dire_rays});
			lock.unlock();
#if writeLogs
			logFile << std::endl;
			logFile.close();
#endif
			return;
		}
		auto task = std::move(q.front());
		q.pop();
		lock.unlock();
		++task_n;
		auto x = task();
		for (int i = 0; i < x.size() - 2; i += 3) {
			auto prim = x.at(i);
			auto dire = x.at(i + 1);
			auto delimiter = x.at(i + 2);
			prim_rays += prim;
			dire_rays += dire;

#if writeLogs
			logFile << prim << " " << dire;
			if (delimiter == -1) {
				logFile << ";";
			}
#endif
		}
		logFile << std::endl;
	}
}
}  // namespace util

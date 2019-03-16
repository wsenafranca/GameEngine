#ifndef DISPATCH_QUEUE_H
#define DISPATCH_QUEUE_H

#include <queue>
#include <functional>
#include <mutex>

class DispatchQueue {
public:
	static DispatchQueue* main();

	void async(const std::function<void()> &task);
	void sync(const std::function<void()> &task);
	void poll();
private:
	std::queue< std::function<void()> > queue;
	std::mutex mutex;
};

#endif
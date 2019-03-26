#include "DispatchQueue.h"

DispatchQueue* DispatchQueue::main() {
	static DispatchQueue q;
	return &q;
}

void DispatchQueue::async(const std::function<void()> &task) {
	std::unique_lock<std::mutex>(mutex);
	queue.push(task);
}

void DispatchQueue::sync(const std::function<void()> &task) {
	task();
}

void DispatchQueue::onCreate() {

}

void DispatchQueue::onUpdate(float dt) {
	std::queue< std::function<void()> > buffer;
	{
		std::unique_lock<std::mutex>(mutex);
		if(queue.empty()) return;
		queue.swap(buffer);
	}
	std::function<void()> task;
	while(!buffer.empty()) {
		task = buffer.front();
		task();
		buffer.pop();
	}
}

void DispatchQueue::onDestroy() {
	// cleanup
	onUpdate(0);
}
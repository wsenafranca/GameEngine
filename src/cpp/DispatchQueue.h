#ifndef DISPATCH_QUEUE_H
#define DISPATCH_QUEUE_H

#include <queue>
#include <functional>
#include <mutex>
#include <AppListener.h>

class DispatchQueue : public AppListener {
public:
	static DispatchQueue* main();

	void async(const std::function<void()> &task);
	void sync(const std::function<void()> &task);

	void onCreate() override;
	void onUpdate(float dt) override {}
	void onPreUpdate(float dt) override;
	void onDestroy() override;
private:
	std::queue< std::function<void()> > queue;
	std::mutex mutex;
};

#endif
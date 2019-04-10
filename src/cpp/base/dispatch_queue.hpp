#ifndef BASE_DISPATCH_QUEUE_HPP
#define BASE_DISPATCH_QUEUE_HPP

#include <functional>
#include <queue>
#include <vector>
#include <base/clock.hpp>

namespace base
{

class dispatch_queue {
public:
	void sync(const std::function<void()>& f);

	void async(const base::time when, const std::function<void()>& f);
	void async(const std::function<void()>& f);

	void poll();
	void clear();

	struct main {
		static void sync(const std::function<void()>& f);
		static void async(const base::time when, const std::function<void()>& f);
		static void async(const std::function<void()>& f);
		static void poll();
		static void clear();
	};

	static dispatch_queue _main;

private:
	dispatch_queue() = default;
	~dispatch_queue() = default;
	dispatch_queue(const dispatch_queue&) = delete;
	dispatch_queue& operator=(const dispatch_queue&) = delete;

	struct task {
		base::time time;
		std::function<void()> f;
	};
	struct comparator {
		bool operator()(const task& t1, const task& t2) {return t1.time > t2.time;}
	};
	std::priority_queue<
		dispatch_queue::task, 
		std::vector<dispatch_queue::task>, 
		dispatch_queue::comparator> q;
};

}

#endif
#include "dispatch_queue.hpp"

namespace base
{

dispatch_queue dispatch_queue::_main;

void dispatch_queue::sync(const std::function<void()>& f) {
	f();
}

void dispatch_queue::async(const base::time when, const std::function<void()>& f) {
	task t;
	t.time = when;
	t.f = f;
	q.push(t);
}

void dispatch_queue::async(const std::function<void()>& f) {
	async(base::clock::now(), f);
}

void dispatch_queue::poll() {
	auto now = base::clock::now();
	while(!q.empty()) {
		task t = q.top();
		if(t.time > now) {
			break;
		}
		q.pop();
		t.f();
	}
}

void dispatch_queue::clear() {
	while(!q.empty()) {
		task t = q.top();
		q.pop();
		t.f();
	}
}

void dispatch_queue::main::sync(const std::function<void()>& f) {
	_main.sync(f);
}

void dispatch_queue::main::async(const base::time when, const std::function<void()>& f) {
	_main.async(when, f);
}

void dispatch_queue::main::async(const std::function<void()>& f) {
	_main.async(f);
}

void dispatch_queue::main::poll() {
	_main.poll();
}

void dispatch_queue::main::clear() {
	_main.clear();
}

}
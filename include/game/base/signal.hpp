#ifndef BASE_SIGNAL_HPP
#define BASE_SIGNAL_HPP

#include "macros.hpp"
#include <unordered_map>
#include <functional>

BEGIN_NAMESPACE(base)

class base_signal_t {
public:
    virtual void disconnect(unsigned long id) = 0;
};

template<class ...Args>
class signal_t : public base_signal_t {
public:
    signal_t() : _gen_id(0) {}

    signal_t(const signal_t&) : _gen_id(0) {}

    signal_t& operator=(const signal_t&) {
        _slots.clear();
        return *this;
    }

    virtual ~signal_t() {
        _slots.clear();
    }

    template<class T>
    unsigned long connect(const T* object, void (T::*func)(Args...) const) {
        return connect([object, func](Args... args){
            (object->*func)(args...);
        });
    }

    template<class T>
    unsigned long connect(T* object, void (T::*func)(Args...)) {
        return connect([object, func](Args... args){
            (object->*func)(args...);
        });
    }

    unsigned long connect(const std::function<void(Args...)>& func) {
        auto id = _gen_id++;
        _slots[id] = func;
        return id;
    }

    void disconnect(unsigned long id) override {
        auto it = _slots.find(id);
        if(it != _slots.end()) {
            _slots.erase(it);
        }
    }

    void emit(Args... args) {
        for(auto& it : _slots) {
            it.second(args...);
        }
    }
private:
    std::unordered_map< unsigned long, std::function<void(Args...)> > _slots;
    unsigned long _gen_id;
};

END_NAMESPACE(base)

#define SIGNAL(sgnl, ...) base::signal_t<__VA_ARGS__> sgnl
#define CONNECT(sndr, sgnl, rcvr, slt) sndr->sgnl.connect(rcvr, slt)
#define EMIT(sgnl, ...) sgnl.emit(__VA_ARGS__)
#define DISCONNECT(sgnl, id) sgnl.disconnect(id)

#endif //BASE_SIGNAL_HPP

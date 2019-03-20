#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <Bundle.h>

class Object : public std::enable_shared_from_this<Object> {
public:
	Object()= default;
	virtual ~Object() = default;

	void name(const std::string &name) {m_name = name;}
	const std::string& name() const {return m_name;}
	std::string& name() {return m_name;}

	template<class T>
	T* as() {
		return dynamic_cast<T*>(this);
	}

	void properties(const Bundle &bundle) {m_properties = bundle;}
	const Bundle& properties() const {return m_properties;}
	Bundle& properties() {return m_properties;}

private:
	std::string m_name;
	Bundle m_properties;
};

#endif
#ifndef NODE_H
#define NODE_H

#include <string>
#include <set>
#include <glm/glm.hpp>
#include <Color.h>
#include <BlendFunc.h>
#include <Object.h>
#include <Component.h>
#include <vector>

class Node : public Object {
public:
	Node();
	virtual ~Node();

	void create();
	void preUpdate(float dt);
	void update(float dt);
	void postUpdate(float dt);
	void destroy();

	void addNode(Node* child);
	void removeNode(Node* child);

	void addComponent(Component* component);
	void removeComponent(Component* component);

	template<class T>
	T* findNode(const std::string &name) {
		for(auto child : m_children) {
			if(child->name() == name) {
				return child->as<T>();
			}
			auto ret = child->findNode<T>(name);
			if(ret) return ret;
		}
		return nullptr;
	}

	template<class T>
	T* findComponent(const std::string &name) {
		for(auto component : m_components) {
			if(component->name() == name) {
				return component->as<T>();
			}
		}
		return nullptr;
	}

	const Node* parent() const;

	glm::mat4 transform() const;

	void position(const glm::vec2 &position);
	const glm::vec2& position() const;
	glm::vec2& position();
	void size(const glm::vec2 &size);
	const glm::vec2& size() const;
	glm::vec2& size();
	void width(float width);
	const float& width() const;
	float& width();
	void height(float height);
	const float& height() const;
	float& height();
	void scale(const glm::vec2 &scale);
	const glm::vec2& scale() const;
	glm::vec2& scale();
	void rotate(const float& rotate);
	const float& rotate() const;
	float& rotate();
	void origin(const glm::vec2 &origin);
	const glm::vec2& origin() const;
	glm::vec2& origin();
	void color(const Color &color);
	const Color& color() const;
	Color& color();
	void blendFunc(const BlendFunc &blendFunc);
	const BlendFunc& blendFunc() const;
	BlendFunc& blendFunc();

protected:
	virtual void onCreate() {}
	virtual void onPreUpdate(float dt) {}
	virtual void onUpdate(float dt) {}
	virtual void onPostUpdate(float dt) {}
	virtual void onDestroy() {}

private:
	Node *m_parent;
	std::set<Node*> m_children;
	glm::vec2 m_position, m_size, m_scale, m_origin;
	float m_rotate;
	Color m_color;
	BlendFunc m_blendFunc;
	std::set<Component*> m_components;
};

#endif
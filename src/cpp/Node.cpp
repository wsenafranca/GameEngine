#include "Node.h"
#include <DispatchQueue.h>
#include <cmath>

Node::Node() :  m_parent(nullptr),
				m_position(0.0f, 0.0f),
				m_size(1.0f, 1.0f),
				m_scale(1.0f, 1.0f),
				m_rotate(0.0f),
				m_origin(0.0f, 0.0f), 
				m_color(255, 255, 255, 255),
				m_blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED) 
{
	DispatchQueue::main()->async([this]{
		onCreate();
		for(auto &component : m_components) {
			component->node(this);
			component->onCreate();
		}
	});
}

Node::~Node() {
	for(auto &child : m_children) {
		delete child;
	}
	for(auto &component : m_components) {
		component->onDestroy();
		delete component;
	}
	onDestroy();
}

void Node::preUpdate(float dt) {
	for(auto &component : m_components) {
		component->onPreUpdate(dt);
	}
	onPreUpdate(dt);
	for(auto &child : m_children) {
		child->preUpdate(dt);
	}
}

void Node::update(float dt) {
	for(auto &component : m_components) {
		component->onUpdate(dt);
	}
	onUpdate(dt);
	for(auto &child : m_children) {
		child->update(dt);
	}
}

void Node::postUpdate(float dt) {
	for(auto &component : m_components) {
		component->onPostUpdate(dt);
	}
	onPostUpdate(dt);
	for(auto &child : m_children) {
		child->postUpdate(dt);
	}
}

void Node::destroy() {
	DispatchQueue::main()->async([this]{
		delete this;
	});
}

void Node::addNode(Node* child) {
	m_children.insert(child);
	child->m_parent = this;
}

void Node::removeNode(Node* child) {
	for(auto c : m_children) {
		if(c->name() == child->name()) {
			DispatchQueue::main()->async([this, c]{
				m_children.erase(c);
				c->m_parent = nullptr;
			});
			return;
		}
		c->removeNode(child);
	}
}

void Node::addComponent(Component* component) {
	m_components.insert(component);
}

void Node::removeComponent(Component* component) {
	auto it = m_components.find(component);
	if(it != m_components.end()) {
		m_components.erase(it);
	}
}

glm::mat4 Node::transform() const {
	glm::mat4 parent(1.0f);
	if(m_parent) {
		parent = m_parent->transform();
	}
	glm::mat4 translate(1.0f), origin1(1.0f), origin2(1.0f), scale(1.0f), rotate(1.0f);
	translate[3][0] = m_position.x; translate[3][1] = m_position.y;
	origin1[3][0] = -m_origin.x; origin1[3][1] = -m_origin.y;
	origin2[3][0] = m_origin.x; origin2[3][1] = m_origin.y;
	scale[0][0] = m_scale.x; scale[1][1] = m_scale.y;
	float s = sinf(m_rotate);
	float c = cosf(m_rotate);
	float t = 1-c;
	rotate[0][0] = c; rotate[1][0] = -s;
	rotate[0][1] = s; rotate[1][1] = c;
	rotate[2][2] = t + c;

	return parent*translate*origin2*rotate*scale*origin1;
}

const Node* Node::parent() const {
	return m_parent;
}

void Node::position(const glm::vec2 &position) {
	m_position = position;
}

const glm::vec2& Node::position() const {
	return m_position;
}

glm::vec2& Node::position() {
	return m_position;
}

void Node::size(const glm::vec2 &size) {
	m_size = size;
}

const glm::vec2& Node::size() const {
	return m_size;
}

glm::vec2& Node::size() {
	return m_size;
}

void Node::width(float width) {
	m_size.x = width;
}

const float& Node::width() const {
	return m_size.x;
}

float& Node::width() {
	return m_size.x;
}

void Node::height(float height) {
	m_size.y = height;
}

const float& Node::height() const {
	return m_size.y;
}

float& Node::height() {
	return m_size.y;
}

void Node::scale(const glm::vec2 &scale) {
	m_scale = scale;
}

const glm::vec2& Node::scale() const {
	return m_scale;
}

glm::vec2& Node::scale() {
	return m_scale;
}

void Node::rotate(const float& rotate) {
	m_rotate = rotate;
}

const float& Node::rotate() const {
	return m_rotate;
}

float& Node::rotate() {
	return m_rotate;
}

void Node::origin(const glm::vec2 &origin) {
	m_origin = origin;
}

const glm::vec2& Node::origin() const {
	return m_origin;
}

glm::vec2& Node::origin() {
	return m_origin;
}

void Node::color(const Color &color) {
	m_color = color;
}

const Color& Node::color() const {
	return m_color;
}

Color& Node::color() {
	return m_color;
}

void Node::blendFunc(const BlendFunc &blendFunc) {
	m_blendFunc.src = blendFunc.src;
	m_blendFunc.dst = blendFunc.dst;
}

const BlendFunc& Node::blendFunc() const {
	return m_blendFunc;
}

BlendFunc& Node::blendFunc() {
	return m_blendFunc;
}

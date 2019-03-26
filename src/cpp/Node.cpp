#include "Node.h"
#include <DispatchQueue.h>

Node::Node() :  m_parent(nullptr),
				m_position(0.0f, 0.0f),
				m_size(1.0f, 1.0f),
				m_translate(0.0f, 0.0f),
				m_scale(1.0f, 1.0f),
				m_rotation(b2Rot(0.0f)),
				m_transform(1.0f),
				m_dirtyTransform(true),
				m_origin(0.0f, 0.0f),
				m_flipX(false),
				m_flipY(false),
				m_color(255, 255, 255, 255),
				m_zOrder(0),
				m_blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED) 
{
	
}

Node::~Node() {
	for(auto &child : m_children) {
		child->release();
	}
	for(auto &component : m_components) {
		component->onDestroy();
		delete component;
	}
	onDestroy();
}

Node* Node::create(const std::string &name) {
	Node *node = new Node();
	node->setName(name);
	return node;
}

void Node::update(float dt) {
	for(auto &component : m_components) {
		component->onPreUpdate(dt);
	}

	for(auto &component : m_components) {
		component->onUpdate(dt);
	}

	onUpdate(dt);

	for(auto &component : m_components) {
		component->onPostUpdate(dt);
	}

	for(auto &child : m_children) {
		child->update(dt);
	}
}

void Node::render() {
	onRender();

	for(auto &component : m_components) {
		component->onRender();
	}

	for(auto &child : m_children) {
		child->render();
	}
}

void Node::addNode(Node* child) {
	DispatchQueue::main()->async([this, child]{
		m_children.push_back(child);
		child->acquire();
		child->m_parent = this;
		child->onCreate();
		for(auto &component : child->m_components) {
			component->onCreate();
		}
	});
}

void Node::removeNode(Node* child) {
	for(size_t i = 0; i < m_children.size(); i++) {
		Node* c = m_children[i];
		if(c->getName() == child->getName()) {
			DispatchQueue::main()->async([this, i]{
				Node* c = m_children[i];
				m_children.erase(m_children.begin() + i);
				c->m_parent = nullptr;
				c->release();
			});
			return;
		}
		c->removeNode(child);
	}
}

void Node::addComponent(Component* component) {
	component->setNode(this);
	m_components.push_back(component);
}

void Node::removeComponent(Component* component) {
	auto it = std::find(m_components.begin(), m_components.end(), component);
	if(it != m_components.end()) {
		m_components.erase(it);
	}
}

b2Mat4 Node::getGlobalTransform() const {
	b2Mat4 t = getLocalTransform();
	Node *p = m_parent;
	while(p) {
		t = b2Mul(p->getLocalTransform(), t);
		p = p->m_parent;
	}
	return t;
}

b2Mat4 Node::getLocalTransform() const {
	if(m_dirtyTransform) {
		b2Vec2 p = m_position + m_translate;
		b2Vec2 o = b2Vec2(m_size.x*m_origin.x*0.5f, m_size.y*m_origin.y*0.5f);
		p.x += o.x;
		p.y += o.y;
		b2Mat4 translate = b2CreateTranslation(p);
		m_transform = b2Mul(translate, b2CreateRotation(m_rotation));
		b2Scale(m_transform, m_scale);

		m_transform[3].x += m_transform[0].x*-o.x + m_transform[1].x*-o.y;
		m_transform[3].y += m_transform[0].y*-o.x + m_transform[1].y*-o.y;
		m_transform[3].z += m_transform[0].z*-o.x + m_transform[1].z*-o.y;

		m_dirtyTransform = false;
	}

	return m_transform;
}

Node* Node::getRoot() {
	Node* p = this;
	while(p->m_parent) {
		p = p->m_parent;
	}
	return p;
}

Node* Node::getParent() {
	return m_parent;
}

void Node::setPosition(const b2Vec2 &position) {
	m_position = position;
	m_dirtyTransform = true;
}

void Node::setPosition(const float &x, const float &y) {
	m_position.x = x;
	m_position.y = y;
	m_dirtyTransform = true;
}

const b2Vec2& Node::getPosition() const {
	return m_position;
}

void Node::setSize(const b2Vec2 &size) {
	m_size = size;
	m_dirtyTransform = true;
}

void Node::setSize(const float &x, const float &y) {
	m_size.x = x;
	m_size.y = y;
	m_dirtyTransform = true;
}

const b2Vec2& Node::getSize() const {
	return m_size;
}

void Node::setWidth(const float &width) {
	m_size.x = width;
	m_dirtyTransform = true;
}

const float& Node::getWidth() const {
	return m_size.x;
}

void Node::setHeight(const float &height) {
	m_size.y = height;
	m_dirtyTransform = true;
}

const float& Node::getHeight() const {
	return m_size.y;
}

void Node::setTranslate(const b2Vec2 &translate) {
	m_translate.x = translate.x;
	m_translate.y = translate.y;
}

void Node::setTranslate(const float &x, const float &y) {
	m_translate.x = x;
	m_translate.y = y;
}

const b2Vec2& Node::getTranslate() const {
	return m_translate;
}

void Node::setScale(const b2Vec2 &scale) {
	m_scale = scale;
	m_dirtyTransform = true;
}

void Node::setScale(const float &x, const float &y) {
	m_scale.x = x;
	m_scale.y = y;
	m_dirtyTransform = true;
}

const b2Vec2& Node::getScale() const {
	return m_scale;
}

void Node::setRotation(const b2Rot &rot) {
	m_rotation = rot;
}

void Node::setRotation(const float& theta) {
	m_rotation.Set(theta);
	m_dirtyTransform = true;
}

const b2Rot& Node::getRotation() const {
	return m_rotation;
}

void Node::setOrigin(const b2Vec2 &origin) {
	m_origin = origin;
	m_dirtyTransform = true;
}

void Node::setOrigin(const float &x, const float &y) {
	m_origin.x = x;
	m_origin.y = y;
	m_dirtyTransform = true;
}

const b2Vec2& Node::getOrigin() const {
	return m_origin;
}

void Node::flipX() {
	m_flipX = !m_flipX;
}

void Node::flipY() {
	m_flipY = !m_flipY;
}

void Node::setFlip(const bool& x, const bool& y) {
	m_flipX = x;
	m_flipY = y;
}

void Node::setFlipX(const bool& x) {
	m_flipX = x;
}

void Node::setFlipY(const bool& y) {
	m_flipY = y;
}

const bool& Node::getFlipX() const {
	return m_flipX;
}

const bool& Node::getFlipY() const {
	return m_flipY;
}

void Node::setColor(const Color &color) {
	m_color = color;
}

void Node::setColor(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a) {
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

const Color& Node::getColor() const {
	return m_color;
}

void Node::setZOrder(const int &index) {
	m_zOrder = index;
}

const int& Node::getZOrder() const {
	return m_zOrder;
}

void Node::setBlendFunc(const BlendFunc &blendFunc) {
	m_blendFunc.src = blendFunc.src;
	m_blendFunc.dst = blendFunc.dst;
}

const BlendFunc& Node::getBlendFunc() const {
	return m_blendFunc;
}

sol::object Node::luaIndex(sol::stack_object key, sol::this_state L) {
	auto maybe_string_key = key.as<sol::optional<std::string>>();
	if (maybe_string_key) {
		const std::string& k = *maybe_string_key;
		if(k == "position") {
			return sol::object(L, sol::in_place, &getPosition());
		}
		if(k == "size") {
			return sol::object(L, sol::in_place, &getSize());
		}
		if(k == "width") {
			return sol::object(L, sol::in_place, getWidth());
		}
		if(k == "height") {
			return sol::object(L, sol::in_place, getHeight());
		}
		if(k == "scale") {
			return sol::object(L, sol::in_place, &getScale());
		}
		if(k == "rotation") {
			return sol::object(L, sol::in_place, getRotation().GetAngle());
		}
		if(k == "origin") {
			return sol::object(L, sol::in_place, &getOrigin());
		}
		if(k == "flipX") {
			return sol::object(L, sol::in_place, getFlipX());
		}
		if(k == "flipY") {
			return sol::object(L, sol::in_place, getFlipY());
		}
		if(k == "color") {
			return sol::object(L, sol::in_place, &getColor());
		}
		if(k == "zOrder") {
			return sol::object(L, sol::in_place, getZOrder());
		}
		if(k == "parent") {
			return sol::object(L, sol::in_place, getParent());
		}
		if(k == "root") {
			return sol::object(L, sol::in_place, getRoot());	
		}
	}

	return Object::luaIndex(key, L);
}

void Node::luaNewIndex(sol::stack_object key, sol::stack_object value, sol::this_state L) {
	auto maybe_string_key = key.as<sol::optional<std::string>>();
	if (maybe_string_key) {
		const std::string& k = *maybe_string_key;
		if(k == "position") {
			setPosition(value.as<b2Vec2>());
		}
		if(k == "size") {
			setSize(value.as<b2Vec2>());
		}
		if(k == "width") {
			setWidth(value.as<float>());
		}
		if(k == "height") {
			setHeight(value.as<float>());
		}
		if(k == "scale") {
			setScale(value.as<b2Vec2>());
		}
		if(k == "rotation") {
			setRotation(value.as<float>());
		}
		if(k == "origin") {
			setOrigin(value.as<b2Vec2>());
		}
		if(k == "flipX") {
			setFlipX(value.as<bool>());
		}
		if(k == "flipY") {
			setFlipY(value.as<bool>());
		}
		if(k == "color") {
			setColor(value.as<Color>());
		}
		if(k == "zOrder") {
			setZOrder(value.as<int>());
		}
	}
	Object::luaNewIndex(key, value, L);
}
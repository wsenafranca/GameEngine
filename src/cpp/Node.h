#ifndef NODE_H
#define NODE_H

#include <string>
#include <Math.h>
#include <Color.h>
#include <BlendFunc.h>
#include <Object.h>
#include <Component.h>
#include <vector>

class Node : public Object {
public:
	TYPENAME(Node)

	static Node* create(const std::string &name);
	
	void update(float dt);
	void render();

	void addNode(Node* child);
	void removeNode(Node* child);

	void addComponent(Component* component);
	void removeComponent(Component* component);

	template<class T>
	T* findNodeByName(const std::string &name) {
		for(auto child : m_children) {
			if(child->getName() == name) {
				return child->as<T>();
			}
			auto ret = child->findNodeByName<T>(name);
			if(ret) return ret;
		}
		return nullptr;
	}

	Node* findNodeByType(const std::string &type) {
		for(auto child : m_children) {
			if(child->getTypeName() == type) {
				return child;
			}
			auto ret = child->findNodeByType(type);
			if(ret) return ret;
		}
		return nullptr;
	}

	template<class T>
	T* findComponentByName(const std::string &name) {
		for(auto component : m_components) {
			if(component->getName() == name) {
				return component->as<T>();
			}
		}
		return nullptr;
	}

	Component* findComponentByType(const std::string &type) {
		for(auto component : m_components) {
			if(component->getTypeName() == type) {
				return component;
			}
		}
		return nullptr;
	}

	Node* getRoot();
	Node* getParent();

	b2Mat4 getGlobalTransform() const;
	b2Mat4 getLocalTransform() const;

	void setPosition(const b2Vec2 &position);
	void setPosition(const float &x, const float &y);
	const b2Vec2& getPosition() const;

	void setSize(const b2Vec2 &size);
	void setSize(const float &x, const float &y);
	const b2Vec2& getSize() const;

	void setWidth(const float &width);
	const float& getWidth() const;

	void setHeight(const float &height);
	const float& getHeight() const;

	void setTranslate(const b2Vec2 &translate);
	void setTranslate(const float &x, const float &y);
	const b2Vec2& getTranslate() const;

	void setScale(const b2Vec2 &scale);
	void setScale(const float &x, const float &y);
	const b2Vec2& getScale() const;

	void setRotation(const float& theta);
	void setRotation(const b2Rot &rot);
	const b2Rot& getRotation() const;

	void setOrigin(const b2Vec2 &origin);
	void setOrigin(const float &x, const float &y);
	const b2Vec2& getOrigin() const;

	void flipX();
	void flipY();
	void setFlip(const bool& x, const bool& y);
	void setFlipX(const bool& x);
	void setFlipY(const bool& y);
	const bool& getFlipX() const;
	const bool& getFlipY() const;

	void setZOrder(const int &index);
	const int& getZOrder() const;

	void setColor(const Color &color);
	void setColor(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a);
	const Color& getColor() const;

	void setBlendFunc(const BlendFunc &blendFunc);
	const BlendFunc& getBlendFunc() const;

	virtual sol::object luaIndex(sol::stack_object key, sol::this_state L) override;
	virtual void luaNewIndex(sol::stack_object key, sol::stack_object value, sol::this_state L) override;

protected:
	Node();
	virtual ~Node();
	virtual void onCreate() {}
	virtual void onUpdate(float dt) {}
	virtual void onRender() {}
	virtual void onDestroy() {}

private:
	Node *m_parent;
	std::vector<Node*> m_children;
	b2Vec2 m_position, m_size, m_translate, m_scale, m_origin;
	b2Rot m_rotation;
	bool m_flipX, m_flipY;
	mutable b2Mat4 m_transform;
	mutable bool m_dirtyTransform;
	Color m_color;
	int m_zOrder;
	BlendFunc m_blendFunc;
	std::vector<Component*> m_components;
};

#endif
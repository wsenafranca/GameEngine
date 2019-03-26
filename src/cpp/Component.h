#ifndef COMPONENT_H
#define COMPONENT_H

#include <Object.h>

class Node;

class Component : public Object {
public:
	TYPENAME(Component)

	void setNode(Node *node);
	Node* getNode();
	friend class Node;
protected:
	virtual void onCreate();
	virtual void onPreUpdate(float dt);
	virtual void onUpdate(float dt);
	virtual void onPostUpdate(float dt);
	virtual void onRender();
	virtual void onDestroy();
private:
	Node *m_node;
};

#endif
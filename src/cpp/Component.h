#ifndef COMPONENT_H
#define COMPONENT_H

#include <Object.h>

class Node;

class Component : public Object {
public:
	void node(Node *node);
	Node* node();
	friend class Node;
protected:
	virtual void onCreate();
	virtual void onPreUpdate(float dt);
	virtual void onUpdate(float dt);
	virtual void onPostUpdate(float dt);
	virtual void onDestroy();
private:
	Node *m_node;
};

#endif
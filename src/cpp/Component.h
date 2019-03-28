#ifndef COMPONENT_H
#define COMPONENT_H

#include <Object.h>
#include <AppListener.h>

class Node;

class Component : public Object, public AppListener {
public:
	TYPENAME(Component)

	void setNode(Node *node);
	Node* getNode();
	friend class Node;
protected:
	virtual void onCreate() override;
	virtual void onUpdate(float dt) override;
	virtual void onRender() override;
	virtual void onDestroy() override;
private:
	Node *m_node;
};

#endif
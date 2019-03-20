#include "Component.h"
#include <Node.h>

void Component::node(Node *node) {
	m_node = node;
}

Node* Component::node() {
	return m_node;
}

void Component::onCreate() {

}

void Component::onPreUpdate(float dt) {

}

void Component::onUpdate(float dt) {

}

void Component::onPostUpdate(float dt) {

}

void Component::onDestroy() {

}
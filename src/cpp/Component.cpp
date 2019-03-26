#include "Component.h"
#include <Node.h>

void Component::setNode(Node *node) {
	m_node = node;
}

Node* Component::getNode() {
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

void Component::onRender() {
	
}

void Component::onDestroy() {

}
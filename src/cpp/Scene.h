#ifndef SCENE_H
#define SCENE_H

#include <Node.h>

class Scene : public Node {
public:
	TYPENAME(Scene)
	void onCreate();
};

#endif
#ifndef SCENE_H
#define SCENE_H

#include <Node.h>

class Scene : public Node {
public:
	void run(float dt);
	void onCreate();
};

#endif
#include <Application.h>
#include <iostream>
#include <fstream>
#include <Scene.h>
#include <TMXMap.h>
#include <Camera.h>
#include <LightManager.h>

class MyApplication : public AppDelegate {
	void onCreate() override {
		scene = new Scene();

		scene->addNode(TMXMap::create("map1"));
		add(scene);

		getWindow()->addMouseButtonListener([this](int button, int action, int mods, int x, int y){
			if(action == GLFW_RELEASE) {
				b2Vec2 pos = Camera::current()->unProject(x, 600-y);
				Color c;
				c.r = 100 + rand()%150;
				c.g = 100 + rand()%150;
				c.b = 100 + rand()%150;
				c.a = 150 + rand()%50;
				float dist = rand()%20 + 5;
				if(button == 0) {
					LightManager::instance()->createPointLight(10, c, dist, pos.x, pos.y);
				}
				else if(button == 1) {
					auto player = scene->findNodeByName<Node>("player");
					if(!player) return;

					glm::vec2 v1 = glm::vec2(pos.x, pos.y);
					glm::vec2 v2 = glm::vec2(player->getPosition().x, player->getPosition().y);
					float dir = -glm::degrees(atan2(v1.y-v2.y, v2.x-v1.x));
					LightManager::instance()->createConeLight(10, c, dist, pos.x, pos.y, dir, 45);
				}
			}
		});

		getWindow()->addKeyListener([this](int key, int scancode, int action, int mods) {
			/*
			printf("%d\n", key);
			auto sprite = scene->findNode<Node>("Player");
			if(action != 0 && key == GLFW_KEY_Q) {
				camera.zoom()-=0.1f;
			}
			if(action != 0 && key == GLFW_KEY_E) {
				camera.zoom()+=0.1f;
			}
			if(action != 0 && key == GLFW_KEY_W) {
				sprite->position().y+=50.0f;
			}
			if(action != 0 && key == GLFW_KEY_S) {
				sprite->position().y-=50.0f;
			}
			if(action != 0 && key == GLFW_KEY_A) {
				sprite->position().x-=50.0f;
			}
			if(action != 0 && key == GLFW_KEY_D) {
				sprite->position().x+=50.0f;
			}
			*/
		});
		

		glShadeModel(GL_SMOOTH);
	}

	void onUpdate(float delta) override {
	}

	void onDestroy() override {
	}

	Scene *scene;
	bool ready;
};

int main() {
	try {
		Application::app()->setDelegate(new MyApplication());
		Application::app()->exec();
	} catch(const std::exception &e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
#include <Application.h>
#include <iostream>
#include <fstream>
#include <Shader.h>
#include <glm/glm.hpp>
#include <Scene.h>
#include <ParticleEmitter.h>
#include <TextureManager.h>
#include <Camera.h>
#include <Sprite.h>
#include <SpriteRenderer.h>
#include <LuaScript.h>
#include <ParticleSystem.h>

class MyApplication : public Application {
	void onCreate() {
		camera.viewport(800, 600);
		camera.zoom(2.0f);

		scene = new Scene();
		Sprite *sprite = new Sprite();
		sprite->name("Player");
		sprite->texture(TextureManager::factory()->load("graphics/fog.png"));
		sprite->addComponent(new SpriteRenderer());
		sprite->addComponent(new LuaScript("Player"));
		scene->addNode(sprite);
		sprite->addNode(new ParticleEmitter("katamari"));

		/*
		auto root = ActorManager::factory()->create<Actor>("root");
		root->add(ActorManager::factory()->create<ParticleEmitter>("katamari"));
		root->add(ActorManager::factory()->create<ParticleEmitter>("hexagon"));
		root->add(ActorManager::factory()->create<ParticleEmitter>("mana"));
		*/

		window()->addMouseButtonListener([this](int button, int action, int mods, int x, int y){
			auto sprite = scene->findNode<Node>("Player");
			glm::vec2 p = camera.unProject(x, y);
			sprite->position(p);
		});

		window()->addKeyListener([this](int key, int scancode, int action, int mods) {
			printf("%d\n", key);
			/*
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
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	}

	void onUpdate(float delta) {
		scene->run(delta);
	}

	void onDestroy() {
		delete scene;
	}

	bool ready;
	Scene *scene;
	Camera camera;
};

int main() {
	try {
		MyApplication app;
		app.exec();
	} catch(const std::exception &e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
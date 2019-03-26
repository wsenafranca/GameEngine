#include <Application.h>
#include <iostream>
#include <fstream>
#include <Shader.h>
#include <Scene.h>
#include <TextureManager.h>
#include <Camera.h>
#include <Sprite.h>
#include <LuaScript.h>
#include <RigidBody.h>
#include <ParticleSystem.h>
#include <Physics.h>
#include <TMXMap.h>

class MyApplication : public AppDelegate {
	void onCreate() override {
		scene = new Scene();
		/*
		Sprite *sprite = Sprite::create("Player");
		auto texture = TextureManager::instance()->load("graphics/player.png");
		auto sprites = TextureRegion::split(texture, 4, 3);
		sprite->setTexture(sprites[1]);
		//sprite->addComponent(new LuaScript("Player"));

		scene->addNode(sprite);

		b2BodyDef def;
		def.fixedRotation = false;
		def.type = b2_dynamicBody;
		b2PolygonShape boxShape;
		boxShape.SetAsBox(0.5f, 0.5f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;

		auto body = new RigidBody(&def);
		body->createFixture(&fixtureDef);
		//sprite->addComponent(body);

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -10.0f);
		b2Body* groundBody = Physics::instance()->world->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(100.0f, 1.0f);
		groundBody->CreateFixture(&groundBox, 0.0f);

		//sprite->addNode(Node::create<ParticleEmitter>("katamari"));
		//sprite->addNode(Node::create<ParticleEmitter>("hexagon"));
		//sprite->addNode(Node::create<ParticleEmitter>("mana"));

		auto particle = ParticleSystem::create("katamari");
		particle->setScale(0.01f, 0.01f);
		particle->setPosition(-0.6f, 0.0f);
		particle->setZOrder(sprite->getZOrder()+1);
		//particle->setColor(0, 0, 255, 255);
		sprite->addNode(particle);
		*/

		auto map = TMXMap::create("map1");
		map->setScale(1.0f, 1.0f);
		scene->addNode(map);

		getWindow()->addMouseButtonListener([this](int button, int action, int mods, int x, int y){
			/*
			auto sprite = scene->findNode<Node>("Player");
			glm::vec2 p = camera.unProject(x, y);
			sprite->position(p);
			*/
		});

		getWindow()->addKeyListener([this](int key, int scancode, int action, int mods) {
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
		//glCullFace(GL_FRONT);
	}

	void onUpdate(float delta) override {
		scene->run(delta);
	}

	void onDestroy() override {
		delete scene;
	}

	bool ready;
	Scene *scene;
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
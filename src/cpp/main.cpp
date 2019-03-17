#include <Application.h>
#include <iostream>
#include <fstream>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ActorManager.h>
#include <Scene.h>
#include <ParticleEmitter.h>
#include <BlurRenderer.h>
#include <TextureManager.h>

class MyApplication : public Application {
	void onCreate() {
		renderer = new BlurRenderer();

		auto root = ActorManager::factory()->create<Actor>("root");
		root->add(ActorManager::factory()->create<ParticleEmitter>("katamari"));
		root->add(ActorManager::factory()->create<ParticleEmitter>("hexagon"));
		root->add(ActorManager::factory()->create<ParticleEmitter>("mana"));
		scene = new Scene(root);

		loadShaders();

		//projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 2000.0f);
		zoom = 2.0f;
		projection = glm::ortho(zoom*-400.0f, zoom*400.0f, zoom*-300.0f, zoom*300.0f);
		view = glm::mat4(1.0f);

		window()->addMouseButtonListener([this](int button, int action, int mods, int x, int y){
			glm::vec3 p = glm::unProject(glm::vec3((float)x, 600.0f-y, 0.0f), view, projection, glm::vec4(0.0f, 0.0f, 800.0f, 600.0f));
			auto particles = ActorManager::factory()->find<ParticleEmitter>("mana");
			printf("%s\n", particles->name().c_str());
			particles->data.position[0] = p.x;
			particles->data.position[1] = p.y;
		});
		window()->addKeyListener([this](int key, int scancode, int action, int mods){
			if(action != 0 && key == GLFW_KEY_W) {
				zoom-=0.1f;
			}
			if(action != 0 && key == GLFW_KEY_S) {
				zoom+=0.1f;
			}
			projection = glm::ortho(zoom*-400.0f, zoom*400.0f, zoom*-300.0f, zoom*300.0f);
		});

		glShadeModel(GL_SMOOTH);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void onUpdate(float delta) {
		scene->update(delta);
	}

	void onRender() {
		shader.use();
		glUniformMatrix4fv(glGetUniformLocation(shader, "u_projection"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader, "u_view"), 1, false, glm::value_ptr(view));
		//renderer->begin();
			scene->render(shader);
		//renderer->end();
	}
	void onDestroy() {
		shader.destroy();
		delete scene;
		delete renderer;
	}

	void loadShaders() {
		shader.add("glsl/particle.vert", GL_VERTEX_SHADER);
		shader.add("glsl/particle.frag", GL_FRAGMENT_SHADER);
		shader.create();
	}

	bool ready;
	Scene *scene;
	Shader shader;
	BlurRenderer *renderer;
	
	glm::mat4 view, projection;
	float zoom;
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
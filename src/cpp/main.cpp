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

		scene = new Scene(ActorManager::factory()->create<ParticleEmitter>("particles"));
		auto particles = ActorManager::factory()->find<ParticleEmitter>("particles");
		particles->load("data/rain.json");

		loadShaders();

		//projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 2000.0f);
		projection = glm::ortho(-800.0f, 800.0f, -600.0f, 600.0f);

		eye = glm::vec3(0.0f, 0.0f, 1.0f);

		window()->addMouseButtonListener([this](int button, int action, int mods, int x, int y){
			glm::vec3 p = glm::unProject(glm::vec3((float)x, 600.0f-y, 0.0f), view, projection, glm::vec4(0.0f, 0.0f, 800.0f, 600.0f));
			auto particles = ActorManager::factory()->find<ParticleEmitter>("particles");
			printf("%s\n", particles->name().c_str());
			particles->position.x = p.x;
			particles->position.y = p.y;
		});

		glShadeModel(GL_SMOOTH);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void onUpdate(float delta) {		
		view = glm::mat4(1.0f);//glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
	glm::vec3 eye;
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
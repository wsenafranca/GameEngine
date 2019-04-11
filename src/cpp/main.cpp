#include <app/application.hpp>
#include <graphics/window.hpp>
#include <math/math.hpp>
#include <base/dispatch_queue.hpp>
#include <graphics/camera.hpp>
#include <nodes/sprite.hpp>
#include <nodes/particle/system.hpp>
#include <io/json.hpp>
#include <graphics/shape_renderer.hpp>
#include <physics/shapes/polygon.hpp>
#include <physics/shapes/circle.hpp>
#include <physics/world.hpp>
#include <io/input.hpp>
#include <debug/debug.hpp>
#include <graphics/effects/lighting/point_light.hpp>
#include <graphics/effects/lighting/directional_light.hpp>
#include <graphics/effects/lighting/cone_light.hpp>

class delegate : public app::delegate {
	void create() override {
		world = physics::world::create();
		current = nullptr;

		create_platform(0.0f, -200.0f, 500, 100);
		create_platform(0.0f, -125.0f, 100, 50);
		create_platform(-225.0f, 150.0f, 50, 500);
		//create_platform(50.0f, -150.0f, 150, 80, -30);

		lighting = graphics::effects::lighting::renderer::create(world);

        pulse_light = graphics::effects::lighting::point_light::create(lighting, 50, graphics::color::yellow, 150, -189, 148);
        dir_light = graphics::effects::lighting::directional_light::create(lighting, 100, graphics::color(0.25f, 0.25f, 0.25f, 0.45f), math::radians(-45));
        cone_light = graphics::effects::lighting::cone_light::create(lighting, 50, graphics::color::magenta, 300, 150, 90, glm::radians(255.0f), glm::radians(30.0f));

        zoom = 1.0f;
        graphics::camera::main->zoom(zoom);
	}

	void on_test(int x, int y) {
        printf("%d\n", x+y);
	}


	void create_platform(float x, float y, float width, float height, float angle = 0.0f) {
		physics::body::def def;
		def.position.x = x;
		def.position.y = y;
		def.angle = math::radians(angle);
		def.type = physics::body::type::static_body;
		auto body = world->create_body(def);
		physics::fixture::def fdef;
		auto shape = new physics::shapes::polygon();
		shape->rect(0, 0, width, height);
		fdef.shape = shape;
		body->create_fixture(fdef);
	}

	void update(float dt) override {
		static float r = 60.0f;
		r += dt;
		if(r > 360) r -= 360;
        dir_light->direction(math::radians(r));

        static float d = 80.0f, s = 1.0f;
        d += dt*s*5;
        if(d > 100 || d < 80) s *= -1;
        if(pulse_light) {
            pulse_light->distance(d);
        }

        if(current && math::vector::distance(current->position(), cone_light->position()) <= cone_light->dist()) {
            auto dir = math::atan2(current->position().y - cone_light->position().y, current->position().x - cone_light->position().x);
            cone_light->direction(dir);
        }

		state += dt;
		count_fps++;
		if(state > 1.0f) {
			state = 0.0f;
			graphics::this_window::title(
				title + 
				std::string(" - FPS: ") + std::to_string(count_fps) +
				std::string(" - Particles: " + std::to_string(world->body_count())) +
				std::string(" - angle: " + std::to_string(r))
				);
			count_fps = 0;
		}

		world->update(dt);

		if(io::input::mouse::triggered(0)) {
			auto p = graphics::camera::main->unproject(io::input::mouse::position());
			physics::body::def def;
			def.position.x = p.x;
			def.position.y = p.y;
			def.type = physics::body::type::dynamic_body;
			auto body = world->create_body(def);
			physics::fixture::def fdef;
			auto shape = new physics::shapes::circle();
			shape->radius(16);
			fdef.shape = shape;
			body->create_fixture(fdef);
			body->controller(new physics::controllers::controller2d());
			current = body;
		}
		if(io::input::mouse::triggered(1)) {
			auto p = graphics::camera::main->unproject(io::input::mouse::position());
			graphics::effects::lighting::point_light::create(lighting, 50, graphics::color::red, 150, p.x, p.y);
			printf("%f %f\n", p.x, p.y);
		}

        if(io::input::keyboard::pressed(GLFW_KEY_Q)) {
            zoom+=dt;
            graphics::camera::main->zoom(zoom);
        }
        else if(io::input::keyboard::pressed(GLFW_KEY_E)) {
            zoom-=dt;
            graphics::camera::main->zoom(zoom);
        }

		if(current) {
			if(io::input::keyboard::pressed(GLFW_KEY_A)) {
				current->move_left();
			}
			else if(io::input::keyboard::pressed(GLFW_KEY_D)) {
				current->move_right();
			}
			if(io::input::keyboard::pressed(GLFW_KEY_W)) {
				current->jump_input_down();
			}
			else if(io::input::keyboard::released(GLFW_KEY_W)) {
				current->jump_input_up();
			}
		}
	}

	void render() override {
		for(auto body = world->body_list(); body; body = body->next()) {
			debug::draw(body, graphics::color::green);
		}

		debug::flush();

		lighting->render();
	}

	void destroy() override {
	    lighting = nullptr;
	    world = nullptr;
	}

	float state;
	int count_fps;
	float zoom;
	int num_particles;
	std::string title;
	base::pointer<nodes::sprite> sprite;
	base::pointer<nodes::particle::system> particles;
	base::pointer<nodes::particle::emitter> emitter;

	base::pointer<physics::world> world;
	physics::body* current;
	base::pointer<graphics::effects::lighting::renderer> lighting;
    base::pointer<graphics::effects::lighting::directional_light> dir_light;
    base::pointer<graphics::effects::lighting::point_light> pulse_light;
    base::pointer<graphics::effects::lighting::cone_light> cone_light;
};

int main(int argc, char **argv) {
	graphics::this_window::title(argv[0]);
	graphics::this_window::resize(graphics::size(800, 600));

	app::application app(new delegate);
	return app.exec();
}

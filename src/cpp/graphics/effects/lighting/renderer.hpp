#ifndef GRAPHICS_EFFECTS_LIGHTING_RENDERER_HPP
#define GRAPHICS_EFFECTS_LIGHTING_RENDERER_HPP

#include <graphics/render/protocol.hpp>
#include <graphics/frame_buffer.hpp>
#include <graphics/blend_func.hpp>
#include <graphics/color.hpp>
#include <graphics/shader.hpp>
#include <graphics/mesh.hpp>
#include <graphics/camera.hpp>
#include <physics/world.hpp>
#include <list>

namespace graphics
{

namespace effects
{

namespace lighting
{

class light;

class renderer : public base::object {
public:
	static base::pointer<renderer> create(const base::pointer<physics::world>& world);
	void render();

	friend class lighting::light;

    int light_rendered_last_frame;
    base::pointer<physics::world> _world;
    float x1, x2, y1, y2;

    void resize(int width, int height);
protected:
	explicit renderer(base::pointer<physics::world>  world);

private:
	struct lightmap : public base::object {
		lightmap(renderer* handler, int fbo_width, int fbo_height);
		void render();
		void gaussian_blur();

		base::pointer<graphics::frame_buffer> frame_buffer, pingpong_buffer;
		base::pointer<graphics::mesh> mesh;
		renderer* handler;
		int fbo_width, fbo_height;
		base::pointer<graphics::shader> shadow_shader, without_shadow_shader;
		base::pointer<graphics::shader> blur_shader, diffuse_shader;
		graphics::blend_func diffuse_func, shadow_func, simple_func;
	};

	graphics::color _ambient;
	base::pointer<graphics::shader> _shader;
	base::pointer<renderer::lightmap> _lightmap;
	std::list< base::pointer<lighting::light> > _lights, _disabled_lights;
	base::pointer<graphics::camera> _camera;

	bool _shadow, _blur, _diffuse;
	int _blur_num;
};

}

}

}


#endif
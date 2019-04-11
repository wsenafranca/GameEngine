#include <utility>
#include <graphics/effects/lighting/renderer.hpp>
#include <graphics/window.hpp>
#include <graphics/effects/lighting/light.hpp>

namespace graphics::effects::lighting
{

base::pointer<renderer> renderer::create(const base::pointer<physics::world>& world) {
	return new renderer(world);
}

renderer::renderer(base::pointer<physics::world>  world) :
								_world(std::move(world)),
								_ambient(0.2f, 0.3f, 0.4f, 0.5f),
								x1(0), x2(0), y1(0), y2(0),
								light_rendered_last_frame(0)
{
	_diffuse = false;
	_blur = true;
	_blur_num = 3;
	_shadow = true;
	_shader = graphics::shader::create("_light_shader_");
	_shader->add_source(R"(
		#version 330 core

		layout (location = 0) in vec4 vertex_positions;
		layout (location = 1) in vec4 quad_colors;
		layout (location = 2) in float s;

		uniform mat4 u_projTrans;

		out vec4 v_color;

		void main() {
			v_color = s*quad_colors;
			gl_Position =  u_projTrans*vertex_positions;
		}
	)", GL_VERTEX_SHADER);
	_shader->add_source(R"(
		#version 330 core

		in vec4 v_color;
        out vec4 FragColor;

		void main() {
			FragColor = sqrt(v_color);
		}
	)", GL_FRAGMENT_SHADER);
	_shader->link();

	_camera = graphics::camera::main;

	auto d = graphics::this_window::dimension();
	resize((int)d.size.x, (int)d.size.y);

	connect(&graphics::window::instance, graphics::window::instance.window_did_resize, this, &renderer::resize);
}

void renderer::render() {
    light_rendered_last_frame = 0;

    float x = _camera->position().x;
    float y = _camera->position().y;
    float cam_viewport_width = _camera->viewport().x*_camera->zoom();
    float cam_viewport_height = _camera->viewport().y*_camera->zoom();

    float half_viewport_width = cam_viewport_width * 0.5f;
    x1 = x - half_viewport_width;
    x2 = x + half_viewport_width;

    float half_viewport_height = cam_viewport_height * 0.5f;
    y1 = y - half_viewport_height;
    y2 = y + half_viewport_height;

    glDepthMask(false);
    glEnable(GL_BLEND);
    _lightmap->simple_func.apply();

    for(base::pointer<lighting::light>& light : _lights) {
		light->update();
	}

	if(_shadow || _blur) {
		_lightmap->frame_buffer->bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

    _shader->bind();
    _shader->uniform_matrix("u_projTrans", _camera->transform());
    for(auto& light : _lights) {
        light->render();
    }
    _shader->unbind();

	if(_shadow || _blur) {
		_lightmap->frame_buffer->unbind();
		bool needed = light_rendered_last_frame > 0;
		if (needed && _blur) {
			_lightmap->gaussian_blur();
		}
	}
	_lightmap->render();
}

void renderer::resize(int width, int height) {
	_lightmap = new lightmap(this, width, height);
}

renderer::lightmap::lightmap(renderer* handler, int fbo_width, int fbo_height) : 
								handler(handler), fbo_width(fbo_width), fbo_height(fbo_height),
								diffuse_func(graphics::blend_func{GL_DST_COLOR, GL_ZERO}),
								shadow_func(graphics::blend_func{GL_ONE, GL_ONE_MINUS_SRC_ALPHA}),
								simple_func(graphics::blend_func{GL_SRC_ALPHA, GL_ONE})
{
	frame_buffer = graphics::frame_buffer::create(graphics::frame_buffer::builder(fbo_width, fbo_height)
		.color_buffer(texture::internal_format::rgba, texture::format::rgba, texture::type::type_unsigned_byte));
	pingpong_buffer = graphics::frame_buffer::create(graphics::frame_buffer::builder(fbo_width, fbo_height)
		.color_buffer(texture::internal_format::rgba, texture::format::rgba, texture::type::type_unsigned_byte));

	float vertices[] = {
		-1.0f,  1.0f, 0, 1,
		-1.0f, -1.0f, 0, 0,
         1.0f, -1.0f, 1, 0,
         1.0f,  1.0f, 1, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mesh = graphics::mesh::create();
	mesh->vertices(16*sizeof(float), vertices, mesh::usage::static_draw);
	mesh->indices(6*sizeof(unsigned int), indices, mesh::usage::static_draw);
	mesh->attributes(
		mesh::make_attribute(0, 2, mesh::type::type_float, false, sizeof(float)*4, 0),
		mesh::make_attribute(1, 2, mesh::type::type_float, false, sizeof(float)*4, sizeof(float)*2)
	);

	shadow_shader = graphics::shader::create("shadow_shader");
	shadow_shader->add_source(R"(
		#version 330 core

		layout (location = 0) in vec4 a_position;
		layout (location = 1) in vec2 a_texcoord;

		out vec2 v_texcoord;

		void main() {
			v_texcoord = a_texcoord;
			gl_Position = a_position;
		}
	)", GL_VERTEX_SHADER);
	shadow_shader->add_source(R"(
		#version 330 core

		uniform sampler2D u_texture;
		uniform vec4 ambient;

		in vec2 v_texcoord;

		void main() {
			vec4 c = texture2D(u_texture, v_texcoord);
			gl_FragColor.rgb = c.rgb * c.a + ambient.rgb;
			gl_FragColor.a = ambient.a - c.a;
		}
	)", GL_FRAGMENT_SHADER);
	shadow_shader->link();

	without_shadow_shader = graphics::shader::create("_without_shadow_shader");
	without_shadow_shader->add_source(R"(
		#version 330 core

		layout (location = 0) in vec2 a_position;
		layout (location = 1) in vec2 a_texcoord;

		out vec2 v_texcoord;

		void main() {
			v_texcoord = a_texcoord;
			gl_Position = vec4(a_position, 0, 1);
		}
	)", GL_VERTEX_SHADER);
	without_shadow_shader->add_source(R"(
		#version 330 core

		uniform sampler2D u_texture;
		in vec2 v_texcoord;

		void main() {
			gl_FragColor = texture2D(u_texture, v_texcoord);
		}
	)", GL_FRAGMENT_SHADER);
	without_shadow_shader->link();

	blur_shader = graphics::shader::create("_blur_shader");
	blur_shader->add_source(R"(
		#version 330 core

		layout (location = 0) in vec4 a_position;
		layout (location = 1) in vec2 a_texCoord;

		uniform vec2 dir;
		uniform vec2 fboSize;

		out vec2 v_texCoords0;
		out vec2 v_texCoords1;
		out vec2 v_texCoords2;
		out vec2 v_texCoords3;
		out vec2 v_texCoords4;

		void main() {
			vec2 futher = vec2(3.2307692308 / fboSize.x, 3.2307692308 / fboSize.y );
			vec2 closer = vec2(1.3846153846 / fboSize.x, 1.3846153846 / fboSize.y );

			vec2 f = futher * dir;
			vec2 c = closer * dir;
			v_texCoords0 = a_texCoord - f;
			v_texCoords1 = a_texCoord - c;
			v_texCoords2 = a_texCoord;
			v_texCoords3 = a_texCoord + c;
			v_texCoords4 = a_texCoord + f;
			gl_Position = a_position;
		}
	)", GL_VERTEX_SHADER);
	blur_shader->add_source(R"(
		#version 330 core

		uniform sampler2D u_texture;

		uniform int isDiffuse;

		in vec2 v_texCoords0;
		in vec2 v_texCoords1;
		in vec2 v_texCoords2;
		in vec2 v_texCoords3;
		in vec2 v_texCoords4;

		const float center = 0.2270270270;
		const float close  = 0.3162162162;
		const float far    = 0.0702702703;

		void main() {
		    if(isDiffuse != 0) {
		        gl_FragColor.rgb =  far    * texture2D(u_texture, v_texCoords0).rgb + 
		                            close  * texture2D(u_texture, v_texCoords1).rgb + 
		                            center * texture2D(u_texture, v_texCoords2).rgb + 
		                            close  * texture2D(u_texture, v_texCoords3).rgb +
		                            far    * texture2D(u_texture, v_texCoords4).rgb;
		    }
		    else {
		        gl_FragColor =  far    * texture2D(u_texture, v_texCoords0) +
		                        close  * texture2D(u_texture, v_texCoords1) + 
		                        center * texture2D(u_texture, v_texCoords2) + 
		                        close  * texture2D(u_texture, v_texCoords3) +
		                        far    * texture2D(u_texture, v_texCoords4);
		    }
		}
	)", GL_FRAGMENT_SHADER);
	blur_shader->link();

	diffuse_shader = graphics::shader::create("_diffuse_shader");
	diffuse_shader->add_source(R"(
		#version 330 core

		layout (location = 0) in vec4 a_position;
		layout (location = 1) in vec2 a_texcoord;

		out vec2 f_texcoord;

		void main(void) {
			gl_Position = a_position;
			f_texcoord = a_texcoord;
		}
	)", GL_VERTEX_SHADER);
	diffuse_shader->add_source(R"(
		#version 330 core

		uniform sampler2D u_texture;
        uniform vec4 ambient;

		in vec2 f_texcoord;
		in vec4 f_color;
		in vec2 f_position;

		void main (void) {
			gl_FragColor.rgb = (ambient.rgb + texture2D(u_texture, f_texcoord).rgb);
			gl_FragColor.a = 1.0f;
		}
	)", GL_FRAGMENT_SHADER);
	diffuse_shader->link();
}

void renderer::lightmap::render() {
	bool needed = handler->light_rendered_last_frame > 0;
	frame_buffer->color_texture(0)->bind();
	if(handler->_shadow) {
		graphics::shader* shader;
		graphics::color c = handler->_ambient;

		if(handler->_diffuse) {
			shader = diffuse_shader.get();
            shader->bind();
            shader->uniform4f("ambient", c.r, c.g, c.b, c.a);
			diffuse_func.apply();
		}
		else {
			shader = shadow_shader.get();
            shader->bind();
            shader->uniform4f("ambient", c.r * c.a, c.g * c.a, c.b * c.a, 1.0f - c.a);
			shadow_func.apply();
		}

		mesh->draw_indices(mesh::mode::triangles, 6, mesh::type::type_unsigned_int);
		shader->unbind();
	}
	else if(needed) {
		simple_func.apply();
		without_shadow_shader->bind();
		mesh->draw_indices(mesh::mode::triangles, 6, mesh::type::type_unsigned_int);
		without_shadow_shader->unbind();
	}
	frame_buffer->color_texture(0)->unbind();
}

void renderer::lightmap::gaussian_blur() {
	glDisable(GL_BLEND);
	for(int i = 0; i < handler->_blur_num; i++) {
		frame_buffer->color_texture(0)->bind();
		pingpong_buffer->bind();
		{
			blur_shader->bind();
			blur_shader->uniform2f("dir", 1.0f, 0.0f);
			blur_shader->uniform2f("fboSize", fbo_width, fbo_height);
			blur_shader->uniform1i("isDiffuse", handler->_diffuse);
			mesh->draw_indices(mesh::mode::triangles, 6, mesh::type::type_unsigned_int);
		}
		pingpong_buffer->unbind();

		pingpong_buffer->color_texture(0)->bind();
		frame_buffer->bind();
		{
			blur_shader->bind();
			blur_shader->uniform2f("dir", 0.0f, 0.1f);
			blur_shader->uniform2f("fboSize", fbo_width, fbo_height);
			blur_shader->uniform1i("isDiffuse", handler->_diffuse);
			mesh->draw_indices(mesh::mode::triangles, 6, mesh::type::type_unsigned_int);
		}
		frame_buffer->unbind();
	}
	pingpong_buffer->color_texture(0)->unbind();
	blur_shader->unbind();
	glEnable(GL_BLEND);
}

}

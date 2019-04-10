#include "batch.hpp"

namespace graphics
{

base::pointer<graphics::batch> batch::_default_batch = nullptr;

base::pointer<graphics::batch>& batch::default_batch() {
	if(!_default_batch) {
		_default_batch = batch::create();
	}

	return _default_batch;
}

base::pointer<graphics::batch> batch::create(unsigned int num_sprites) {
	auto b = new graphics::batch();
	b->init(num_sprites);
	return b;
}

batch::batch() : _drawing(false) {
	// starts the context stack
	_stack.push(context());
	_current = &_stack.top();

	color(color::white);
	blend_func(blend_func::alpha_non_premultiplied);
	shader(create_default_shader());
	camera(graphics::camera::main);
	enable_blend();
	enable_draw_with_indices();
	draw_mode(mesh::mode::triangles);
}

batch::~batch() {
	delete [] _buffer;
}

void batch::init(unsigned int num_sprites) {
	_max_vertices = num_sprites*vertex_data::quad_size*10;
	_count_vertices = 0;
	_count_elements = 0;
	_buffer = new float[_max_vertices];

	_current->mesh = mesh::create();
	_current->mesh->vertices(sizeof(float)*_max_vertices, 0, mesh::usage::dynamic_draw);
	_current->mesh->attributes(
		mesh::make_attribute(0, 2, 
			mesh::type::type_float, false, sizeof(float)*5, 0),
		mesh::make_attribute(1, 4, 
			mesh::type::type_unsigned_byte, true, sizeof(float)*5, sizeof(float)*2),
		mesh::make_attribute(2, 2, 
			mesh::type::type_float, false, sizeof(float)*5, sizeof(float)*3)
	);

	unsigned int num_indices = num_sprites*6;
	unsigned int *indices = (unsigned int*) malloc(sizeof(unsigned int)*num_indices);
	for(int i = 0, j = 0; i < num_indices; i+=6, j+=4) {
		indices[i    ] = j;
		indices[i + 1] = (j + 1);
		indices[i + 2] = (j + 2);
		indices[i + 3] = (j + 2);
		indices[i + 4] = (j + 3);
		indices[i + 5] = j;
	}
	_current->mesh->indices(num_indices, indices, mesh::usage::static_draw);
	std::free(indices);
}

void batch::begin() {
	save();
	_drawing = true;
	_current->shader->bind();
}

void batch::render(void *vertices, unsigned int num_attributes, unsigned int num_elements) {
	unsigned int total_vertices = num_attributes*num_elements;
	
	unsigned int rendered = 0;
	while(rendered < total_vertices) {
		int num_vertices = math::clamp(_max_vertices - _count_vertices, 0, total_vertices);
		if(num_vertices == 0) break;
		std::memcpy(_buffer+_count_vertices, vertices, sizeof(float)*num_vertices);
		_count_elements += num_vertices/num_attributes;
		_count_vertices += num_vertices;
		rendered += num_vertices;
		if(_count_vertices + num_attributes >= _max_vertices) {
			flush();
		}
	}
}

void batch::flush() {
	if(_count_elements == 0 || !_texture) return;
	shader()->bind();
	shader()->uniform_matrix("u_projection", camera()->transform());
	shader()->uniform_color("u_color", color());
	shader()->uniform1i("u_enable_texture", 1);

	blend_func().apply();
	_texture->bind();
	_texture->active();
	_current->mesh->vertices(0, sizeof(GLfloat)*_count_vertices, _buffer);
	if(_current->draw_mode == mesh::mode::points) {
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SPRITE);
	}
	if(_current->draw_with_indices) {
		_current->mesh->draw_indices(_current->draw_mode, _count_elements*6, mesh::type::type_unsigned_int);
	}
	else {
		_current->mesh->draw_vertices(_current->draw_mode, 0, _count_vertices);
	}
	if(_current->draw_mode == mesh::mode::points) {
		glDisable(GL_PROGRAM_POINT_SIZE);
		glDisable(GL_POINT_SPRITE);
	}
	
	_texture->unbind();
	_texture->inactive();

	_count_elements = 0;
	_count_vertices = 0;
}

void batch::end() {
	flush();
	_current->shader->unbind();
	restore();
	_drawing = false;
}

void batch::save() {
	_stack.push(*_current);
	_current = &_stack.top();
}

void batch::restore() {
	flush();
	_stack.pop();
	_current = &_stack.top();
}

void batch::texture(const graphics::texture::ref& texture) {
	if(_texture != texture) {
		flush();
		_texture = texture;
	}
}

const graphics::texture::ref& batch::texture() const {
	return _texture;
}

void batch::mesh(const base::pointer<graphics::mesh>& mesh) {
	if(_current->mesh != mesh) {
		flush();
		_current->mesh = mesh;
	}
}

const base::pointer<graphics::mesh>& batch::mesh() const {
	return _current->mesh;
}

base::pointer<graphics::mesh>& batch::mesh() {
	return _current->mesh;
}

void batch::shader(const base::pointer<graphics::shader>& sh) {
	if(_current->shader != sh) {
		flush();
		_current->shader = sh;
		if(_drawing) {
			_current->shader->bind();
		}
	}
}

const base::pointer<graphics::shader>& batch::shader() const {
	return _current->shader;
}

base::pointer<graphics::shader>& batch::shader() {
	return _current->shader;
}

void batch::camera(const base::pointer<graphics::camera>& cam) {
	if(_current->camera != cam) {
		flush();
		_current->camera = cam;
	}
}

const base::pointer<graphics::camera>& batch::camera() const {
	return _current->camera;
}

base::pointer<graphics::camera>& batch::camera() {
	return _current->camera;
}

void batch::color(const graphics::color& color) {
	if(_current->color != color) {
		flush();
		_current->color = color;
	}
}

const graphics::color& batch::color() const {
	return _current->color;
}

graphics::color& batch::color() {
	return _current->color;
}

void batch::blend_func(const graphics::blend_func& func) {
	if(_current->func != func) {
		flush();
		_current->func = func;
	}
}

const graphics::blend_func& batch::blend_func() const {
	return _current->func;
}

graphics::blend_func& batch::blend_func() {
	return _current->func;
}

void batch::enable_blend() {
	if(!_current->blend) {
		flush();
		_current->blend = true;
		glEnable(GL_BLEND);
	}
}

void batch::disable_blend() {
	if(_current->blend) {
		flush();
		_current->blend = false;
		glDisable(GL_BLEND);
	}
}

const bool& batch::blend() const {
	return _current->blend;
}

void batch::draw_mode(const mesh::mode& mode) {
	if(_current->draw_mode != mode) {
		flush();
		_current->draw_mode = mode;
	}
}

const mesh::mode& batch::draw_mode() const {
	return _current->draw_mode;
}

void batch::enable_draw_with_indices() {
	if(!_current->draw_with_indices) {
		flush();
		_current->draw_with_indices = true;
	}
}

void batch::disable_draw_with_indices() {
	if(_current->draw_with_indices) {
		flush();
		_current->draw_with_indices = false;
	}
}

const bool& batch::draw_with_indices() const {
	return _current->draw_with_indices;
}

base::pointer<graphics::shader> batch::create_default_shader() {
	auto shader = graphics::shader::create("default");

	shader->add_source(R"(
		#version 400

		uniform mat4 u_projection;

		layout(location = 0) in vec2 v_position;
		layout(location = 1) in vec4 v_color;
		layout(location = 2) in vec2 v_texcoord;

		out vec4 f_color;
		out vec2 f_texcoord;

		void main(void) {
			f_color = v_color;
			gl_Position = u_projection*vec4(v_position.x, v_position.y, 0.0f, 1.0f);
			f_texcoord = v_texcoord;
		}
	)", GL_VERTEX_SHADER);

	shader->add_source(R"(
		#version 400

		uniform vec4 u_color;
		uniform int u_enable_texture;
		uniform sampler2D tex;

		in vec4 f_color;
		in vec2 f_texcoord;

		out vec4 color;

		void main(void) {
			/* color grading
			float grey = color.r * 0.3 + color.g * 0.59 + color.b * 0.11;
			gl_FragColor = vec4(grey, grey, grey, 1.0);
			*/
			if(u_enable_texture != 0) {
				color = f_color*texture2D(tex, f_texcoord)*u_color;
			}
			else {
				color = f_color*u_color;
			}
		}
	)", GL_FRAGMENT_SHADER);

	shader->link();

	return shader;
}

}
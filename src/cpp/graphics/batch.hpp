#ifndef GRAPHICS_BATCH_HPP
#define GRAPHICS_BATCH_HPP

#include <base/object.hpp>
#include <base/pointer.hpp>
#include <graphics/shader.hpp>
#include <graphics/camera.hpp>
#include <graphics/mesh.hpp>
#include <graphics/color.hpp>
#include <graphics/blend_func.hpp>
#include <graphics/texture.hpp>
#include <math/math.hpp>
#include <graphics/render/protocol.hpp>
#include <graphics/vertex_data.hpp>
#include <stack>

#ifndef DEFAULT_BATCH_NUM_ELEMENTS
#define DEFAULT_BATCH_NUM_ELEMENTS 4000
#endif

namespace graphics
{

class batch : public render::protocol {
public:
	struct context {
		base::pointer<graphics::shader> shader;
		base::pointer<graphics::camera> camera;
		base::pointer<graphics::mesh> mesh;
		graphics::color color;
		graphics::blend_func func;
		bool blend;
		mesh::mode draw_mode;
		bool draw_with_indices;
	};

	static base::pointer<graphics::batch> create(unsigned int num_sprites = DEFAULT_BATCH_NUM_ELEMENTS);
	static base::pointer<graphics::batch>& default_batch();
	virtual ~batch();

	virtual void render(void *vertices, unsigned int num_attributes, unsigned int num_elements);
	virtual void flush();
	void save();
	void restore();

	void texture(const graphics::texture::ref& texture);
	const graphics::texture::ref& texture() const;

	void mesh(const base::pointer<graphics::mesh>& mesh);
	const base::pointer<graphics::mesh>& mesh() const;
	base::pointer<graphics::mesh>& mesh();

	void shader(const base::pointer<graphics::shader> &sh);
	const base::pointer<graphics::shader>& shader() const;
	base::pointer<graphics::shader>& shader();

	void camera(const base::pointer<graphics::camera>& cam);
	const base::pointer<graphics::camera>& camera() const;
	base::pointer<graphics::camera>& camera();

	void color(const graphics::color& color);
	const graphics::color& color() const;
	graphics::color& color();

	void blend_func(const graphics::blend_func& func);
	const graphics::blend_func& blend_func() const;
	graphics::blend_func& blend_func();

	void enable_blend();
	void disable_blend();
	const bool& blend() const;

	void draw_mode(const mesh::mode& mode);
	const mesh::mode& draw_mode() const;

	void enable_draw_with_indices();
	void disable_draw_with_indices();
	const bool& draw_with_indices() const;

protected:
	batch();
	virtual void init(unsigned int num_sprites);
	virtual void begin() override;
	virtual void end() override;

	static base::pointer<graphics::batch> _default_batch;
	static base::pointer<graphics::shader> create_default_shader();

	float *_buffer;
	unsigned int _max_vertices, _count_vertices, _count_elements;
	texture::ref _texture;
	bool _drawing;

private:
	std::stack<context> _stack;
	context *_current;
};

}

#endif
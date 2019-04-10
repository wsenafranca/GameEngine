#ifndef GRAPHICS_FRAME_BUFFER_HPP
#define GRAPHICS_FRAME_BUFFER_HPP

#include <vector>
#include <base/object.hpp>
#include <base/pointer.hpp>
#include <graphics/gl.hpp>
#include <graphics/texture.hpp>

namespace graphics
{

class frame_buffer : public base::object {
public:
	struct builder {
		builder(unsigned int width, unsigned int height);
		builder& color_buffer(const texture::internal_format& internal, const texture::format& format, const texture::type& type);
		builder& stencil_buffer(const texture::internal_format& internal, const texture::format& format, const texture::type& type);
		builder& depth_buffer(const texture::internal_format& internal, const texture::format& format, const texture::type& type);

		unsigned int width, height;
		std::vector<texture::builder> colors;
		texture::builder depth;
		texture::builder stencil;
		bool has_depth;
		bool has_stencil;
	};

	static base::pointer<graphics::frame_buffer> create(const frame_buffer::builder& builder);
	~frame_buffer() override;
	void bind();
	static void unbind();

	const graphics::texture::ref& color_texture(int index = 0) const;
	graphics::texture::ref& color_texture(int index = 0);

	const graphics::texture::ref& depth_texture() const;
	graphics::texture::ref& depth_texture();

	const graphics::texture::ref& stencil_texture() const;
	graphics::texture::ref& stencil_texture();

protected:
	explicit frame_buffer(const frame_buffer::builder& builder);
	static void num_color_buffers(unsigned int num_buffers);
private:
	unsigned int _fbo, _depth_rbo, _stencil_rbo;
	static int _default_fbo;
	std::vector<graphics::texture::ref> _colors;
	graphics::texture::ref _depth, _stencil;
};

}

#endif
#include "frame_buffer.hpp"

namespace graphics
{

int frame_buffer::_default_fbo = -1;

frame_buffer::builder::builder(unsigned int width, unsigned int height) : 
								width(width), height(height), has_depth(false), has_stencil(false) 
{

}

frame_buffer::builder& frame_buffer::builder::color_buffer(const texture::internal_format& internal, const texture::format& format, const texture::type& type) {
	colors.push_back(texture::builder()
						.internal_format(internal)
						.format(format)
						.type(type)
						.size(width, height));
	return *this;
}

frame_buffer::builder& frame_buffer::builder::stencil_buffer(const texture::internal_format& internal, const texture::format& format, const texture::type& type) {
	stencil.internal_format(internal)
			.format(format)
			.type(type)
			.size(width, height);
	has_stencil = true;
	return *this;
}

frame_buffer::builder& frame_buffer::builder::depth_buffer(const texture::internal_format& internal, const texture::format& format, const texture::type& type) {
	depth.internal_format(internal)
			.format(format)
			.type(type)
			.size(width, height);
	has_depth = true;
	return *this;
}

base::pointer<graphics::frame_buffer> frame_buffer::create(const frame_buffer::builder& builder) {
	return new frame_buffer(builder);
}

frame_buffer::frame_buffer(const frame_buffer::builder& builder) : _fbo(0), _depth_rbo(0), _stencil_rbo(0) {
	if(_default_fbo < 0) {
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_default_fbo);
	}

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	for(size_t i = 0; i < builder.colors.size(); i++) {
		auto texture = graphics::texture::create(std::to_string(_fbo)+"_color_"+std::to_string(i), builder.colors[i]);
        texture->min_filter(texture::filter::linear);
        texture->mag_filter(texture::filter::linear);
        texture->wrap_s(texture::wrap::clamp_to_edge);
        texture->wrap_t(texture::wrap::clamp_to_edge);
		texture->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture->object(), 0);
		_colors.push_back(texture);
	}

	if(builder.has_depth) {
		glGenRenderbuffers(1, &_depth_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _depth_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)builder.depth._internal_format, builder.depth._width, builder.depth._height);
		auto texture = graphics::texture::create(std::to_string(_fbo)+"_depth", builder.depth);
        texture->min_filter(texture::filter::linear);
        texture->mag_filter(texture::filter::linear);
        texture->wrap_s(texture::wrap::clamp_to_edge);
        texture->wrap_t(texture::wrap::clamp_to_edge);
		texture->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, (GLenum)texture->target(), texture->object(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_rbo);
		_depth = texture;
	}

	if(builder.has_stencil) {
		glGenRenderbuffers(1, &_stencil_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _stencil_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)builder.stencil._internal_format, builder.stencil._width, builder.stencil._height);
		auto texture = graphics::texture::create(std::to_string(_fbo)+"_stencil", builder.stencil);
        texture->min_filter(texture::filter::linear);
        texture->mag_filter(texture::filter::linear);
        texture->wrap_s(texture::wrap::clamp_to_edge);
        texture->wrap_t(texture::wrap::clamp_to_edge);
		texture->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, (GLenum)texture->target(), texture->object(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencil_rbo);
		_stencil = texture;
	}

	if(builder.colors.size() > 1) {
		num_color_buffers(builder.colors.size());
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, _default_fbo);
}

frame_buffer::~frame_buffer() {
	if(_fbo) glDeleteFramebuffers(1, &_fbo);
	if(_depth_rbo) glDeleteRenderbuffers(1, &_depth_rbo);
	if(_stencil_rbo) glDeleteRenderbuffers(1, &_stencil_rbo);
}

void frame_buffer::num_color_buffers(unsigned int num_buffers) {
	auto *attachments = new unsigned int[num_buffers];
	for(unsigned int i = 0; i < num_buffers; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(num_buffers, attachments);
	delete [] attachments;
}

void frame_buffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void frame_buffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _default_fbo);
}

const graphics::texture::ref& frame_buffer::color_texture(int index) const {
	return _colors[index];
}

graphics::texture::ref& frame_buffer::color_texture(int index) {
	return _colors[index];
}

const graphics::texture::ref& frame_buffer::depth_texture() const {
	return _depth;
}

graphics::texture::ref& frame_buffer::depth_texture() {
	return _depth;
}

const graphics::texture::ref& frame_buffer::stencil_texture() const {
	return _stencil;
}

graphics::texture::ref& frame_buffer::stencil_texture() {
	return _stencil;
}

}
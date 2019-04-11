#include <graphics/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stdexcept>

namespace graphics
{

base::pointer<texture::texture_t> texture::load(const filesystem::path& file) {
	const std::string& filename = file.str();
	if(!file.exists()) {
		throw std::runtime_error("File " + filename + "not found.");
	}
	
	if(!base::cache::contains(filename)) {
		int width, height, n;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n, 4);
		texture::builder builder;
		builder._target = texture::target::texture_2d;
		builder._level = 0;
		builder._internal_format = internal_format::rgba;
		builder._data = data;
		builder._width = width;
		builder._height = height;
		builder._border = 0;
		builder._format = format::rgba;
		builder._type = type::type_unsigned_byte;
		auto texture = create(filename, builder);
		stbi_image_free(data);
		return texture;
	}

	return base::cache::get<texture::texture_t>(filename);
}

base::pointer<texture::texture_t> texture::create(const std::string& name, const texture::builder &builder) {
	bool hit;
	auto texture = base::cache::create<texture::texture_t>(name, &hit);
	if(!hit) {
		texture->load_to_gpu(builder);
		texture->min_filter(filter::nearest);
		texture->mag_filter(filter::nearest);
		texture->wrap_s(wrap::clamp);
		texture->wrap_t(wrap::clamp);
	}
	return texture;
}

base::pointer<texture::texture_t> texture::create(const texture::builder &builder) {
    auto texture = new texture_t("");
    texture->load_to_gpu(builder);
    texture->min_filter(filter::nearest);
    texture->mag_filter(filter::nearest);
    texture->wrap_s(wrap::clamp);
    texture->wrap_t(wrap::clamp);
    return texture;
}

texture::builder& texture::builder::target(const texture::target &target) {
	_target = target;
	return *this;
}

texture::builder& texture::builder::level(const int &level) {
	_level = level;
	return *this;
}

texture::builder& texture::builder::internal_format(const texture::internal_format &internal_format) {
	_internal_format = internal_format;
	return *this;
}

texture::builder& texture::builder::format(const texture::format &format) {
	_format = format;
	return *this;
}

texture::builder& texture::builder::type(const texture::type &type) {
	_type = type;
	return *this;
}

texture::builder& texture::builder::border(const int &border) {
	_border = border;
	return *this;
}

texture::builder& texture::builder::data(const unsigned char* data) {
	_data = data;
	return *this;
}

texture::builder& texture::builder::size(const unsigned int &width, const unsigned int &height) {
	_width = width;
	_height = height;
	return *this;
}

texture::texture_t::texture_t(const std::string& name) : base::cacheable(name), _tex(0) {

}

texture::texture_t::~texture_t() {
	if(_tex) glDeleteTextures(1, &_tex);
}

void texture::texture_t::bind() const {
    //glEnable(static_cast<GLenum>(_target));
	glBindTexture(static_cast<GLenum>(_target), _tex);
}

void texture::texture_t::unbind() const {
	glBindTexture(static_cast<GLenum>(_target), 0);
}

void texture::texture_t::active() const {
	glEnable(static_cast<GLenum>(_target));
}

void texture::texture_t::inactive() const {
	glDisable(static_cast<GLenum>(_target));
}

void texture::texture_t::load_to_gpu(const texture::builder &builder) {
    _target = builder._target;
	glGenTextures(1, &_tex);
	bind();
	glTexImage2D(
		static_cast<GLenum>(_target),
		builder._level,
		static_cast<GLint>(builder._internal_format),
		builder._width,
		builder._height,
		builder._border,
		static_cast<GLenum>(builder._format),
		static_cast<GLenum>(builder._type),
		builder._data);
	_width = builder._width;
	_height = builder._height;
	unbind();
}

const unsigned int& texture::texture_t::width() const {
	return _width;
}

const unsigned int& texture::texture_t::height() const {
	return _height;
}

const texture::target& texture::texture_t::target() const {
	return _target;
}

const unsigned int& texture::texture_t::object() const {
	return _tex;
}

void texture::texture_t::base_level(int value) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_BASE_LEVEL, value);
	unbind();
}

void texture::texture_t::border_color(const graphics::color& color) {
	bind();
	glTexParameterfv(static_cast<GLenum>(_target), GL_TEXTURE_BORDER_COLOR, color);
	unbind();
}

void texture::texture_t::compare_func(const texture::compare_func& func) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(func));
	unbind();
}

void texture::texture_t::compare_mode(const texture::compare_mode& mode) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_COMPARE_MODE, static_cast<GLint>(mode));
	unbind();
}

void texture::texture_t::lod_bias(float value) {
	bind();
	glTexParameterf(static_cast<GLenum>(_target), GL_TEXTURE_LOD_BIAS, value);
	unbind();
}

void texture::texture_t::min_lod(float value) {
	bind();
	glTexParameterf(static_cast<GLenum>(_target), GL_TEXTURE_MIN_LOD, value);
	unbind();
}

void texture::texture_t::max_lod(float value) {
	bind();
	glTexParameterf(static_cast<GLenum>(_target), GL_TEXTURE_MAX_LOD, value);
	unbind();
}

void texture::texture_t::min_filter(const texture::filter& filter) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	unbind();
}

void texture::texture_t::mag_filter(const texture::filter& filter) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	unbind();
}

void texture::texture_t::max_level(int level) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_MAX_LEVEL, level);
	unbind();
}

void texture::texture_t::swizzle_r(const texture::swizzle& s) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_SWIZZLE_R, static_cast<GLint>(s));
	unbind();
}

void texture::texture_t::swizzle_g(const texture::swizzle& s) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_SWIZZLE_G, static_cast<GLint>(s));
	unbind();
}

void texture::texture_t::swizzle_b(const texture::swizzle& s) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_SWIZZLE_B, static_cast<GLint>(s));
	unbind();
}

void texture::texture_t::swizzle_a(const texture::swizzle& s) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_SWIZZLE_A, static_cast<GLint>(s));
	unbind();
}

void texture::texture_t::swizzle_rgba(const texture::swizzle& s) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_SWIZZLE_RGBA, static_cast<GLint>(s));
	unbind();
}

void texture::texture_t::wrap_s(const texture::wrap& w) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(w));
	unbind();
}

void texture::texture_t::wrap_t(const texture::wrap& w) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(w));
	unbind();
}

void texture::texture_t::wrap_r(const texture::wrap& w) {
	bind();
	glTexParameteri(static_cast<GLenum>(_target), GL_TEXTURE_WRAP_R, static_cast<GLint>(w));
	unbind();
}

std::vector<texture::region> texture::texture_t::split(unsigned int row, unsigned int col) {
	int w = _width / col;
	int h = _height / row;
	std::vector<texture::region> regions(w*h);

	float inv_width = 1.0f/_width;
	float inv_height = 1.0f/_height;
	for(int i = 0, k = 0; i < row; i++) {
		for(int j = 0; j < col; j++, k++) {
			regions[k].texture = texture::ref(this);
			regions[k].u1 = (j*w)*inv_width;
			regions[k].v1 = (i*h)*inv_height;
			regions[k].u2 = (j*w + w)*inv_width;
			regions[k].v2 = (i*h + h)*inv_height;
			regions[k].width = w;
			regions[k].height = h;
		}
	}

	return regions;
}

bool texture::texture_t::operator==(const texture_t &t) const {
	return _tex == t._tex;
}

bool texture::texture_t::operator!=(const texture_t &t) const {
	return _tex != t._tex;
}

texture::region::region() : texture(nullptr), u1(0.0f), v1(0.0f), u2(0.0f), v2(0.0f), width(0), height(0) {

}

texture::region::region(const ref& texture) {
	if(texture) {
		this->texture = texture;
		u1 = 0.0f;
		v1 = 1.0f;
		u2 = 1.0f;
		v2 = 0.0f;
		width = texture->width();
		height = texture->height();
	}
	else {
		this->texture = nullptr;
		u1 = 0.0f;
		v1 = 0.0f;
		u2 = 0.0f;
		v2 = 0.0f;
		width = 0;
		height = 0;
	}
}

texture::region::region(const region& r) : texture(r.texture), 
								  u1(r.u1), v1(r.v1), 
								  u2(r.u2), v2(r.v2), 
								  width(r.width), height(r.height)
{

}

texture::region::region(std::nullptr_t) : texture(nullptr), 
								 u1(0.0f), v1(0.0f), 
								 u2(0.0f), v2(0.0f), 
								 width(0), height(0) 
{
}

texture::region& texture::region::operator=(const texture::region& r) {
	texture = r.texture;
	u1 = r.u1;
	v1 = r.v1;
	u2 = r.u2;
	v2 = r.v2;
	width = r.width;
	height = r.height;
	return *this;
}

texture::region& texture::region::operator=(const texture::ref& texture) {
	if(texture) {
		this->texture = texture;
		u1 = 0.0f;
		v1 = 1.0f;
		u2 = 1.0f;
		v2 = 0.0f;
		width = texture->width();
		height = texture->height();
	}
	else {
		this->texture = nullptr;
		u1 = 0.0f;
		v1 = 0.0f;
		u2 = 0.0f;
		v2 = 0.0f;
		width = 0;
		height = 0;
	}
	return *this;
}

bool texture::region::operator==(const texture::region& r) const {
	return  texture == r.texture &&
			u1 == r.u1 && v1 == r.v1 &&
			u2 == r.u2 && v2 == r.v2 &&
			width == r.width && height == r.height;
}

bool texture::region::operator==(const texture::ref& texture) const {
	return this->texture == texture;
}

bool texture::region::operator!=(const texture::region& r) const {
	return  texture != r.texture ||
			u1 != r.u1 || v1 != r.v1 ||
			u2 != r.u2 || v2 != r.v2 ||
			width != r.width || height != r.height;
}

bool texture::region::operator!=(const texture::ref& texture) const {
	return this->texture != texture;
}

}

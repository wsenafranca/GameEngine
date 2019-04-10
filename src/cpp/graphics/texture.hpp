#ifndef GRAPHICS_TEXTURE_HPP
#define GRAPHICS_TEXTURE_HPP

#include <graphics/gl.hpp>
#include <graphics/color.hpp>
#include <base/object.hpp>
#include <base/pointer.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem/path.h>
#include <base/cache.hpp>

namespace graphics
{

namespace texture
{

enum class target {
	texture_1d = 						GL_TEXTURE_1D, 
	texture_1d_array = 					GL_TEXTURE_1D_ARRAY, 
	proxy_texture_1d_array = 			GL_PROXY_TEXTURE_1D_ARRAY,
	texture_2d = 						GL_TEXTURE_2D,
	proxy_texture_2d = 					GL_PROXY_TEXTURE_2D, 
	texture_2d_array = 					GL_TEXTURE_2D_ARRAY, 
	texture_2d_multisample = 			GL_TEXTURE_2D_MULTISAMPLE, 
	texture_2d_multisample_arrays = 	GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 
	texture_3d = 						GL_TEXTURE_3D, 
	texture_cube_map = 					GL_TEXTURE_CUBE_MAP, 
	texture_cube_map_array = 			GL_TEXTURE_CUBE_MAP_ARRAY, 
	texture_rectangle = 				GL_TEXTURE_RECTANGLE,
	proxy_texture_rectangle = 			GL_PROXY_TEXTURE_RECTANGLE,
	texture_cube_map_positive_x =		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	texture_cube_map_negative_x =		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	texture_cube_map_positive_y =		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	texture_cube_map_negative_y =		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	texture_cube_map_positive_z =		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	texture_cube_map_negative_z =		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	proxy_texture_cube_map =			GL_PROXY_TEXTURE_CUBE_MAP
};
enum class filter {
	nearest = GL_NEAREST,
	linear = GL_LINEAR,
	nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
	linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
	nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
	linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR
};
enum class wrap {
	clamp = GL_CLAMP,
	clamp_to_edge = GL_CLAMP_TO_EDGE,
	clamp_to_border = GL_CLAMP_TO_BORDER,
	mirrored_repeat = GL_MIRRORED_REPEAT,
	repeat = GL_REPEAT,
	mirror_clamp_to_edge = GL_MIRROR_CLAMP_TO_EDGE
};
enum class swizzle {
	red = GL_RED,
	green = GL_GREEN,
	blue = GL_BLUE,
	alpha = GL_ALPHA,
	zero = GL_ZERO,
	one = GL_ONE
};
enum class compare_func {
	less_or_equal = GL_LEQUAL,
	greater_or_equal = GL_GEQUAL,
	less = GL_LESS,
	greater = GL_GREATER,
	equal = GL_EQUAL,
	notequal = GL_NOTEQUAL,
	always = GL_ALWAYS,
	never = GL_NEVER
};
enum class compare_mode {
	compare_ref_to_texture = GL_COMPARE_REF_TO_TEXTURE,
	none = GL_NONE
};
enum class internal_format{
	depth_component = GL_DEPTH_COMPONENT,
	depth_stencil = GL_DEPTH_STENCIL,
	red = GL_RED,
	rg = GL_RG,
	rgb = GL_RGB,
	rgba = GL_RGBA,
	r8 = GL_R8,
	r8_snorm = GL_R8_SNORM,
	r16 = GL_R16,
	r16_snorm = GL_R16_SNORM,
	rg8 = GL_RG8,
	rg8_snorm = GL_RG8_SNORM,
	rg16 = GL_RG16,
	rg16_snorm = GL_RG16_SNORM,
	r3_g3_b2 = GL_R3_G3_B2,
	rgb4 = GL_RGB4,
	rgb5 = GL_RGB5,
	rgb8 = GL_RGB8,
	rgb8_snorm = GL_RGB8_SNORM,
	rgb10 = GL_RGB10,
	rgb12 = GL_RGB12,
	rgb16_snorm = GL_RGB16_SNORM,
	rgba2 = GL_RGBA2,
	rgba4 = GL_RGBA4,
	rgb5_a1 = GL_RGB5_A1,
	rgba8 = GL_RGBA8,
	rgba8_snorm = GL_RGBA8_SNORM,
	rgb10_a2 = GL_RGB10_A2,
	rgb10_a2ui = GL_RGB10_A2UI,
	rgba12 = GL_RGBA12,
	rgba16 = GL_RGBA16,
	srgb8 = GL_SRGB8,
	srgb8_alpha8 = GL_SRGB8_ALPHA8,
	r16f = GL_R16F,
	rg16f = GL_RG16F,
	rgb16f = GL_RGB16F,
	rgba16f = GL_RGBA16F,
	r32f = GL_R32F,
	rg32f = GL_RG32F,
	rgb32f = GL_RGB32F,
	rgba32f = GL_RGBA32F,
	r11f_g11f_b10f = GL_R11F_G11F_B10F,
	rgb8_e5 = GL_RGB9_E5,
	r8i = GL_R8I,
	r8ui = GL_R8UI,
	r16i = GL_R16I,
	r16ui = GL_R16UI,
	r32i = GL_R32I,
	r32ui = GL_R32UI,
	rg8i = GL_RG8I,
	rg8ui = GL_RG8UI,
	rg16i = GL_RG16I,
	rg16ui = GL_RG16UI,
	rg32i = GL_RG32I,
	rg32ui = GL_RG32UI,
	rgb8i = GL_RGB8I,
	rgb8ui = GL_RGB8UI,
	rgb16i = GL_RGB16I,
	rgb16ui = GL_RGB16UI,
	rgb32i = GL_RGB32I,
	rgb32ui = GL_RGB32UI,
	rgba8i = GL_RGBA8I,
	rgba8ui = GL_RGBA8UI,
	rgba16i = GL_RGBA16I,
	rgba16ui = GL_RGBA16UI,
	rgba32i = GL_RGBA32I,
	rgba32ui = GL_RGBA32UI,
	compressed_red = GL_COMPRESSED_RED,
	compressed_rg = GL_COMPRESSED_RG,
	compressed_rgb = GL_COMPRESSED_RGB,
	compressed_rgba = GL_COMPRESSED_RGBA,
	compressed_srgb = GL_COMPRESSED_SRGB,
	compressed_srgb_alpha = GL_COMPRESSED_SRGB_ALPHA,
	compressed_red_rgtc1 = GL_COMPRESSED_RED_RGTC1,
	compressed_signed_red_rgtc1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
	compressed_rg_rgtc2 = GL_COMPRESSED_RG_RGTC2,
	compressed_signed_rg_rgtc2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
	compressed_rgba_bptc_unorm = GL_COMPRESSED_RGBA_BPTC_UNORM,
	compressed_srgb_alpha_bptc_unorm = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
	compressed_rgb_bptc_signed_float = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
	compressed_rgb_bptc_unigned_float = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
};
enum class format {
	red = GL_RED,
	rg = GL_RG,
	rgb = GL_RGB,
	bgr = GL_BGR,
	rgba = GL_RGBA,
	bgra = GL_BGRA,
	red_integer = GL_RED_INTEGER,
	rg_integer = GL_RG_INTEGER,
	rgb_integer = GL_RGB_INTEGER,
	bgr_integer = GL_BGR_INTEGER,
	rgba_integer = GL_RGBA_INTEGER,
	bgra_integer = GL_BGRA_INTEGER,
	stencil_index = GL_STENCIL_INDEX,
	depth_component = GL_DEPTH_COMPONENT,
	depth_stencil = GL_DEPTH_STENCIL
};
enum class type {
	type_unsigned_byte = GL_UNSIGNED_BYTE,
	type_byte = GL_BYTE,
	type_unsigned_short = GL_UNSIGNED_SHORT,
	type_short = GL_SHORT,
	type_unsigned_int = GL_UNSIGNED_INT,
	type_int = GL_INT,
	type_half_float = GL_HALF_FLOAT,
	type_float = GL_FLOAT,
	type_unsigned_byte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
	type_unsigned_byte_2_3_3_rev = GL_UNSIGNED_BYTE_2_3_3_REV,
	type_unsigned_short_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
	type_unsigned_short_5_6_5_rev = GL_UNSIGNED_SHORT_5_6_5_REV,
	type_unsigned_short_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
	type_unsigned_short_4_4_4_4_rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	type_unsigned_short_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
	type_unsigned_short_1_5_5_5_rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	type_unsigned_int_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
	type_unsigned_int_8_8_8_8_rev = GL_UNSIGNED_INT_8_8_8_8_REV,
	type_unsigned_int_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
	type_unsigned_int_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV
};

struct builder {
	builder() : 
		_target(target::texture_2d), _level(0), 
		_internal_format(internal_format::rgba), _data(nullptr), 
		_width(0), _height(0), _border(0), 
		_format(format::rgba), _type(type::type_unsigned_byte) {}

	texture::builder& target(const texture::target &target);
	texture::builder& level(const int &level);
	texture::builder& internal_format(const texture::internal_format &internal_format);
	texture::builder& format(const texture::format &format);
	texture::builder& type(const texture::type &type);
	texture::builder& border(const int &border);
	texture::builder& data(const unsigned char* data);
	texture::builder& size(const unsigned int &width, const unsigned int &height);

	texture::target _target;
	int _level;
	texture::internal_format _internal_format;
	const unsigned char* _data;
	unsigned _width, _height;
	int _border;
	texture::format _format;
	texture::type _type;
};

class texture_t;

base::pointer<texture_t> load(const filesystem::path& file);
base::pointer<texture_t> load(const texture::target &target, const filesystem::path& file);
base::pointer<texture_t> create(const std::string& name, const texture::builder &builder);

typedef base::pointer<texture_t> ref;

struct region {
	region();
	region(const ref& texture);
	region(const region& r);
	region(std::nullptr_t);
	region& operator=(const region& r);
	region& operator=(const ref& texture);
	bool operator==(const region& r) const;
	bool operator==(const ref& texture) const;
	bool operator!=(const region& r) const;
	bool operator!=(const ref& texture) const;
	explicit operator bool() const noexcept {
		return texture != nullptr;
	}

	ref texture;
	float u1, v1, u2, v2;
	float width, height;
};

class texture_t : public base::cacheable {
public:
	texture_t(const std::string& name);
	virtual ~texture_t();

	void bind() const;
	void unbind() const;
	void active() const;
	void inactive() const;

	const unsigned int& width() const;
	const unsigned int& height() const;
	const texture::target& target() const;
	const unsigned int& object() const;

	void base_level(int value);
	void border_color(const graphics::color& color);
	void compare_func(const texture::compare_func& func);
	void compare_mode(const texture::compare_mode& mode);
	void lod_bias(float value);
	void min_lod(float value);
	void max_lod(float value);
	void min_filter(const texture::filter& filter);
	void mag_filter(const texture::filter& filter);
	void max_level(int level);
	void swizzle_r(const texture::swizzle& s);
	void swizzle_g(const texture::swizzle& s);
	void swizzle_b(const texture::swizzle& s);
	void swizzle_a(const texture::swizzle& s);
	void swizzle_rgba(const texture::swizzle& s);
	void wrap_s(const texture::wrap& w);
	void wrap_t(const texture::wrap& w);
	void wrap_r(const texture::wrap& w);

	std::vector<region> split(unsigned int row, unsigned int col);

	operator unsigned int() {
		return _tex;
	}

	explicit operator bool() const noexcept {
		return _tex != 0;
	}

	bool operator==(const texture_t &t) const;
	bool operator!=(const texture_t &t) const;

	friend base::pointer<texture_t> texture::load(const texture::target &target, const filesystem::path& file);
	friend base::pointer<texture_t> texture::create(const std::string& name, const texture::builder &builder);

protected:
	void load_to_gpu(const texture::builder &builder);

private:
	unsigned int _tex;
	unsigned int _width, _height;
	texture::target _target;
};

}

}

#endif
#include <graphics/color.hpp>
#include <assert.hpp>

int main() {
	{
		graphics::color c;
		ASSERT(c.r == 0.0f);
		ASSERT(c.g == 0.0f);
		ASSERT(c.b == 0.0f);
		ASSERT(c.a == 0.0f);
	}
	{
		graphics::color c(1.0f, 0.025256f, .5f, 0.0f);
		ASSERT(c.r == 1.0f);
		ASSERT(c.g == 0.025256f);
		ASSERT(c.b == 0.5f);
		ASSERT(c.a == 0.0f);
	}
	{
		graphics::color c = graphics::make_color(255, 0, 0, 255);
		ASSERT(c.r == 1.0f);
		ASSERT(c.g == 0.0f);
		ASSERT(c.b == 0.0f);
		ASSERT(c.a == 1.0f);
	}
	{
		unsigned char color[] = {255, 255, 0, 255};
		graphics::color c = graphics::make_color(color);
		ASSERT(c.r == 1.0f);
		ASSERT(c.g == 1.0f);
		ASSERT(c.b == 0.0f);
		ASSERT(c.a == 1.0f);
	}
	{
		graphics::color c(graphics::make_color(255, 0, 0));
		ASSERT(c.r == 1.0f);
		ASSERT(c.g == 0.0f);
		ASSERT(c.b == 0.0f);
		ASSERT(c.a == 1.0f);
	}
	{
		ASSERT(graphics::color::red == graphics::color(1.0f, 0.0f, 0.0f));
		ASSERT(graphics::color::green == graphics::color(0.0f, 1.0f, 0.0f));
		ASSERT(graphics::color::blue == graphics::color(0.0f, 0.0f, 1.0f));
		ASSERT(graphics::color::cyan == graphics::color(0.0f, 1.0f, 1.0f));
		ASSERT(graphics::color::magenta == graphics::color(1.0f, 0.0f, 1.0f));
		ASSERT(graphics::color::yellow == graphics::color(1.0f, 1.0f, 0.0f));
		ASSERT(graphics::color::black == graphics::color(0.0f, 0.0f, 0.0f));
		ASSERT(graphics::color::white == graphics::color(1.0f, 1.0f, 1.0f));
	}
	{
		ASSERT(graphics::rgb("#ff0000") == graphics::color::red);
		ASSERT(graphics::rgb(0xff0000) == graphics::color::red);

		ASSERT(graphics::rgba("#ff0000ff") == graphics::color::red);
		ASSERT(graphics::rgba(0xff0000ff) == graphics::color::red);

		ASSERT(graphics::argb("#ffff0000") == graphics::color::red);
		ASSERT(graphics::argb(0xffff0000) == graphics::color::red);

		float bits = graphics::color::pack(graphics::color::red);
		ASSERT(graphics::color::unpack(bits) == graphics::color::red);
	}

	return 0;
}
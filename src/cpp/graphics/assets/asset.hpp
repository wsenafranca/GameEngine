#ifndef GRAPHICS_ASSETS_ASSET_HPP
#define GRAPHICS_ASSETS_ASSET_HPP

#include <base/cache.hpp>
#include <filesystem/path.h>

class asset : public base::cacheable {
public:
	asset(const filesystem::path& path);
	virtual void load();
	void complete();
};

#endif
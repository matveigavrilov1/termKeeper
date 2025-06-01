#pragma once

#include "storage/cache.h"

namespace tk
{
class xmlCacheManager
{
public:
	xmlCacheManager();
	cache::shared_ptr_t getCache() const;

	bool parse(const std::string& filename);
	bool dump(const std::string& filename);

private:
	cache::shared_ptr_t cache_;
};
} // namespace tk
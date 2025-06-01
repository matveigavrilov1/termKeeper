#pragma once

#include <map>

#include "core/window.h"


#include "storage/cache.h"
#include "storage/storage.h"

namespace tk
{
class uiImpl
{
public:
	uiImpl(cache::shared_ptr_t cache, storage::shared_ptr_type storage);
	void init();
	int run();

private:
	cache::shared_ptr_t cache_;
	storage::shared_ptr_type storage_;

	std::map<std::string, window::shared_ptr_t> windows_;
};
} // namespace tk
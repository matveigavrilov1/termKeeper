#pragma once

#include <map>

#include "core/window.h"

#include "data/cache.h"
#include "data/storage.h"

namespace ui
{
class uiImpl
{
public:
	uiImpl(data::cache::shared_ptr_t cache, data::storage::shared_ptr_t storage);
	void init();
	int run();

private:
	data::cache::shared_ptr_t cache_;
	data::storage::shared_ptr_t storage_;

	std::map<std::string, core::window::shared_ptr_t> windows_;
};
} // namespace ui
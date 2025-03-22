#pragma once

#include <map>

#include "cli/core/window.h"

#include "clipboardController/interface.h"

#include "storage/cache.h"
#include "storage/storage.h"

namespace tk
{
class cliImpl
{
public:
	cliImpl(clipboardController::shared_ptr_type clc, cache::shared_ptr_type cache, storage::shared_ptr_type storage);
	void init();
	int run();

private:
	clipboardController::shared_ptr_type clc_;
	cache::shared_ptr_type cache_;
	storage::shared_ptr_type storage_;

	std::map<std::string, window::shared_ptr_type> windows_;
};
} // namespace tk
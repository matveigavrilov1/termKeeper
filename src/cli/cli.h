#pragma once

#include <map>

#include "cli/core/window.h"

namespace tk
{
class cli
{
public:
	void init();
	void run();

private:
	std::map<std::string, window::shared_ptr_type> windows_;
};
} // namespace tk
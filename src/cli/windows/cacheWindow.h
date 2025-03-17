#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cli/forms/selectionListForm.h"
#include "cli/windows/borderedWindow.h"

namespace tk
{
class cacheWindow
: public borderedWindow
, public std::enable_shared_from_this<cacheWindow>
{
public:
	cacheWindow(size_t x, size_t y, size_t width, size_t height, const std::string& name = "Cache");
	void update();

	void handleInputEvent(event::shared_ptr_type event) override;

private:
	std::vector<std::string> cache_;
	int activeIndex_ { 0 };
	selectionListForm form_;
};
} // namespace tk
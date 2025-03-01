#pragma once

#include <string>
#include <vector>

#include "cli/forms/selectionListForm.h"
#include "cli/windows/borderedWindow.h"
namespace tk
{
class cacheWindow : public borderedWindow
{
public:
	cacheWindow(size_t x, size_t y, size_t width, size_t height);
	void update();

	void handleInputEvent(event::shared_ptr_type event) override;

	const std::string name{"cache"};
private:
	std::vector<std::string> cache_;
	int activeIndex_{0};
	selectionListForm form_;
};
} // namespace tk
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cli/windows/borderedWindow.h"
#include "cli/forms/selectionListForm.h"

namespace tk
{
class menuWindow
: public borderedWindow
, public std::enable_shared_from_this<menuWindow>
{
public:
	menuWindow(size_t x, size_t y, size_t width, size_t height, const std::string& name = "Menu");

	void update();
	void handleInputEvent(event::shared_ptr_type event) override;

	void addWindow(window::shared_ptr_type win);
	void removeWindow(const std::string& name);

private:
	selectionListForm form_;
	std::vector<window::shared_ptr_type> windows_ {};
};
} // namespace tk
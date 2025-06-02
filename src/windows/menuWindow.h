#pragma once

#include <memory>
#include <string>
#include <vector>

#include "windows/borderedWindow.h"
#include "forms/selectionListForm.h"

namespace wndws
{
class menuWindow final
: public borderedWindow 
, public std::enable_shared_from_this<menuWindow>
{
public:
	menuWindow(const std::string& name = "Menu");

	void update() override;
	void handleInputEvent(core::event::shared_ptr_t event) override;

	void addWindow(window::shared_ptr_t win);
	void removeWindow(const std::string& name);

private:
	forms::selectionListForm form_;
	std::vector<window::shared_ptr_t> windows_ {};
};
} // namespace wndws
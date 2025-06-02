#pragma once

#include <memory>
#include <string>
#include <vector>

#include "windows/utils/controllerWindow.h"
#include "forms/selectionListForm.h"

namespace wndws
{
class menuWindow final
: public controllerWindow 
, public std::enable_shared_from_this<menuWindow>
{
public:
	menuWindow(const std::string& name = "Menu");

	void updateBordered() override;

	void addWindow(window::shared_ptr_t win);
	void removeWindow(const std::string& name);

protected:
	bool handleArrowUpDecorator(const core::inputEvent::keyModifiers &mods) override;
	bool handleArrowDownDecorator(const core::inputEvent::keyModifiers &mods) override;

private:
	forms::selectionListForm form_;
	std::vector<window::shared_ptr_t> windows_ {};
};
} // namespace wndws
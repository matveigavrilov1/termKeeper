#include "windows/menuWindow.h"

#include <algorithm>

#include "core/interface.h"

#include "core/window.h"
#include "utils/logger.h"
#include "windows/utils/controllerWindow.h"

namespace wndws
{
menuWindow::menuWindow(const std::string& name)
: controllerWindow(name)
, form_ { false }
{
	LOG_DBG("Creating menuWindow with name: " << name);
	clear();

	form_.setRelativeSize({ 1, 1 });
	LOG_DBG("Form relative size set to {1, 1}");
}

void menuWindow::updateBordered()
{
	LOG_DBG("Updating menuWindow");
	isThisController() ? form_.showSelected() : form_.unshowSelected();

	LOG_DBG("Displaying form content");
	form_.show(*this);
}

bool menuWindow::handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods)
{
	LOG_DBG("Handling ARROW_UP event");
	auto selected = form_.getSelected();
	LOG_DBG("Deactivating current window: " << selected.uuid);
	core::screen().deactivateWindow(selected.uuid);

	LOG_DBG("Switching to previous item");
	form_.switchUp();

	selected = form_.getSelected();
	LOG_DBG("Activating new selected window: " << selected.uuid);
	core::screen().activateWindow(selected.uuid);
	core::screen().showWindow(selected.uuid, os::console::get());
	return true;
}

bool menuWindow::handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods)
{
	LOG_DBG("Handling ARROW_DOWN event");
	auto selected = form_.getSelected();
	LOG_DBG("Deactivating current window: " << selected.uuid);
	core::screen().deactivateWindow(selected.uuid);

	LOG_DBG("Switching to next item");
	form_.switchDown();

	selected = form_.getSelected();
	LOG_DBG("Activating new selected window: " << selected.uuid);
	core::screen().activateWindow(selected.uuid);
	core::screen().showWindow(selected.uuid, os::console::get());
	return true;
}

void menuWindow::addWindow(window::shared_ptr_t win)
{
	LOG_DBG("Adding window to menu: " << win->name() << " (UUID: " << win->uuid() << ")");
	windows_.push_back(win);

	LOG_DBG("Adding item to form: " << win->name());
	form_.addItem({ win->name(), win->uuid() });

	LOG_DBG("Menu now contains " << windows_.size() << " windows");
}

void menuWindow::removeWindow(const std::string& name)
{
	LOG_DBG("Attempting to remove window: " << name);

	auto it = std::find_if(windows_.begin(), windows_.end(), [&name](auto win) { return win->name() == name; });

	if (it != windows_.end())
	{
		LOG_DBG("Window found, removing from list");
		windows_.erase(it);
		LOG_DBG("Window removed. Remaining windows: " << windows_.size());

		// Note: Should also remove from form_ but implementation is missing
		LOG_DBG("Warning: Window not removed from form (implementation missing)");
	}
	else
	{
		LOG_DBG("Window not found in menu");
	}
}

} // namespace wndws
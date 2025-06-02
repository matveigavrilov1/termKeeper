#include "windows/menuWindow.h"

#include <algorithm>

#include "core/interface.h"
#include "core/utils.h"
#include "core/window.h"
#include "utils/logger.h"

namespace wndws
{
menuWindow::menuWindow(const std::string& name)
: borderedWindow(name)
, form_ { false }
{
	LOG_DBG("Creating menuWindow with name: " << name);
	clear();

	form_.setRelativeSize({ 1, 1 });
	LOG_DBG("Form relative size set to {1, 1}");
}

void menuWindow::update()
{
	LOG_DBG("Updating menuWindow");
	updateSize();

	LOG_DBG("Showing selected item in form");
	form_.showSelected();

	LOG_DBG("Displaying form content");
	form_.show(*this);
}

void menuWindow::handleInputEvent(core::event::shared_ptr_t event)
{
	LOG_DBG("Handling input event in menuWindow");

	if (event->type() != core::INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type: " << event->type());
		return;
	}

	LOG_DBG("Showing selected item in form");
	form_.showSelected();

	auto input = std::static_pointer_cast<core::inputEvent>(event);
	LOG_DBG("Processing input event type: " << input->inputType());

	switch (input->inputType())
	{
		case core::inputEvent::ARROW_UP:
		{
			LOG_DBG("Handling ARROW_UP event");
			auto selected = form_.getSelected();
			LOG_DBG("Deactivating current window: " << selected.uuid);
			core::core::getScreen().deactivateWindow(selected.uuid);

			LOG_DBG("Switching to previous item");
			form_.switchUp();

			selected = form_.getSelected();
			LOG_DBG("Activating new selected window: " << selected.uuid);
			core::core::getScreen().activateWindow(selected.uuid);
			core::core::getScreen().showWindow(selected.uuid, os::console::get());
		}
		break;
		case core::inputEvent::ARROW_DOWN:
		{
			LOG_DBG("Handling ARROW_DOWN event");
			auto selected = form_.getSelected();
			LOG_DBG("Deactivating current window: " << selected.uuid);
			core::core::getScreen().deactivateWindow(selected.uuid);

			LOG_DBG("Switching to next item");
			form_.switchDown();

			selected = form_.getSelected();
			LOG_DBG("Activating new selected window: " << selected.uuid);
			core::core::getScreen().activateWindow(selected.uuid);
			core::core::getScreen().showWindow(selected.uuid, os::console::get());
		}
		break;
		case core::inputEvent::ARROW_RIGHT:
		{
			LOG_DBG("Handling ARROW_RIGHT event");
			auto selected = form_.getSelected();
			LOG_DBG("Changing controller window to: " << selected.uuid);
			core::core::getScreen().changeControllerWindow(selected.uuid);

			LOG_DBG("Pushing unspecified input event");
			pushInputEvent(core::inputEvent::UNSPECIFIED);
		}
		break;
		default: LOG_DBG("Unhandled input event type: " << input->inputType()); break;
	}

	update();
	LOG_DBG("Showing updated window");
	showWindow(shared_from_this());
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
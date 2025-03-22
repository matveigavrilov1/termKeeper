#include "cli/windows/menuWindow.h"

#include <algorithm>

#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/core/window.h"
#include "utils/logger.h"

namespace tk
{
menuWindow::menuWindow(size_t x, size_t y, size_t width, size_t height, const std::string& name)
: borderedWindow(x, y, width, height, name)
, form_(0, 0, width - 2, height - 2, true)
{ }

void menuWindow::update()
{
	borderedWindow::clear();

	form_.updateBuffer();
	form_.show(*this);
}

void menuWindow::handleInputEvent(event::shared_ptr_type event)
{
	if (event->type() != INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type");
		return;
	}
	form_.showSelected();

	auto input = std::static_pointer_cast<inputEvent>(event);

	switch (input->inputType())
	{
		case inputEvent::ARROW_LEFT:
		{
			cli::core::getScreen().deactivateWindow(form_.getSelected());
			form_.switchUp();
			cli::core::getScreen().activateWindow(form_.getSelected());
			cli::core::getScreen().showWindow(form_.getSelected(), cli::core::getConsoleManager());
		}
		break;
		case inputEvent::ARROW_RIGHT:
		{
			cli::core::getScreen().deactivateWindow(form_.getSelected());
			form_.switchDown();
			cli::core::getScreen().activateWindow(form_.getSelected());
			cli::core::getScreen().showWindow(form_.getSelected(), cli::core::getConsoleManager());
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			cli::core::getScreen().changeControllerWindow(form_.getSelected());
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		default: break;
	}

	update();
	showWindow(shared_from_this());
}

void menuWindow::addWindow(window::shared_ptr_type win)
{
	windows_.push_back(win);
	form_.addItem(win->name());
}

void menuWindow::removeWindow(const std::string& name)
{
	auto it = std::find_if(windows_.begin(), windows_.end(), [&name](auto win) { return win->name() == name; });
	if (it != windows_.end())
	{
		windows_.erase(it);
	}
}

} // namespace tk
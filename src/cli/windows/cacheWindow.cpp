#include "cli/windows/cacheWindow.h"

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/windows/borderedWindow.h"
#include "utils/logger.h"
#include <memory>

namespace tk
{
cacheWindow::cacheWindow(size_t x, size_t y, size_t width, size_t height, const std::string& name)
: borderedWindow(x, y, width, height, name)
, form_ {
	0,
	0,
	width - 2,
	height - 2,
}
{
	form_.addItem("test1");
	form_.addItem("test2");
	form_.addItem("test3");
	form_.addItem(name);
}

void cacheWindow::update()
{
	LOG_DBG("Updating Cache Window");
	borderedWindow::clear();

	form_.updateBuffer();
	form_.show(*this);

	LOG_DBG("Pushing event to show Cache Window");

	showWindow(shared_from_this());
}

void cacheWindow::handleInputEvent(event::shared_ptr_type event)
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
		case inputEvent::ARROW_UP:
		{
			if (form_.selectedIndex() == 0)
			{
				cli::core::getScreen().changeControllerWindow("Menu");
				form_.unshowSelected();
				update();
				cli::core::getScreen().show(cli::core::getConsoleManager());
				pushInputEvent(inputEvent::UNKNOWN);
				break;
			}
			form_.switchUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			form_.switchDown();
		}
		break;
		default: break;
	}

	update();
}
} // namespace tk
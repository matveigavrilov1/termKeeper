#include "cli/windows/cacheWindow.h"

#include <memory>

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/windows/borderedWindow.h"
#include "utils/logger.h"

namespace tk
{
cacheWindow::cacheWindow(size_t x, size_t y, size_t width, size_t height)
: borderedWindow(x, y, width, height, "Cache")
, cache_ { "test1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10" }
, form_ { cache_, 0, 0, width - 2, height - 2, false }
{ }

void cacheWindow::update()
{
	LOG_DBG("Updating Cache Window");
	borderedWindow::clear();

	form_.updateBuffer();
	form_.show(*this);

	LOG_DBG("Pushing event to show Cache Window");

	cli::core::getEventManager().pushEvent(showWindowEvent::make(name));
}

void cacheWindow::handleInputEvent(event::shared_ptr_type event)
{
	LOG_DBG("Cache window got event: " << event->type());
	if (!event->data())
	{
		LOG_WRN("Cache window got event with no data");
		return;
	}
	auto data = std::dynamic_pointer_cast<inputEvent::inputData<char>>(event->data());
	if (!data)
	{
		LOG_WRN("Cache window got event with wrong data type");
		return;
	}
	char ch = data->input();
	if (ch == 'w')
	{
		form_.switchUp();
	}
	else if (ch == 'a')
	{
		// backspace
	}
	else if (ch == 's')
	{
		form_.switchDown();
	}
	else if (ch == 'd')
	{
		// text
	}
	else if (ch == 'q')
	{
		setChar(0, 0, 'H');
	}
	else if (ch == 'r')
	{
		setChar(width() - 1, height() - 1, 'H');
	}
	update();
}
} // namespace tk
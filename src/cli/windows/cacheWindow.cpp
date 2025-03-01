#include "cli/windows/cacheWindow.h"

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/windows/borderedWindow.h"
#include "utils/logger.h"

namespace tk
{
cacheWindow::cacheWindow(size_t x, size_t y, size_t width, size_t height)
: borderedWindow(x, y, width, height, "Cache")
, cache_ { "test1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10" }
, form_ {cache_, 0, 0, width - 2, height - 2, true }
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
	if (event->type() == eventType::INPUT_EVENT)
		form_.handleInput(event);

	update();
}
} // namespace tk
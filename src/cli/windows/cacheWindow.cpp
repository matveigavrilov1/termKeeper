#include "cli/windows/cacheWindow.h"
#include "cli/core/utils.h"
#include "cli/windows/borderedWindow.h"
#include "utils/logger.h"

namespace tk
{
cacheWindow::cacheWindow(size_t x, size_t y, size_t width, size_t height)
: borderedWindow(x, y, width, height, "Cache")
, cache_ { "test1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10" }
{ }

void cacheWindow::update()
{
	borderedWindow::clear();
	size_t y = 0;
	for (const auto& cache_item : cache_)
	{
		WORD attr = (y == activeIndex_) ? window::HIGHLIGHT_COLOR : DEFAULT_COLOR;
		utils::insertString(*this, 0, y++, cache_item, attr);
	}
	
	borderedWindow::setChar(10, 10, 'X');
	LOG_DBG("Pushing event to show Cache Window");
	utils::pushShowWindowEvent(name);
}

void cacheWindow::handleInputEvent(event::shared_ptr_type event)
{
	LOG_DBG("Cache window got event: " << event->type());
	auto data = utils::getInputEventData<char>(event);
	if (!data)
	{
		LOG_WRN("Cache window got event with wrong data type");
		return;
	}
	char ch = data->input();
	if (ch == 'w')
	{
		if (activeIndex_ == 0)
		{
			activeIndex_ = static_cast<int>(cache_.size()) - 1;
		}
		else
		{
			activeIndex_ -= 1;
		}
	}
	else if (ch == 'a')
	{
		// backspace
	}
	else if (ch == 's')
	{
		if (activeIndex_ == static_cast<int>(cache_.size()) - 1)
		{
			activeIndex_ = 0;
		}
		else
		{
			activeIndex_ += 1;
		}
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
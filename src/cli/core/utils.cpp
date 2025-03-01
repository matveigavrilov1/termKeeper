#include <string>

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/core/utils.h"

namespace tk
{
namespace utils
{
void fillLine(window& wnd, size_t y, std::string str)
{
	for (size_t i = 0, strl = str.length(), ie = wnd.width(); i < ie && i < strl; ++i)
	{
		wnd.setChar(i, y, str[i]);
	}
}

void fillColumn(window& wnd, size_t x, std::string str)
{
	for (size_t i = 0, strl = str.length(), ie = wnd.height(); i < ie && i < strl; ++i)
	{
		wnd.setChar(x, i, str[i]);
	}
}

void insertString(window& wnd, size_t x, size_t y, const std::string& str, WORD attr)
{
	if (x >= wnd.width() || y >= wnd.height())
	{
		return;
	}

	for (size_t i = 0; i < str.length(); i++)
	{
		if (x + i >= wnd.width())
		{
			break;
		}
		wnd.setChar(x + i, y, str[i]);
		wnd.setAttribute(x + i, y, attr);
	}
}

void highlightLine(window& wnd, size_t y, WORD attr)
{
	if (y >= wnd.height())
	{
		return;
	}

	for (size_t x = 0; x < wnd.width(); x++)
	{
		wnd.setAttribute(x, y, attr);
	}
}

void unhighlightLine(window& wnd, size_t y, WORD attr)
{
	if (y >= wnd.height())
	{
		return;
	}

	for (size_t x = 0; x < wnd.width(); x++)
	{
		wnd.setAttribute(x, y, attr);
	}
}

void clearLine(window& wnd, size_t y, WORD attr)
{
	if (y >= wnd.height())
	{
		return;
	}

	for (size_t x = 0; x < wnd.width(); x++)
	{
		wnd.setChar(x, y, ' ');
		wnd.setAttribute(x, y, attr);
	}
}

void pushShowWindowEvent(const std::string& windowName)
{
	cli::core::getEventManager().pushEvent(std::make_shared<showWindowEvent>(std::make_shared<showWindowEvent::windowData>(windowName)));
}
} // namespace utils
} // namespace tk
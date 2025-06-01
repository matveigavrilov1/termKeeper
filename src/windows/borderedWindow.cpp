#include "windows/borderedWindow.h"

#include <algorithm>
#include <iterator>

#include "core/window.h"
#include "os/console.h"

namespace tk
{

static constexpr window::charInfo HORIZONTAL_LINE = { .ch = 196, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE };
static constexpr window::charInfo VERTICAL_LINE = { .ch = 179, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE };
static constexpr window::charInfo TOP_LEFT_CORNER = { .ch = 218, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE };
static constexpr window::charInfo TOP_RIGHT_CORNER = { .ch = 191, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE };
static constexpr window::charInfo BOTTOM_LEFT_CORNER = { .ch = 192, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE };
static constexpr window::charInfo BOTTOM_RIGHT_CORNER = {
	.ch = 217, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};

void borderedWindow::clear()
{
	window::clear();
	drawBorder();
}

window::position_on_screen borderedWindow::contentPos() const
{
	auto real = window::realPos();
	return { real.x + 1, real.y + 1 };
}

window::window_size borderedWindow::contentSize() const
{
	auto real = window::realSize();
	return {real.width - 2, real.height - 2};
}

bool borderedWindow::isBorder(size_t x, size_t y) const
{
	return x == 0 || x == realWidth() - 1 || y == 0 || y == realHeight() - 1;
}

void borderedWindow::drawBorder()
{
	for (size_t x = 0; x < realWidth(); ++x)
	{
		window::setChar({ x, 0 }, HORIZONTAL_LINE);
	}

	for (size_t x = 0; x < realWidth(); ++x)
	{
		window::setChar({ x, realHeight() - 1 }, HORIZONTAL_LINE);
	}

	for (size_t y = 0; y < realHeight(); ++y)
	{
		window::setChar({ 0, y }, VERTICAL_LINE);
	}

	for (size_t y = 0; y < realHeight(); ++y)
	{
		window::setChar({ realWidth() - 1, y }, VERTICAL_LINE);
	}

	window::setChar({ 0, 0 }, TOP_LEFT_CORNER);
	window::setChar({ realWidth() - 1, 0 }, TOP_RIGHT_CORNER);
	window::setChar({ 0, realHeight() - 1 }, BOTTOM_LEFT_CORNER);
	window::setChar({ realWidth() - 1, realHeight() - 1 }, BOTTOM_RIGHT_CORNER);

	os::console::charBuffer title;
	auto titleStr = name();
	std::transform(titleStr.begin(), titleStr.end(), std::back_inserter(title),
		[](char ch)
		{
			os::console::charInfo res_info;
			res_info.ch = ch;
			return res_info;
		});
	size_t titleLength = titleStr.length();
	size_t titleX = 2;
	if (titleX + titleLength > realWidth())
	{
		titleX = 1;
	}

	for (size_t i = 0; i < titleLength && titleX + i < realWidth() - 1; ++i)
	{
		window::setChar({ titleX + i, 0 }, title[i]);
	}
}

} // namespace tk
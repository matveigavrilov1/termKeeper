#include "cli/windows/borderedWindow.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "os/console.h"
#include "utils/logger.h"

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

void borderedWindow::setChar(position_on_window pos, charInfo ch)
{
	if (pos.x >= width() || pos.y >= width())
	{
		std::string error = "Coordinates (x, y) are out of range: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + " for width: " + std::to_string(width())
			+ " and height: " + std::to_string(height());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}

	size_t realX = pos.x ;
	size_t realY = pos.y;
	window::setChar({ realX, realY }, ch);
}

size_t borderedWindow::innerWidth() const
{
	return window::width() - 2;
}

size_t borderedWindow::innerHeight() const
{
	return window::height() - 2;
}

bool borderedWindow::isBorder(size_t x, size_t y) const
{
	return x == 0 || x == width() - 1 || y == 0 || y == height() - 1;
}

void borderedWindow::drawBorder()
{
	for (size_t x = 0; x < width(); ++x)
	{
		window::setChar({ x, 0 }, HORIZONTAL_LINE);
	}

	for (size_t x = 0; x < width(); ++x)
	{
		window::setChar({ x, height() - 1 }, HORIZONTAL_LINE);
	}

	for (size_t y = 0; y < height(); ++y)
	{
		window::setChar({ 0, y }, VERTICAL_LINE);
	}

	for (size_t y = 0; y < height(); ++y)
	{
		window::setChar({ width() - 1, y }, VERTICAL_LINE);
	}

	window::setChar({ 0, 0 }, TOP_LEFT_CORNER);
	window::setChar({ width() - 1, 0 }, TOP_RIGHT_CORNER);
	window::setChar({ 0, height() - 1 }, BOTTOM_LEFT_CORNER);
	window::setChar({ width() - 1, height() - 1 }, BOTTOM_RIGHT_CORNER);

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
	if (titleX + titleLength > width())
	{
		titleX = 1;
	}

	for (size_t i = 0; i < titleLength && titleX + i < width() - 1; ++i)
	{
		window::setChar({ titleX + i, 0 }, title[i]);
	}
}

} // namespace tk
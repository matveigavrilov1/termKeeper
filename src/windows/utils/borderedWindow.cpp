#include "windows/utils/borderedWindow.h"

#include <algorithm>
#include <iterator>

#include "core/interface.h"
#include "core/window.h"
#include "os/console.h"

namespace wndws
{

static constexpr core::window::charInfo HORIZONTAL_LINE = {
	.ch = 196, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};
static constexpr core::window::charInfo VERTICAL_LINE = {
	.ch = 179, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};
static constexpr core::window::charInfo TOP_LEFT_CORNER = {
	.ch = 218, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};
static constexpr core::window::charInfo TOP_RIGHT_CORNER = {
	.ch = 191, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};
static constexpr core::window::charInfo BOTTOM_LEFT_CORNER = {
	.ch = 192, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};
static constexpr core::window::charInfo BOTTOM_RIGHT_CORNER = {
	.ch = 217, .bgColor = os::console::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE
};

void borderedWindow::clear()
{
	window::clear();
	drawBorder();
}

void borderedWindow::update()
{
	setHighlightTitle(core::controllerm().active(uuid()));
	updateSize();
	drawBorder();
	drawTitle();
	updateBordered();
}

void borderedWindow::updateBordered() { }

core::window::position_on_screen borderedWindow::contentPos() const
{
	auto real = window::realPos();
	return { real.x + 1, real.y + 1 };
}

core::window::window_size borderedWindow::contentSize() const
{
	auto real = window::realSize();
	return { real.width - 2, real.height - 2 };
}

void borderedWindow::setHighlightTitle(bool highlight)
{
	highlightTitle_ = highlight;
}

bool borderedWindow::isBorder(size_t x, size_t y) const
{
	return x == 0 || x == realWidth() - 1 || y == 0 || y == realHeight() - 1;
}

void borderedWindow::drawBorder()
{
	for (size_t x = 0; x < realWidth(); ++x)
	{
		core::window::setChar({ x, 0 }, HORIZONTAL_LINE);
	}

	for (size_t x = 0; x < realWidth(); ++x)
	{
		core::window::setChar({ x, realHeight() - 1 }, HORIZONTAL_LINE);
	}

	for (size_t y = 0; y < realHeight(); ++y)
	{
		core::window::setChar({ 0, y }, VERTICAL_LINE);
	}

	for (size_t y = 0; y < realHeight(); ++y)
	{
		core::window::setChar({ realWidth() - 1, y }, VERTICAL_LINE);
	}

	core::window::setChar({ 0, 0 }, TOP_LEFT_CORNER);
	core::window::setChar({ realWidth() - 1, 0 }, TOP_RIGHT_CORNER);
	core::window::setChar({ 0, realHeight() - 1 }, BOTTOM_LEFT_CORNER);
	core::window::setChar({ realWidth() - 1, realHeight() - 1 }, BOTTOM_RIGHT_CORNER);
}

void borderedWindow::drawTitle()
{
	os::console::charBuffer title;
	auto titleStr = name();
	auto bgColor = highlightTitle_ ? os::console::color::CONSOLE_COLOR_WHITE : os::console::color::CONSOLE_COLOR_BLACK;
	auto txtColor = highlightTitle_ ? os::console::color::CONSOLE_COLOR_BLACK : os::console::color::CONSOLE_COLOR_WHITE;

	std::transform(titleStr.begin(), titleStr.end(), std::back_inserter(title),
		[bgColor, txtColor](char ch)
		{
			os::console::charInfo res_info;
			res_info.ch = ch;
			res_info.bgColor = bgColor;
			res_info.txtColor = txtColor;
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
		core::window::setChar({ titleX + i, 0 }, title[i]);
	}
}


} // namespace wndws
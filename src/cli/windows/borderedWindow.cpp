#include "cli/windows/borderedWindow.h"

#include <stdexcept>

#include "utils/logger.h"

namespace tk
{

static constexpr unsigned char HORIZONTAL_LINE = 196;			// ─
static constexpr unsigned char VERTICAL_LINE = 179;				// │
static constexpr unsigned char TOP_LEFT_CORNER = 218;			// ┌
static constexpr unsigned char TOP_RIGHT_CORNER = 191;		// ┐
static constexpr unsigned char BOTTOM_LEFT_CORNER = 192;	// └
static constexpr unsigned char BOTTOM_RIGHT_CORNER = 217; // ┘

borderedWindow::borderedWindow(size_t x, size_t y, size_t width, size_t height, const std::string& name)
: window(x, y, width, height, name)
{
	if (width < 3 || height < 3)
	{
		throw std::invalid_argument("Width and height must be at least 3 to accommodate borders");
	}
	drawBorder();
}

borderedWindow::borderedWindow(const borderedWindow& other)
: window(other)
{
	drawBorder();
}

borderedWindow::borderedWindow(borderedWindow&& other) noexcept
: window(std::move(other))
{
	drawBorder();
}

borderedWindow& borderedWindow::operator= (const borderedWindow& other)
{
	if (this != &other)
	{
		window::operator= (other);
		drawBorder();
	}
	return *this;
}

borderedWindow& borderedWindow::operator= (borderedWindow&& other) noexcept
{
	if (this != &other)
	{
		window::operator= (std::move(other));
		drawBorder();
	}
	return *this;
}

void borderedWindow::clear()
{
	window::clear();
	drawBorder();
}

void borderedWindow::setChar(size_t x, size_t y, char ch)
{
	if (x >= innerWidth() || y >= innerHeight())
	{
		std::string error = "Coordinates (x, y) are out of range: " + std::to_string(x) + ", " + std::to_string(y) + " for width: " + std::to_string(innerWidth())
			+ " and height: " + std::to_string(innerHeight());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}

	size_t realX = x + 1;
	size_t realY = y + 1;
	window::setChar(realX, realY, ch);
}

void borderedWindow::setAttribute(size_t x, size_t y, WORD attr)
{
	if (x >= innerWidth() || y >= innerHeight())
	{
		std::string error = "Coordinates (x, y) are out of range: " + std::to_string(x) + ", " + std::to_string(y) + " for width: " + std::to_string(innerWidth())
			+ " and height: " + std::to_string(innerHeight());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}

	size_t realX = x + 1;
	size_t realY = y + 1;
	window::setAttribute(realX, realY, attr);
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
		window::setChar(x, 0, HORIZONTAL_LINE);
	}

	for (size_t x = 0; x < width(); ++x)
	{
		window::setChar(x, height() - 1, HORIZONTAL_LINE);
	}

	for (size_t y = 0; y < height(); ++y)
	{
		window::setChar(0, y, VERTICAL_LINE);
	}

	for (size_t y = 0; y < height(); ++y)
	{
		window::setChar(width() - 1, y, VERTICAL_LINE);
	}

	window::setChar(0, 0, TOP_LEFT_CORNER);
	window::setChar(width() - 1, 0, TOP_RIGHT_CORNER);
	window::setChar(0, height() - 1, BOTTOM_LEFT_CORNER);
	window::setChar(width() - 1, height() - 1, BOTTOM_RIGHT_CORNER);

	auto title = name();
	size_t titleLength = title.length();
	size_t titleX = 2;
	if (titleX + titleLength > width())
	{
		titleX = 1;
	}

	for (size_t i = 0; i < titleLength && titleX + i < width() - 1; ++i)
	{
		window::setChar(titleX + i, 0, title[i]);
	}
}

} // namespace tk
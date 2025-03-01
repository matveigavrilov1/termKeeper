#include "cli/core/consolem.h"

#include <cassert>
#include <climits>
#include <cstdlib>

#include "utils/logger.h"

namespace tk
{

bool consolem::init()
{
	hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole_ == nullptr || !updateSize())
	{
		LOG_ERR("Failed to initialize console");
		return false;
	}

	return true;
}

bool consolem::write(const std::vector<CHAR_INFO>& buffer, size_t x, size_t y, size_t width, size_t height)
{
	LOG_DBG("Writing buffer to console: " << x << ", " << y << ", " << width << ", " << height);

	if (width == 0 || height == 0 || buffer.size() < width * height)
	{
		LOG_ERR("Invalid buffer size: " << buffer.size() << " for width: " << width << " and height: " << height_);
		return false;
	}

	// after this check static_cast will be correct
	if (x >= width_ || y >= height_ || (x + width) > width_ || (y + height) > height_)
	{
		LOG_ERR("Invalid position: " << x << ", " << y << " for width: " << width << " and height: " << height);
		return false;
	}

	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };

	COORD bufferCoord = { 0, 0 };

	SMALL_RECT writeRegion = { static_cast<SHORT>(x), static_cast<SHORT>(y), static_cast<SHORT>(x + width - 1), static_cast<SHORT>(y + height - 1) };

	if (!WriteConsoleOutput(hConsole_, buffer.data(), bufferSize, bufferCoord, &writeRegion))
	{
		LOG_ERR("Failed to write to console");
		return false;
	}

	return true;
}

bool consolem::setCursorVisibility(bool visible)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(hConsole_, &cursorInfo))
	{
		return false;
	}
	cursorInfo.bVisible = visible;
	if (!SetConsoleCursorInfo(hConsole_, &cursorInfo))
	{
		return false;
	}
	return true;
}

bool consolem::setCursorPosition(size_t x, size_t y)
{
	COORD coord;
	coord.X = static_cast<SHORT>(x);
	coord.Y = static_cast<SHORT>(y);

	if (!SetConsoleCursorPosition(hConsole_, coord))
	{
		return false;
	}
	return true;
}

bool consolem::updateSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hConsole_, &csbi))
	{
		LOG_ERR("Failed to get console screen buffer info");
		return false;
	}

	width_ = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	LOG_DBG("Console size: " << width_ << "x" << height_);
	assert(width_ < SHRT_MAX);
	assert(height_ < SHRT_MAX);
	return true;
}

size_t consolem::height()
{
	return height_;
}

size_t consolem::width()
{
	return width_;
}

} // namespace tk
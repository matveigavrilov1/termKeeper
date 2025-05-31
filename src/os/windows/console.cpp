#include "os/console.h"

#include <algorithm>
#include <windows.h>
#include <memory>
#include <stdexcept>

#include "utils/logger.h"

namespace os
{

struct console::impl
{
	HANDLE hConsole;
	bool isVisible;

	impl()
	: hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	, isVisible(true)
	{
		if (hConsole == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("Failed to get console handle");
		}
	}

	~impl() = default;

	WORD convertForegroundColor(color color)
	{
		switch (color)
		{
			case CONSOLE_COLOR_BLACK: return 0;
			case CONSOLE_COLOR_RED: return FOREGROUND_RED;
			case CONSOLE_COLOR_GREEN: return FOREGROUND_GREEN;
			case CONSOLE_COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
			case CONSOLE_COLOR_BLUE: return FOREGROUND_BLUE;
			case CONSOLE_COLOR_MAGENTA: return FOREGROUND_RED | FOREGROUND_BLUE;
			case CONSOLE_COLOR_CYAN: return FOREGROUND_GREEN | FOREGROUND_BLUE;
			case CONSOLE_COLOR_WHITE: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			default: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}

	WORD convertBackgroundColor(color color)
	{
		switch (color)
		{
			case CONSOLE_COLOR_BLACK: return 0;
			case CONSOLE_COLOR_RED: return BACKGROUND_RED;
			case CONSOLE_COLOR_GREEN: return FOREGROUND_GREEN;
			case CONSOLE_COLOR_YELLOW: return BACKGROUND_RED | BACKGROUND_GREEN;
			case CONSOLE_COLOR_BLUE: return BACKGROUND_BLUE;
			case CONSOLE_COLOR_MAGENTA: return BACKGROUND_RED | BACKGROUND_BLUE;
			case CONSOLE_COLOR_CYAN: return BACKGROUND_GREEN | BACKGROUND_BLUE;
			case CONSOLE_COLOR_WHITE: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
			default: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
		}
	}

	std::vector<CHAR_INFO> convertBuffer(const charBuffer& buffer)
	{
		std::vector<CHAR_INFO> res;
		res.reserve(buffer.size());
		std::transform(buffer.begin(), buffer.end(), std::back_inserter(res),
		[this](charInfo info)
		{
			CHAR_INFO res_info;
			res_info.Char.AsciiChar = info.ch;
			res_info.Attributes = convertBackgroundColor(info.bgColor) | convertForegroundColor(info.txtColor);
			LOG_INF(char(info.ch));
			return res_info;
		});

		return res;
	}
};

console::shared_ptr_t console::get()
{
	static auto instance = std::make_shared<console>();
	return instance;
}

console::console()
: pimpl_(std::make_unique<impl>())
{ }

console::~console() = default;

void console::clear()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(pimpl_->hConsole, &csbi))
		return;

	COORD topLeft = { 0, 0 };
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;
	FillConsoleOutputCharacter(pimpl_->hConsole, ' ', length, topLeft, &written);
	FillConsoleOutputAttribute(pimpl_->hConsole, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(pimpl_->hConsole, topLeft);
}

void console::setCursorPosition(position pos)
{
	COORD coord = { static_cast<SHORT>(pos.x), static_cast<SHORT>(pos.y) };
	SetConsoleCursorPosition(pimpl_->hConsole, coord);
}

bool console::setCursorVisibility(bool visible)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(pimpl_->hConsole, &cursorInfo))
	{
		return false;
	}
	cursorInfo.bVisible = visible;
	if (!SetConsoleCursorInfo(pimpl_->hConsole, &cursorInfo))
	{
		return false;
	}
	return true;
}

console::position console::getCursorPosition() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(pimpl_->hConsole, &csbi))
		return { 0, 0 };

	return { static_cast<size_t>(csbi.dwCursorPosition.X), static_cast<size_t>(csbi.dwCursorPosition.Y) };
}

bool console::setConsoleSize(size sz)
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Cannot get console window", "Error", MB_ICONERROR);
		return false;
	}

	RECT rect;
	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, rect.left, rect.top, sz.width, sz.height, TRUE);
	return true;
}

console::size console::getConsoleSize() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(pimpl_->hConsole, &csbi))
		return { 80, 25 }; // default size

	size_t width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	size_t height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	LOG_DBG("Console size: " << width << ", " << height);
	return { width, height };
}

void console::putChar(position pos, char c)
{
	setCursorPosition(pos);
	DWORD written;
	WriteConsoleA(pimpl_->hConsole, &c, 1, &written, nullptr);
}

void console::putString(position pos, const char* str)
{
	setCursorPosition(pos);
	DWORD written;
	WriteConsoleA(pimpl_->hConsole, str, static_cast<DWORD>(strlen(str)), &written, nullptr);
}

void console::setTextColor(position pos, color color)
{
	setCursorPosition(pos);
	WORD attr = pimpl_->convertForegroundColor(color);
	SetConsoleTextAttribute(pimpl_->hConsole, attr);
}

void console::setBgColor(position pos, color color)
{
	setCursorPosition(pos);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(pimpl_->hConsole, &csbi);
	WORD attr = (pimpl_->convertForegroundColor(color) << 4) | (csbi.wAttributes & 0x0F);
	SetConsoleTextAttribute(pimpl_->hConsole, attr);
}

bool console::write(const charBuffer& buffer, position pos, size sz)
{
	LOG_DBG("Writing buffer to console: " << pos.x << ", " << pos.y << ", " << sz.width << ", " << sz.height);

	if (sz.width == 0 || sz.height == 0 || buffer.size() < sz.width * sz.height)
	{
		LOG_ERR("Invalid buffer size: " << buffer.size() << " for width: " << sz.width << " and height: " << sz.height);
		return false;
	}

	auto consoleSize = getConsoleSize();

	// after this check static_cast will be correct
	if (pos.x >= consoleSize.width || pos.y >= consoleSize.height || (pos.x + sz.width) > consoleSize.width || (pos.y + sz.height) > consoleSize.height)
	{
		LOG_ERR("Invalid position: " << pos.x << ", " << pos.y << " for width: " << sz.width << " and height: " << sz.height);
		return false;
	}

	COORD bufferSize = { static_cast<SHORT>(sz.width), static_cast<SHORT>(sz.height) };

	COORD bufferCoord = { 0, 0 };

	SMALL_RECT writeRegion = { static_cast<SHORT>(pos.x), static_cast<SHORT>(pos.y), static_cast<SHORT>(pos.x + sz.width - 1),
		static_cast<SHORT>(pos.y + sz.height - 1) };

	auto convertedBuffer = pimpl_->convertBuffer(buffer);

	if (!WriteConsoleOutput(pimpl_->hConsole, convertedBuffer.data(), bufferSize, bufferCoord, &writeRegion))
	{
		LOG_ERR("Failed to write to console");
		return false;
	}

	return true;
}

void console::fillArea(position pos1, position pos2, char fillChar)
{
	COORD topLeft = { static_cast<SHORT>(pos1.x), static_cast<SHORT>(pos1.y) };
	COORD bottomRight = { static_cast<SHORT>(pos2.x), static_cast<SHORT>(pos2.y) };

	SHORT width = bottomRight.X - topLeft.X + 1;
	SHORT height = bottomRight.Y - topLeft.Y + 1;

	for (SHORT y = 0; y < height; ++y)
	{
		COORD lineStart = { topLeft.X, static_cast<SHORT>(topLeft.Y + y) };
		DWORD written;
		FillConsoleOutputCharacterA(pimpl_->hConsole, fillChar, width, lineStart, &written);
	}
}

void console::clearArea(position pos1, position pos2)
{
	fillArea(pos1, pos2, ' ');
}

void console::resetColors()
{
	SetConsoleTextAttribute(pimpl_->hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

} // namespace os
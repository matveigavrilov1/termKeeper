#include "os/console.h"

#include <algorithm>
#include <windows.h>
#include <memory>
#include <stdexcept>
#include <thread>

#include "core/events.h"
#include "core/utils.h"
#include "core/interface.h"
#include "utils/logger.h"

namespace os
{

struct console::impl
{
	HANDLE hConsole;
	HWND hWnd;
	bool isVisible;
	std::atomic<bool> running { false };
	std::jthread inputHandlerThread;

	impl()
	: hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	, hWnd(GetConsoleWindow())
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
				return res_info;
			});

		return res;
	}

	void inputHandler()
	{
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
		{
			std::cerr << "GetStdHandle failed (" << GetLastError() << ")\n";
			running = false; // Stop the thread if we can't get the input handle.
			return;
		}

		INPUT_RECORD ir[128];
		DWORD cNumRead;

		while (running)
		{
			WaitForSingleObject(hStdin, INFINITE); // Ждем события ввода
			if (!ReadConsoleInput(hStdin, ir, 128, &cNumRead))
			{
				continue;
			}

			for (DWORD i = 0; i < cNumRead; i++)
			{
				if (ir[i].EventType == WINDOW_BUFFER_SIZE_EVENT)
				{
					core::screen().updateAll();
					core::screen().show(get());
				}
				else if (ir[i].EventType == KEY_EVENT)
				{
					KEY_EVENT_RECORD& ker = ir[i].Event.KeyEvent;
					if (ker.bKeyDown)
					{
						char asciiChar = ker.uChar.AsciiChar;
						WORD virtualKeyCode = ker.wVirtualKeyCode;

						bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
						bool ctrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
						bool altPressed = (GetKeyState(VK_MENU) & 0x8000) != 0;

						LOG_DBG("Key pressed: " << (int)virtualKeyCode << " Char: " << (int)asciiChar);

						core::inputEvent::type eventType = core::inputEvent::UNSPECIFIED;

						// Handle special keys
						switch (virtualKeyCode)
						{
							case VK_RETURN: eventType = core::inputEvent::ENTER; break;
							case VK_ESCAPE: eventType = core::inputEvent::ESC; break;
							case VK_HOME: eventType = core::inputEvent::HOME; break;
							case VK_END: eventType = core::inputEvent::END; break;
							case VK_PRIOR: eventType = core::inputEvent::PAGE_UP; break;
							case VK_NEXT: eventType = core::inputEvent::PAGE_DOWN; break;
							case VK_INSERT: eventType = core::inputEvent::INSERT; break;
							case VK_DELETE: eventType = core::inputEvent::DELETE_KEY; break;
							case VK_LEFT: eventType = core::inputEvent::ARROW_LEFT; break;
							case VK_RIGHT: eventType = core::inputEvent::ARROW_RIGHT; break;
							case VK_UP: eventType = core::inputEvent::ARROW_UP; break;
							case VK_DOWN: eventType = core::inputEvent::ARROW_DOWN; break;
							case VK_F1: eventType = core::inputEvent::F1; break;
							case VK_F2: eventType = core::inputEvent::F2; break;
							case VK_F3: eventType = core::inputEvent::F3; break;
							case VK_F4: eventType = core::inputEvent::F4; break;
							case VK_F5: eventType = core::inputEvent::F5; break;
							case VK_F6: eventType = core::inputEvent::F6; break;
							case VK_F7: eventType = core::inputEvent::F7; break;
							case VK_F8: eventType = core::inputEvent::F8; break;
							case VK_F9: eventType = core::inputEvent::F9; break;
							case VK_F10: eventType = core::inputEvent::F10; break;
							case VK_F11: eventType = core::inputEvent::F11; break;
							case VK_F12: eventType = core::inputEvent::F12; break;
							case VK_BACK: eventType = core::inputEvent::BACKSPACE; break;
							default: eventType = core::inputEvent::KEY_PRESSED; break;
						}

						core::pushInputEvent(eventType, asciiChar, shiftPressed, ctrlPressed, altPressed);
					}
				}
			}
		}
	}

	void runInputHandler()
	{
		if (!running)
		{
			running = true;
			inputHandlerThread = std::jthread([this]() { inputHandler(); });
		}
	}

	void stopInputHandler()
	{
		if (running)
		{
			running = false;
			inputHandlerThread.join();
		}
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

bool console::show()
{
	if (!pimpl_->isVisible)
	{
		ShowWindow(pimpl_->hWnd, SW_SHOW);
		SetForegroundWindow(pimpl_->hWnd);
		SetFocus(pimpl_->hWnd);
		pimpl_->runInputHandler();
		pimpl_->isVisible = true;
		return true;
	}
	return false;
}

bool console::hide()
{
	if (pimpl_->isVisible)
	{
		ShowWindow(pimpl_->hWnd, SW_HIDE);
		pimpl_->stopInputHandler();
		pimpl_->isVisible = false;
		return true;
	}
	return false;
}

bool console::visible()
{
	return pimpl_->isVisible;
}

void console::setConsoleName(const char* name)
{
	SetConsoleTitle(TEXT(name));
}

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
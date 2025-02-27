#include "cli/cliCore.h"

#include <algorithm>
#include <errhandlingapi.h>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "utils/logger.h"

using namespace tk;

static bool isInit = false;
static std::vector<CHAR_INFO> screen;
static short consoleWidth = 0;
static short consoleHeight = 0;
static HANDLE hConsole = nullptr;

static double loggerHeightCoeff = 0.05;
static short loggerHeight = 0;
static short logStartIndex = 0;
static short logMaxLength = 0;

static short userWidth;
static short userHeight;

static const WORD HIGHLIGHT_COLOR = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
static const WORD DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

void cliCore::init()
{
	if (isInit)
	{
		return;
	}

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	hideCursor();
	initConsoleSize();
	initScreen();

	isInit = true;
}

void cliCore::update()
{
	if (!isInit || screen.empty())
	{
		return;
	}

	COORD bufferSize = { consoleWidth, consoleHeight };
	COORD bufferCoord = { 0, 0 };
	SHORT right = consoleWidth - 1;
	SHORT bottom = consoleHeight - 1;
	SMALL_RECT writeRegion = { 0, 0, right, bottom };

	if (!WriteConsoleOutput(hConsole, screen.data(), bufferSize, bufferCoord, &writeRegion))
	{
		throw std::runtime_error("Error WriteConsoleOutput: " + std::to_string(GetLastError()));
	}
}

void cliCore::log(const char* str)
{
	log(std::string(str));
}

void cliCore::log(const std::string& str)
{
	if (!isInit || screen.empty())
	{
		return;
	}

	size_t length = std::min(str.length(), static_cast<size_t>(logMaxLength));
	std::transform(str.begin(), str.begin() + length, screen.begin() + logStartIndex,
		[](const char& ch)
		{
			CHAR_INFO charInfo;
			charInfo.Char.AsciiChar = ch;
			charInfo.Attributes = DEFAULT_COLOR;
			return charInfo;
		});

	std::generate(screen.begin() + logStartIndex + length, screen.begin() + logStartIndex + logMaxLength,
		[]()
		{
			CHAR_INFO charInfo;
			charInfo.Char.AsciiChar = ' ';
			charInfo.Attributes = DEFAULT_COLOR;
			return charInfo;
		});
}

void cliCore::insertSymb(size_t x, size_t y, char symb)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Inserting symbol failed. x or y out of range");
		return;
	}

	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * consoleWidth + realX;
	screen[pos].Char.AsciiChar = symb;
	screen[pos].Attributes = DEFAULT_COLOR;
}

void cliCore::insertStr(size_t x, size_t y, const std::string& str)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Inserting string failed. x or y out of range");
		return;
	}

	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * consoleWidth + realX;

	for (const auto& symb : str)
	{
		if (!isBorder(pos))
		{
			screen[pos].Char.AsciiChar = symb;
			screen[pos].Attributes = DEFAULT_COLOR;
		}
		else
		{
			pos += 2;
			if (isBorder(pos))
			{
				LOG_ERR("Inserting string failed. Out of range.");
				break;
			}
			screen[pos].Char.AsciiChar = symb;
			screen[pos].Attributes = DEFAULT_COLOR;
		}
		pos++;
	}
}

void cliCore::clearLine(size_t y)
{
	if (y >= userHeight)
	{
		LOG_ERR("Clearing line failed. y out of range");
		return;
	}

	size_t startPos = (y + 1) * consoleWidth + 1;

	for (size_t x = 0; x < userWidth; ++x)
	{
		screen[startPos + x].Char.AsciiChar = ' ';
		screen[startPos + x].Attributes = DEFAULT_COLOR;
	}
}

void cliCore::clearScreen()
{
	for (size_t y = 0; y < userHeight; ++y)
	{
		clearLine(y);
	}
}

void cliCore::highlightChar(size_t x, size_t y)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Highlighting character failed. x or y out of range");
		return;
	}

	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * consoleWidth + realX;
	screen[pos].Attributes = HIGHLIGHT_COLOR;
}

void cliCore::highlightLine(size_t y)
{
	if (y >= userHeight)
	{
		LOG_ERR("Highlighting line failed. y out of range");
		return;
	}

	size_t realY = y + 1;
	size_t startPos = realY * consoleWidth + 1;

	std::for_each(screen.begin() + startPos, screen.begin() + startPos + userWidth, [](CHAR_INFO& charInfo) { charInfo.Attributes = HIGHLIGHT_COLOR; });
}

void cliCore::highlightText(size_t x, size_t y, size_t length)
{
	if (x >= userWidth || y >= userHeight || (x + length) > userWidth)
	{
		LOG_ERR("Highlighting text failed. x, y, or length out of range");
		return;
	}

	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * consoleWidth + realX;

	std::for_each(screen.begin(), screen.begin() + length, [](CHAR_INFO& charInfo) { charInfo.Attributes = HIGHLIGHT_COLOR; });
}

void cliCore::resetHighlightChar(size_t x, size_t y)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Resetting highlight failed. x or y out of range");
		return;
	}

	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * consoleWidth + realX;
	screen[pos].Attributes = DEFAULT_COLOR;
}

void cliCore::resetHighlightLine(size_t y)
{
	if (y >= userHeight)
	{
		LOG_ERR("Resetting highlight failed. y out of range");
		return;
	}

	size_t realY = y + 1;
	size_t startPos = realY * consoleWidth + 1;

	std::for_each(screen.begin() + startPos, screen.begin() + startPos + userWidth, [](CHAR_INFO& charInfo) { charInfo.Attributes = DEFAULT_COLOR; });
}

void cliCore::resetHighlightText(size_t x, size_t y, size_t length)
{
	if (x >= userWidth || y >= userHeight || (x + length) > userWidth)
	{
		LOG_ERR("Resetting highlight failed. x, y, or length out of range");
		return;
	}

	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * consoleWidth + realX;

	std::for_each(screen.begin(), screen.begin() + length, [](CHAR_INFO& charInfo) { charInfo.Attributes = DEFAULT_COLOR; });
}

size_t cliCore::width()
{
	return userWidth;
}

size_t cliCore::height()
{
	return userHeight;
}

bool cliCore::isEmptySymb(size_t x, size_t y)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Checking symbol failed. x or y out of range");
		return false;
	}

	size_t realX = x + 1;
	size_t realY = y + 1;

	size_t pos = realY * consoleWidth + realX;

	return (screen[pos].Char.AsciiChar == ' ');
}

bool cliCore::isEmptyLine(size_t y)
{
	if (y >= userHeight)
	{
		LOG_ERR("Checking line failed. y out of range");
		return false;
	}

	size_t startPos = (y + 1) * consoleWidth + 1;

	for (size_t x = 0; x < userWidth; ++x)
	{
		if (screen[startPos + x].Char.AsciiChar != ' ')
		{
			return false;
		}
	}

	return true;
}

void cliCore::hideCursor()
{
	if (hConsole == nullptr)
	{
		std::cerr << "Error hConsole is nullptr" << std::endl;
		return;
	}
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(hConsole, &cursorInfo))
	{
		throw std::runtime_error("Error GetConsoleCursorInfo: " + std::to_string(GetLastError()));
	}
	cursorInfo.bVisible = false;
	if (!SetConsoleCursorInfo(hConsole, &cursorInfo))
	{
		throw std::runtime_error("Error SetConsoleCursorInfo: " + std::to_string(GetLastError()));
		return;
	}
}

void cliCore::initConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		throw std::runtime_error("Error GetConsoleScreenBufferInfo: " + std::to_string(GetLastError()));
	}

	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	while (loggerHeight < 1)
	{
		loggerHeight = static_cast<size_t>(consoleHeight * loggerHeightCoeff);
		loggerHeightCoeff += 0.1;
	}

	logStartIndex = consoleWidth * (consoleHeight - loggerHeight);
	logMaxLength = consoleWidth * consoleHeight - logStartIndex;

	userWidth = consoleWidth - 2;
	userHeight = consoleHeight - loggerHeight - 2;
}

void cliCore::initScreen()
{
	screen.resize(consoleWidth * consoleHeight);

	for (auto& cell : screen)
	{
		cell.Char.AsciiChar = ' ';
		cell.Attributes = DEFAULT_COLOR;
	}
	
	std::string borderColumn = "+" + std::string(userHeight, '|') + "+";
	fillColummn(consoleWidth - 1, borderColumn);
	fillColummn(0, borderColumn);

	std::string borderLine = "+" + std::string(userWidth, '-') + "+";
	fillLine(0, borderLine);
	fillLine(userHeight + 1, borderLine);
}

void cliCore::fillLine(size_t index, const std::string& str)
{
	if (index >= consoleHeight || str.length() > consoleWidth)
	{
		return;
	}


	std::transform(str.begin(), str.end(), screen.begin() + index * consoleWidth,
		[](char c)
		{
			CHAR_INFO charInfo;
			charInfo.Char.AsciiChar = c;
			charInfo.Attributes = DEFAULT_COLOR;
			return charInfo;
		});
}

void cliCore::fillColummn(size_t index, const std::string& str)
{
	if (index >= consoleWidth || str.length() > consoleHeight)
	{
		return;
	}


	for (size_t i = 0; i < str.length(); ++i)
	{
		screen[i * consoleWidth + index].Char.AsciiChar = str[i];
		screen[i * consoleWidth + index].Attributes = DEFAULT_COLOR;
	}
}

bool cliCore::isBorder(size_t x, size_t y)
{
	return (x == 0 || x == userWidth + 1 || y == 0 || y == userHeight + 1);
}

bool cliCore::isBorder(size_t pos)
{
	size_t x = pos % consoleWidth;
	size_t y = pos / consoleWidth;

	return isBorder(x, y);
}
#include "cli/cliCore.h"

#include <algorithm>
#include <errhandlingapi.h>
#include <iostream>
#include <stdexcept>

#include "utils/logger.h"

using namespace tk;

bool cliCore::isInit_ = false;
std::string cliCore::screen_ = "";
size_t cliCore::width_ = 0;
size_t cliCore::height_ = 0;
size_t cliCore::userHeight_ = 0;
HANDLE cliCore::hConsole_ = nullptr;

static double loggerHeightCoeff = 0.1;
static size_t loggerHeight = 0;
static size_t logStartIndex = 0;
static size_t logMaxLength = 0;

static size_t userWidth;
static size_t userHeight;

void cliCore::init()
{
	if (isInit_)
	{
		return;
	}

	hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
	hideCursor();
	initConsoleSize();
	initScreen();

	isInit_ = true;
}

void cliCore::update()
{
	if (!isInit_ || screen_.empty())
	{
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hConsole_, &csbi))
	{
		throw std::runtime_error("Error GetConsoleScreenBufferInfo: " + std::to_string(GetLastError()));
	}

	if (!SetConsoleCursorPosition(hConsole_, { 0, 0 }))
	{
		throw std::runtime_error("Error SetConsoleCursorPosition: " + std::to_string(GetLastError()));
	}

	DWORD charsWritten;
	if (!WriteConsoleA(hConsole_, screen_.c_str(), screen_.length(), &charsWritten, NULL))
	{
		throw std::runtime_error("Error WriteConsoleA: " + std::to_string(GetLastError()));
	}
}

void cliCore::log(const char* str)
{
	log(std::string(str));
}

void cliCore::log(const std::string& str)
{
	if (!isInit_ || screen_.empty())
	{
		return;
	}
	if (str.length() > logMaxLength)
	{
		std::copy(str.begin(), str.begin() + logMaxLength, screen_.begin() + logStartIndex);
	}
	else
	{
		std::string line = str + std::string(logMaxLength - str.length(), ' ');
		std::copy(line.begin(), line.end(), screen_.begin() + logStartIndex);
	}
}

void cliCore::insertSymb(size_t x, size_t y, char symb)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Inserting symbol failed. x or y out of range");
	}
	size_t realY = y + 1;
	size_t realX = x + 1;
	screen_[realY * width_ + realX] = symb;
}

void cliCore::insertStr(size_t x, size_t y, const std::string& str)
{
	if (x >= userWidth || y >= userHeight)
	{
		LOG_ERR("Inserting string failed. x or y out of range");
	}
	size_t realY = y + 1;
	size_t realX = x + 1;

	size_t pos = realY * (userWidth + 2) + realX;

	for (const auto& symb : str)
	{
		if (!isBorder(pos))
		{
			screen_[pos] = symb;
		}
		else
		{
			pos += 2;
			if (isBorder(pos))
			{
				LOG_ERR("Inserting string failed. Out of range.");
				break;
			}
			screen_[pos] = symb;
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

	size_t startPos = (y + 1) * width_ + 1;

	for (size_t x = 0; x < userWidth; ++x)
	{
		screen_[startPos + x] = ' ';
	}
}

void cliCore::clearScreen()
{
	for (size_t y = 0; y < userHeight; ++y)
	{
		clearLine(y);
	}
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

	size_t pos = realY * width_ + realX;

	return (screen_[pos] == ' ');
}

bool cliCore::isEmptyLine(size_t y)
{
	if (y >= userHeight)
	{
		LOG_ERR("Checking line failed. y out of range");
		return false;
	}

	size_t startPos = (y + 1) * width_ + 1;

	for (size_t x = 0; x < userWidth; ++x)
	{
		if (screen_[startPos + x] != ' ')
		{
			return false;
		}
	}

	return true;
}

void cliCore::hideCursor()
{
	if (hConsole_ == nullptr)
	{
		std::cerr << "Error hConsole is nullptr" << std::endl;
		return;
	}
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(hConsole_, &cursorInfo))
	{
		throw std::runtime_error("Error GetConsoleCursorInfo: " + std::to_string(GetLastError()));
	}
	cursorInfo.bVisible = false;
	if (!SetConsoleCursorInfo(hConsole_, &cursorInfo))
	{
		throw std::runtime_error("Error SetConsoleCursorInfo: " + std::to_string(GetLastError()));
		return;
	}
}

void cliCore::initConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hConsole_, &csbi))
	{
		throw std::runtime_error("Error GetConsoleScreenBufferInfo: " + std::to_string(GetLastError()));
	}

	width_ = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	while (loggerHeight < 1)
	{
		loggerHeight = static_cast<size_t>(height_ * loggerHeightCoeff);
		loggerHeightCoeff += 0.1;
	}
	userHeight_ = height_ - loggerHeight;
	logStartIndex = width_ * (height_ - loggerHeight + 1);
	logMaxLength = width_ * height_ - logStartIndex;

	userWidth = width_ - 2;
	userHeight = height_ - loggerHeight - 2;
}

void cliCore::initScreen()
{
	screen_ = std::string(width_ * height_, ' ');

	std::string loggerLine = "--LOG--" + std::string(width_ - 7, ' ');
	fillLine(height_ - loggerHeight, loggerLine);

	std::string borderColumn = "+" + std::string(userHeight_ - 2, '|') + "+";
	fillColummn(width_ - 1, borderColumn);
	fillColummn(0, borderColumn);

	std::string borderLine = "+" + std::string(width_ - 2, '-') + "+";
	fillLine(0, borderLine);
	fillLine(userHeight_ - 1, borderLine);
}

void cliCore::fillLine(size_t index, const std::string& str)
{
	if (index >= height_ || str.length() > width_)
	{
		return;
	}

	std::copy(str.begin(), str.end(), screen_.begin() + index * width_);
}

void cliCore::fillColummn(size_t index, const std::string& str)
{
	if (index >= width_ || str.length() > height_)
	{
		return;
	}

	for (size_t i = 0; i < str.length(); i++)
	{
		screen_[i * width_ + index] = str[i];
	}
}

bool cliCore::isBorder(size_t x, size_t y)
{
	return (x == 0 || x == userWidth + 1 || y == 0 || y == userHeight + 1);
}

bool cliCore::isBorder(size_t pos)
{
	size_t x = pos % width_;
	size_t y = pos / width_;

	return isBorder(x, y);
}

#include "cli/cliCore.h"

#include <algorithm>
#include <errhandlingapi.h>
#include <iostream>
#include <stdexcept>

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

void cliCore::log(const char * str)
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
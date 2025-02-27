#pragma once

#include <string>
#include <windows.h>

namespace tk
{
class cliCore
{
public:
	static void init();
	static void update();

	static void log(const char* str);
	static void log(const std::string& str);

	// user interface

public:
	static void insertSymb(size_t x, size_t y, char symb);
	static void insertStr(size_t x, size_t y, const std::string& str);
	static void clearLine(size_t y);
	static void clearScreen();
	
	static size_t width();
	static size_t height();

	static bool isEmptySymb(size_t x, size_t y);
	static bool isEmptyLine(size_t y);
private:
	static void hideCursor();
	static void initConsoleSize();
	static void initScreen();

	static void fillLine(size_t index, const std::string& str);
	static void fillColummn(size_t index, const std::string& str);

	static bool isBorder(size_t x, size_t y);
	static bool isBorder(size_t pos);

private:
	static bool isInit_;
	static size_t width_, height_, userHeight_;
	static std::string screen_;

	static HANDLE hConsole_;
};
} // namespace tk
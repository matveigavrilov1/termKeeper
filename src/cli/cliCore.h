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

public:
	static void insertSymb(size_t x, size_t y, char symb);
	static void insertStr(size_t x, size_t y, const std::string& str);
	static void clearLine(size_t y);
	static void clearScreen();

	static void highlightChar(size_t x, size_t y);
	static void highlightLine(size_t y);
	static void highlightText(size_t x, size_t y, size_t length);
	static void resetHighlightChar(size_t x, size_t y);
	static void resetHighlightLine(size_t y);
	static void resetHighlightText(size_t x, size_t y, size_t length);

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
};
} // namespace tk
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

	static void log(const char * str);
	static void log(const std::string& str);

private:
	static void hideCursor();
	static void initConsoleSize();
	static void initScreen();

	static void fillLine(size_t index, const std::string& str);
	static void fillColummn(size_t index, const std::string& str);

private:
	static bool isInit_;
	static size_t width_, height_, userHeight_;
	static std::string screen_;
	
	static HANDLE hConsole_;
};
} // namespace tk
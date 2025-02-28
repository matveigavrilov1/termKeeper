#pragma once

#include <vector>
#include <windows.h>

namespace tk
{

class consolem
{
public:
	consolem();
	~consolem();

	bool init();

	bool write(const std::vector<CHAR_INFO>& buffer, size_t x, size_t y, size_t width, size_t height);

	bool setCursorVisibility(bool visible);

	bool updateSize();

	size_t height();
	size_t width();

private:
	HANDLE hConsole_ { nullptr };
	size_t height_ { 0 }, width_ { 0 };
};
} // namespace tk
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace os
{
class console
{
public:
	using shared_ptr_t = std::shared_ptr<console>;
	using unique_ptr_t = std::unique_ptr<console>;

	struct position
	{
		size_t x;
		size_t y;
	};

	struct size
	{
		size_t width;
		size_t height;
	};

	enum color
	{
		CONSOLE_COLOR_DEFAULT = 0,
		CONSOLE_COLOR_BLACK,
		CONSOLE_COLOR_RED,
		CONSOLE_COLOR_GREEN,
		CONSOLE_COLOR_YELLOW,
		CONSOLE_COLOR_BLUE,
		CONSOLE_COLOR_MAGENTA,
		CONSOLE_COLOR_CYAN,
		CONSOLE_COLOR_WHITE
	};

	struct charInfo
	{
		unsigned char ch;
		color bgColor = CONSOLE_COLOR_BLACK, txtColor = CONSOLE_COLOR_WHITE;
	};
	
	using charBuffer = std::vector<charInfo>;

	static shared_ptr_t get();

	console();
	~console();

	bool show();
	bool hide();
	bool visible();
	void setConsoleName(const char * name);

	void clear();

	void setCursorPosition(position pos);
	bool setCursorVisibility(bool visible);
	position getCursorPosition() const;

	bool setConsoleSize(size sz);
	size getConsoleSize() const;

	void putChar(position pos, char c);
	void putString(position pos, const char* str);
	
	void setTextColor(position pos, color color);
	void setBgColor(position pos, color color);

	bool write(const charBuffer& buffer, position pos, size sz);

	void fillArea(position pos1, position pos2, char fillChar);
	void clearArea(position pos1, position pos2);

	void resetColors();
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};

} // namespace os
#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <windows.h>

#include "cli/core/event.h"

namespace tk
{
class window
{
public:
	window(size_t x, size_t y, size_t width, size_t height);
	virtual ~window() = default;

	window(const window& other);
	window(window&& other) noexcept;
	window& operator= (const window& other);
	window& operator= (window&& other) noexcept;

	using unique_ptr_type = std::unique_ptr<window>;
	using shared_ptr_type = std::shared_ptr<window>;

	CHAR_INFO& operator[] (size_t x, size_t y);
	CHAR_INFO& operator[] (size_t index);

	virtual void setChar(size_t x, size_t y, char ch);
	virtual void setChar(size_t index, char ch);
	virtual void setAttribute(size_t x, size_t y, WORD attr);
	virtual void setAttribute(size_t index, WORD attr);

	// position on screen
	virtual size_t x() const;
	virtual size_t y() const;
	virtual size_t width() const;
	virtual size_t height() const;
	virtual size_t size() const;

	virtual void clear();

	virtual void handleInputEvent(event::shared_ptr_type event);

public:
	static const WORD HIGHLIGHT_COLOR = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	static const WORD DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

public:
	using buffer_type = std::vector<CHAR_INFO>;

	buffer_type& buffer();

private:
	size_t x_, y_, width_, height_;
	buffer_type buffer_;
};

} // namespace tk
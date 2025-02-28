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

	void setChar(size_t x, size_t y, char ch);
	void setChar(size_t index, char ch);
	void setAttribute(size_t x, size_t y, WORD attr);
	void setAttribute(size_t index, WORD attr);

	size_t x() const;
	size_t y() const;
	size_t width() const;
	size_t height() const;
	size_t size() const;

	void clear();

	virtual void handleInputEvent(event::shared_ptr_type event);

public:
	using buffer_type = std::vector<CHAR_INFO>;

	buffer_type& buffer();

	using iterator = buffer_type::iterator;
	using const_iterator = buffer_type::const_iterator;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;

private:
	size_t x_, y_, width_, height_;
	buffer_type buffer_;
};

} // namespace tk
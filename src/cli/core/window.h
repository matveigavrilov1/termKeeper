#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "cli/core/event.h"
#include "os/console.h"

namespace tk
{
class window
{
public:
	using charInfo = os::console::charInfo;
	using position_on_screen = os::console::position;
	using position_on_window = os::console::position;
	using window_size = os::console::size;
	using relative_size = std::pair<double, double>;

	explicit window(const std::string& name);
	window(position_on_screen pos, window_size sz, const std::string& name);
	window(position_on_screen pos, relative_size relativeSize, const std::string& name);
	virtual ~window() = default;

	window(const window& other);
	window(window&& other) noexcept;
	window& operator= (const window& other);
	window& operator= (window&& other) noexcept;

	using unique_ptr_t = std::unique_ptr<window>;
	using shared_ptr_t = std::shared_ptr<window>;

	charInfo& operator[] (position_on_window pos);
	charInfo& operator[] (size_t index);

	std::string name();

	virtual void setChar(position_on_window pos, charInfo ch);
	virtual void setChar(size_t index, charInfo ch);

	// position on screen
	virtual position_on_screen pos() const;
	virtual window_size size() const;
	virtual size_t x() const;
	virtual size_t y() const;
	virtual size_t width() const;
	virtual size_t height() const;

	virtual size_t length() const;
	virtual void clear();
	virtual void update();
	virtual void handleInputEvent(event::shared_ptr_type event);

	void setRelativeSize(relative_size relativeSize);
	void setAbsoluteSize(window_size absoluteSize);
	void setPosition(position_on_screen pos);
	void updateSize();

public:
	using buffer_type = os::console::charBuffer;

	buffer_type& buffer();

private:
	window_size calculateAbsoluteSize() const;

private:
	position_on_screen pos_;
	window_size size_;
	
	relative_size relativeSize_ = { 0.0, 0.0 };
	bool useRelativeSize_ = false;

	buffer_type buffer_;
	std::string name_;
};

} // namespace tk
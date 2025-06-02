#pragma once

#include "core/window.h"

namespace core
{
class form
{
public:
	using form_size = window::window_size;
	using relative_size = window::relative_size;

	form() = default;
	form(window::position_on_window pos, form_size size);
	form(window::position_on_window pos, relative_size relativeSize);

	virtual ~form() = default;

	virtual void show(window& wnd);
	
	virtual window::position_on_window pos() const;
	virtual form_size size() const;
	virtual size_t x() const;
	virtual size_t y() const;
	virtual size_t width() const;
	virtual size_t height() const;

	void setAbsoluteSize(form_size absoluteSize);
	void setPosition(window::position_on_window pos);
	void setRelativeSize(relative_size relativeSize);
	void updateSize(window& wnd);

private:
	form_size calculateAbsoluteSize(window& wnd) const;

protected:
	window::position_on_window pos_ { 0, 0 };
	form_size size_;

	relative_size relativeSize_ = { 0.0, 0.0 };
	bool useRelativeSize_ = false;
};
} // namespace core
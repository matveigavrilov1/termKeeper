#include "cli/core/form.h"

#include <cstddef>

namespace tk
{

form::form(window::position_on_window pos, form_size size)
: pos_ { pos }
, size_ { size }
, useRelativeSize_ { false }
{ }

form::form(window::position_on_window pos, std::pair<double, double> relativeSize)
: pos_ { pos }
, relativeSize_ { relativeSize }
, useRelativeSize_ { true }
{ }

void form::setRelativeSize(std::pair<double, double> relativeSize)
{
	relativeSize_ = relativeSize;
	useRelativeSize_ = true;
}

void form::updateSize(window& wnd)
{
	size_ = calculateAbsoluteSize(wnd);
}

void form::show(window& wnd)
{
	if (useRelativeSize_)
		updateSize(wnd);
}

form::form_size form::calculateAbsoluteSize(window& wnd) const
{
	return { static_cast<size_t>(wnd.size().width * relativeSize_.first), static_cast<size_t>(wnd.size().height * relativeSize_.second) };
}

window::position_on_screen form::pos() const
{
	return pos_;
}

form::form_size form::size() const
{
	return size_;
}

size_t form::x() const
{
	return pos_.x;
}

size_t form::y() const
{
	return pos_.y;
}

size_t form::width() const
{
	return size_.width;
}

size_t form::height() const
{
	return size_.height;
}

} // namespace tk
#include "cli/forms/form.h"

#include <algorithm>
#include <cstddef>

namespace tk
{
form::form(size_t x, size_t y, size_t width, size_t height)
: x_(x)
, y_(y)
, width_(width)
, height_(height)
{
	buffer_.resize(width_ * height_);
	clear();
}

void form::show(window& wnd)
{
	for (size_t i = 0; i < width_; ++i)
	{
		for (size_t j = 0; j < height_; ++j)
		{
			wnd.setChar(x_ + i, y_ + j, buffer_[j * width_ + i].Char.AsciiChar);
			wnd.setAttribute(x_ + i, y_ + j, buffer_[j * width_ + i].Attributes);
		}
	}
}

void form::clear()
{
	std::generate(buffer_.begin(), buffer_.end(),
		[]()
		{
			CHAR_INFO charInfo;
			charInfo.Char.AsciiChar = ' ';
			charInfo.Attributes = window::DEFAULT_COLOR;
			return charInfo;
		});
}

} // namespace tk
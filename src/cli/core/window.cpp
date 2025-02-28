#include "cli/core/window.h"

#include <utility>
#include <stdexcept>

namespace tk
{
window::window(size_t x, size_t y, size_t width, size_t height)
: x_(x)
, y_(y)
, width_(width)
, height_(height)
, buffer_(width * height)
{
	clear();
}

window::window(const window& other)
: x_(other.x_)
, y_(other.y_)
, width_(other.width_)
, height_(other.height_)
, buffer_(other.buffer_)
{ }

window::window(window&& other) noexcept
: x_(std::exchange(other.x_, 0))
, y_(std::exchange(other.y_, 0))
, width_(std::exchange(other.width_, 0))
, height_(std::exchange(other.height_, 0))
, buffer_(std::move(other.buffer_))
{ }

window& window::operator= (const window& other)
{
	if (this != &other)
	{
		x_ = other.x_;
		y_ = other.y_;
		width_ = other.width_;
		height_ = other.height_;
		buffer_ = other.buffer_;
	}
	return *this;
}

window& window::operator= (window&& other) noexcept
{
	if (this != &other)
	{
		x_ = std::exchange(other.x_, 0);
		y_ = std::exchange(other.y_, 0);
		width_ = std::exchange(other.width_, 0);
		height_ = std::exchange(other.height_, 0);
		buffer_ = std::move(other.buffer_);
	}
	return *this;
}

CHAR_INFO& window::operator[] (size_t x, size_t y)
{
	if (x >= width_ || y >= height_)
	{
		throw std::out_of_range("Coordinates (x, y) are out of range");
	}
	return buffer_.at(y * width_ + x);
}

CHAR_INFO& window::operator[] (size_t index)
{
	if (index >= buffer_.size())
	{
		throw std::out_of_range("Index is out of range");
	}
	return buffer_.at(index);
}

void window::setChar(size_t x, size_t y, char ch)
{
	if (x >= width_ || y >= height_)
	{
		throw std::out_of_range("Coordinates (x, y) are out of range");
	}
	buffer_.at(y * width_ + x).Char.AsciiChar = ch;
}

void window::setChar(size_t index, char ch)
{
	if (index >= buffer_.size())
	{
		throw std::out_of_range("Index is out of range");
	}
	buffer_.at(index).Char.AsciiChar = ch;
}

void window::setAttribute(size_t x, size_t y, WORD attr)
{
	if (x >= width_ || y >= height_)
	{
		throw std::out_of_range("Coordinates (x, y) are out of range");
	}
	buffer_.at(y * width_ + x).Attributes = attr;
}

void window::setAttribute(size_t index, WORD attr)
{
	if (index >= buffer_.size())
	{
		throw std::out_of_range("Index is out of range");
	}
	buffer_.at(index).Attributes = attr;
}

size_t window::x() const
{
	return x_;
}

size_t window::y() const
{
	return y_;
}

size_t window::width() const
{
	return width_;
}

size_t window::height() const
{
	return height_;
}

size_t window::size() const
{
	return width_ * height_;
}

void window::clear()
{
	std::fill(buffer_.begin(), buffer_.end(), CHAR_INFO { .Char = { .AsciiChar = ' ' } });
}

window::buffer_type& window::buffer()
{
	return buffer_;
}

void window::handleInputEvent(event::shared_ptr_type event)
{

}
}; // namespace tk
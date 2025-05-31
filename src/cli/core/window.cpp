#include "cli/core/window.h"

#include <utility>
#include <stdexcept>

#include "os/console.h"
#include "utils/logger.h"

namespace tk
{

window::window(const std::string& name)
: pos_ { 0, 0 }
, useRelativeSize_ { true }
, relativeSize_ { 1, 1 }
{ }

window::window(position_on_screen pos, window_size sz, const std::string& name)
: pos_(pos)
, size_(sz)
, buffer_(sz.width * sz.height)
, name_(name)
, useRelativeSize_(false)
{
	clear();
}

window::window(position_on_screen pos, relative_size relativeSize, const std::string& name)
: pos_(pos)
, name_(name)
, relativeSize_(relativeSize)
, useRelativeSize_(true)
{
	size_ = calculateAbsoluteSize();
	buffer_.resize(size_.width * size_.height);
}

window::window(const window& other)
: pos_(other.pos_)
, size_(other.size_)
, useRelativeSize_(other.useRelativeSize_)
, relativeSize_(other.relativeSize_)
, buffer_(other.buffer_)
, name_(other.name_)
{ }

window::window(window&& other) noexcept
: pos_(other.pos_)
, size_(other.size_)
, useRelativeSize_(other.useRelativeSize_)
, relativeSize_(other.relativeSize_)
, buffer_(std::move(other.buffer_))
, name_(std::move(other.name_))
{ }

window& window::operator= (const window& other)
{
	if (this != &other)
	{
		pos_ = other.pos_;
		size_ = other.size_;
		buffer_ = other.buffer_;
		name_ = other.name_;
	}
	return *this;
}

window& window::operator= (window&& other) noexcept
{
	if (this != &other)
	{
		pos_ = other.pos_;
		size_ = other.size_;
		buffer_ = std::move(other.buffer_);
		name_ = std::move(other.name_);
	}
	return *this;
}

window::charInfo& window::operator[] (position_on_window pos)
{
	if (pos.x >= width() || pos.y >= height())
	{
		throw std::out_of_range("Coordinates (x, y) are out of range");
	}
	return buffer_.at(pos.y * width() + pos.x);
}

window::charInfo& window::operator[] (size_t index)
{
	if (index >= buffer_.size())
	{
		throw std::out_of_range("Index is out of range");
	}
	return buffer_.at(index);
}

std::string window::name()
{
	return name_;
}

void window::setChar(position_on_window pos, charInfo ch)
{
	if (pos.x >= width() || pos.y >= height())
	{
		std::string error = "Coordinates (x, y) are out of range: " + std::to_string(pos.x) + ", " + std::to_string(pos.y)
			+ " for width: " + std::to_string(width()) + " and height: " + std::to_string(height());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}
	buffer_.at(pos.y * width() + pos.x) = ch;
}

void window::setChar(size_t index, charInfo ch)
{
	if (index >= buffer_.size())
	{
		std::string error =
			"Index is out of range: " + std::to_string(index) + " for width: " + std::to_string(width()) + " and height: " + std::to_string(height());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}
	buffer_.at(index) = ch;
}

window::position_on_screen window::pos() const
{
	return pos_;
}

window::window_size window::size() const
{
	return size_;
}

size_t window::x() const
{
	return pos_.x;
}

size_t window::y() const
{
	return pos_.y;
}

size_t window::width() const
{
	return size_.width;
}

size_t window::height() const
{
	return size_.height;
}

size_t window::length() const
{
	return size_.width * size_.height;
}

void window::clear()
{
	std::fill(
		buffer_.begin(), buffer_.end(), charInfo { .ch = ' ', .bgColor = os::console::color::CONSOLE_COLOR_BLACK, .txtColor = os::console::CONSOLE_COLOR_WHITE });
}

window::buffer_type& window::buffer()
{
	return buffer_;
}

void window::update()
{
	LOG_DBG("Default window update handler");
}

void window::handleInputEvent(event::shared_ptr_type event)
{
	LOG_DBG("Default window input handler");
}

void window::setRelativeSize(std::pair<double, double> relativeSize)
{
	relativeSize_ = relativeSize;
	useRelativeSize_ = true;
}

void window::setAbsoluteSize(window_size absoluteSize)
{
	size_ = absoluteSize;
	useRelativeSize_ = false;
}

void window::setPosition(position_on_screen pos)
{
	pos_ = pos;
}

void window::updateSize()
{
	if (useRelativeSize_)
	{
		auto newSize = calculateAbsoluteSize();
		if (newSize.height == size_.height || newSize.width != size_.width)
		{
			size_ = newSize;
			buffer_.resize(size_.width * size_.height);
		}
	}
}

window::window_size window::calculateAbsoluteSize() const
{
	return { static_cast<size_t>(os::console::size().width * relativeSize_.first), static_cast<size_t>(os::console::size().height * relativeSize_.second) };
}
}; // namespace tk
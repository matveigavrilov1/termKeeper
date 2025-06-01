#include "core/window.h"

#include <utility>
#include <stdexcept>
#include <uuid.h>

#include "os/console.h"
#include "utils/generate_uuid.h"
#include "utils/logger.h"

namespace tk
{

window::window(const std::string& name)
: pos_ { 0, 0 }
, useRelativeSize_ { true }
, relativeSize_ { 1, 1 }
, name_ { name }
, uuid_ { utils::generate_uuid() }
{
	size_ = calculateAbsoluteSize();
	buffer_.resize(size_.width * size_.height);
	clear();
}

window::window(position_on_screen pos, window_size sz, const std::string& name)
: pos_(pos)
, size_(sz)
, buffer_(sz.width * sz.height)
, useRelativeSize_(false)
, name_(name)
, uuid_ { utils::generate_uuid() }
{
	clear();
}

window::window(position_on_screen pos, relative_size relativeSize, const std::string& name)
: pos_(pos)
, relativeSize_(relativeSize)
, useRelativeSize_(true)
, name_(name)
, uuid_ { utils::generate_uuid() }
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
, uuid_ { utils::generate_uuid() }
{ }

window::window(window&& other) noexcept
: pos_(other.pos_)
, size_(other.size_)
, useRelativeSize_(other.useRelativeSize_)
, relativeSize_(other.relativeSize_)
, buffer_(std::move(other.buffer_))
, name_(std::move(other.name_))
, uuid_ { std::move(other.uuid_) }
{ }

window& window::operator= (const window& other)
{
	if (this != &other)
	{
		pos_ = other.pos_;
		size_ = other.size_;
		buffer_ = other.buffer_;
		name_ = other.name_;
		uuid_ = utils::generate_uuid();
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
		uuid_ = std::move(other.uuid_);
	}
	return *this;
}

window::charInfo& window::operator[] (position_on_window pos)
{
	if (pos.x >= realWidth() || pos.y >= realHeight())
	{
		throw std::out_of_range("Coordinates (x, y) are out of range");
	}
	return buffer_.at(pos.y * realWidth() + pos.x);
}

window::charInfo& window::operator[] (size_t index)
{
	if (index >= buffer_.size())
	{
		throw std::out_of_range("Index is out of range");
	}
	return buffer_.at(index);
}

const std::string& window::name() const
{
	return name_;
}

const uuids::uuid& window::uuid() const
{
	return uuid_;
}

void window::setChar(position_on_window pos, charInfo ch)
{
	if (pos.x >= realWidth() || pos.y >= realHeight())
	{
		std::string error = "Coordinates (x, y) are out of range: " + std::to_string(pos.x) + ", " + std::to_string(pos.y)
			+ " for width: " + std::to_string(realWidth()) + " and height: " + std::to_string(realHeight());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}
	buffer_[pos.y * realWidth() + pos.x] = ch;
}

void window::setChar(size_t index, charInfo ch)
{
	if (index >= buffer_.size())
	{
		std::string error =
			"Index is out of range: " + std::to_string(index) + " for width: " + std::to_string(realWidth()) + " and height: " + std::to_string(realHeight());
		LOG_ERR(error);
		throw std::out_of_range(error);
	}
	buffer_[index] = ch;
}

void window::setContentChar(position_on_window pos, charInfo ch)
{
	size_t diffX = contentX() - realX();
	size_t diffY = contentY() - realY();
	size_t realX = pos.x + diffX;
	size_t realY = pos.y + diffY;

	setChar({realX, realY}, ch);
}

void window::setContentChar(size_t index, charInfo ch)
{

	size_t x = index % contentWidth();
	size_t y = index / contentWidth();

	size_t diffX = contentX() - realX();
	size_t diffY = contentY() - realY();

	size_t realX = x + diffX;
	size_t realY = y + diffY;

	setChar({realX, realY}, ch);
}

window::position_on_screen window::realPos() const
{
	return pos_;
}

window::window_size window::realSize() const
{
	return size_;
}

size_t window::realX() const
{
	return pos_.x;
}

size_t window::realY() const
{
	return pos_.y;
}

size_t window::realWidth() const
{
	return size_.width;
}

size_t window::realHeight() const
{
	return size_.height;
}

window::position_on_screen window::contentPos() const
{
	return realPos();
}

window::window_size window::contentSize() const
{
	return realSize();
}

size_t window::contentX() const
{
	return contentPos().x;
}

size_t window::contentY() const
{
	return contentPos().y;
}

size_t window::contentWidth() const
{
	return contentSize().width;
}

size_t window::contentHeight() const
{
	return contentSize().height;
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
	updateSize();
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
	auto consoleSize = os::console::get()->getConsoleSize();
	return { static_cast<size_t>(consoleSize.width * relativeSize_.first), static_cast<size_t>(consoleSize.height * relativeSize_.second) };
}
}; // namespace tk
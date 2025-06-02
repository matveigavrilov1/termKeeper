#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include <uuid.h>

#include "core/event.h"
#include "os/console.h"

namespace core
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

	const std::string& name() const;
	const uuids::uuid& uuid() const;

	virtual void setChar(position_on_window pos, charInfo ch);
	virtual void setChar(size_t index, charInfo ch);
	virtual void setContentChar(position_on_window pos, charInfo ch);
	virtual void setContentChar(size_t index, charInfo ch);

	// position on screen
	position_on_screen realPos() const;
	window_size realSize() const;
	size_t realX() const;
	size_t realY() const;
	size_t realWidth() const;
	size_t realHeight() const;

	virtual position_on_screen contentPos() const;
	virtual window_size contentSize() const;
	size_t contentX() const;
	size_t contentY() const;
	size_t contentWidth() const;
	size_t contentHeight() const;

	virtual size_t length() const;
	virtual void clear();
	virtual void update();
	virtual void handleInputEvent(event::shared_ptr_t event);

	virtual void setRelativeSize(relative_size relativeSize);
	virtual void setAbsoluteSize(window_size absoluteSize);
	virtual void setPosition(position_on_screen pos);
	virtual void updateSize();

	void setLeftNeighbourUuid(uuids::uuid uuid);
	void setRightNeighbourUuid(uuids::uuid uuid);
	void setUpperNeighbourUuid(uuids::uuid uuid);
	void setLowerNeighbourUuid(uuids::uuid uuid);

	uuids::uuid getLeftNeighbourUuid() const;
	uuids::uuid getRightNeighbourUuid() const;
	uuids::uuid getUpperNeighbourUuid() const;
	uuids::uuid getLowerNeighbourUuid() const;

public:
	using buffer_type = os::console::charBuffer;

	buffer_type& buffer();

private:
	window_size calculateAbsoluteSize() const;

private:
	position_on_screen pos_;
	window_size size_;

	relative_size relativeSize_ = { 1, 1 };
	bool useRelativeSize_ = false;

	buffer_type buffer_;
	std::string name_;
	uuids::uuid uuid_;

	uuids::uuid leftNeighbour_;
	uuids::uuid rightNeighbour_;
	uuids::uuid upperNeighbour_;
	uuids::uuid lowerNeighbour_;
};

} // namespace core
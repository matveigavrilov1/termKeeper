#pragma once

#include <optional>

#include "core/event.h"
#include "core/window.h"

namespace tk
{
class exitEvent : public event
{
public:
	using shared_ptr_type = std::shared_ptr<exitEvent>;

	exitEvent()
	: event(EXIT_EVENT)
	{ }
};

class inputEvent : public event
{
public:
	using shared_ptr_type = std::shared_ptr<inputEvent>;

	enum type
	{
		ENTER = 0,
		BACKSPACE,
		ESC,
		ARROW_LEFT,
		ARROW_RIGHT,
		ARROW_UP,
		ARROW_DOWN,
		KEY_PRESSED,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		INSERT,
		DELETE_KEY,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		UNSPECIFIED
	};

	inputEvent(type inputType, std::optional<char> key = std::nullopt, bool shiftPressed = false, bool ctrlPressed = false, bool altPressed = false)
	: event(INPUT_EVENT)
	, inputType_(inputType)
	, key_(key)
	, shiftPressed_(shiftPressed)
	, ctrlPressed_(ctrlPressed)
	, altPressed_(altPressed)
	{ }

	unsigned inputType() { return inputType_; }

	std::optional<char> key() { return key_; }

	bool shiftPressed() { return shiftPressed_; }

	bool ctrlPressed() { return ctrlPressed_; }

	bool altPressed() { return altPressed_; }

private:
	unsigned inputType_;
	std::optional<char> key_;
	bool shiftPressed_;
	bool ctrlPressed_;
	bool altPressed_;
};

class windowEvent : public event
{
public:
	using shared_ptr_type = std::shared_ptr<windowEvent>;

	windowEvent(window::shared_ptr_t window)
	: event(WINDOW_EVENT)
	, window_(std::move(window))
	{ }

	window::shared_ptr_t window() { return window_; }

private:
	window::shared_ptr_t window_;
};

class screenEvent : public event
{
	using shared_ptr_type = std::shared_ptr<screenEvent>;

	enum type
	{
		SHOW_SCREEN = 0
	};

	screenEvent(unsigned type = SHOW_SCREEN)
	: event(SCREEN_EVENT)
	, type_(type)
	{ }

	unsigned screenEventType() { return type_; }

private:
	unsigned type_;
};

} // namespace tk
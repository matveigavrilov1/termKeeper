#pragma once

#include <optional>

#include "cli/core/event.h"
#include "cli/core/window.h"

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
		CTRL_C,
		SHIFT_ENTER,
		KEY_PRESSED,
		CTRL_A,
		CTRL_B,
		CTRL_D,
		CTRL_E,
		CTRL_F,
		CTRL_G,
		CTRL_H,
		CTRL_I, // Tab
		CTRL_J,
		CTRL_K,
		CTRL_L,
		CTRL_M,
		CTRL_N,
		CTRL_O,
		CTRL_P,
		CTRL_Q,
		CTRL_R,
		CTRL_S,
		CTRL_T,
		CTRL_U,
		CTRL_V,
		CTRL_W,
		CTRL_X,
		CTRL_Y,
		CTRL_Z,
		CTRL_BACKSLASH,
		CTRL_SQUARE_BRACKET_RIGHT,
		CTRL_CARET,
		CTRL_UNDERSCORE,
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

	inputEvent(type inputType, std::optional<char> key = std::nullopt)
	: event(INPUT_EVENT)
	, inputType_(inputType)
	, key_(key)
	{ }

	unsigned inputType() { return inputType_; }

	std::optional<char> key() { return key_; }

private:
	unsigned inputType_;
	std::optional<char> key_;
};

class windowEvent : public event
{
public:
	using shared_ptr_type = std::shared_ptr<windowEvent>;

	windowEvent(window::shared_ptr_type window)
	: event(WINDOW_EVENT)
	, window_(std::move(window))
	{ }

	window::shared_ptr_type window() { return window_; }

private:
	window::shared_ptr_type window_;
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
#pragma once

#include <optional>

#include "core/event.h"
#include "core/window.h"

namespace core
{
class exitEvent : public event
{
public:
	using shared_ptr_t = std::shared_ptr<exitEvent>;

	exitEvent()
	: event(EXIT_EVENT)
	{ }
};

class inputEvent : public event
{
public:
	using shared_ptr_t = std::shared_ptr<inputEvent>;

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

	struct keyModifiers
	{
		bool shift = false;
		bool ctrl = false;
		bool alt = false;

		bool any() const { return shift || ctrl || alt; }

		bool none() const { return !any(); }
	};

	inputEvent(type inputType, std::optional<char> key = std::nullopt, keyModifiers modifiers = keyModifiers(false, false, false))
	: event(INPUT_EVENT)
	, inputType_(inputType)
	, key_(key)
	, modifiers_ { modifiers }
	{ }

	unsigned inputType() { return inputType_; }

	std::optional<char> key() { return key_; }

	bool shiftPressed() { return modifiers_.shift; }

	bool ctrlPressed() { return modifiers_.ctrl; }

	bool altPressed() { return modifiers_.alt; }

	const keyModifiers& modifiers() { return modifiers_; }

private:
	unsigned inputType_;
	std::optional<char> key_;
	keyModifiers modifiers_;
};

class windowEvent : public event
{
public:
	using shared_ptr_t = std::shared_ptr<windowEvent>;

	enum type
	{
		SHOW_WINDOW = 0
	};

	windowEvent(window::shared_ptr_t window, unsigned type = SHOW_WINDOW)
	: event(WINDOW_EVENT)
	, window_(std::move(window))
	, type_(type)
	{ }

	window::shared_ptr_t window() { return window_; }

private:
	window::shared_ptr_t window_;
	unsigned type_;
};

class screenEvent : public event
{
public:
	using shared_ptr_t = std::shared_ptr<screenEvent>;

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

} // namespace core
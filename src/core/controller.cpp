#include "core/controller.h"
#include "core/events.h"

namespace core
{
bool controller::handleInputEvent(inputEvent::shared_ptr_t inputEvent)
{
	switch (inputEvent->inputType())
	{
		case inputEvent::type::KEY_PRESSED: return handleKeyPressed(inputEvent->key().value(), inputEvent->modifiers());
		case inputEvent::type::BACKSPACE: return handleBackspace(inputEvent->modifiers());
		case inputEvent::type::ENTER: return handleEnter(inputEvent->modifiers());
		case inputEvent::type::ARROW_UP: return handleArrowUp(inputEvent->modifiers());
		case inputEvent::type::ARROW_DOWN: return handleArrowDown(inputEvent->modifiers());
		case inputEvent::type::ARROW_LEFT: return handleArrowLeft(inputEvent->modifiers());
		case inputEvent::type::ARROW_RIGHT: return handleArrowRight(inputEvent->modifiers());
		case inputEvent::type::PAGE_UP: return handlePageUp(inputEvent->modifiers());
		case inputEvent::type::PAGE_DOWN: return handlePageDown(inputEvent->modifiers());
		case inputEvent::type::HOME: return handleHome(inputEvent->modifiers());
		case inputEvent::type::END: return handleEnd(inputEvent->modifiers());
		case inputEvent::type::INSERT: return handleInsert(inputEvent->modifiers());
		case inputEvent::type::DELETE_KEY: return handleDelete(inputEvent->modifiers());
		case inputEvent::type::F1: return handleF1(inputEvent->modifiers());
		case inputEvent::type::F2: return handleF2(inputEvent->modifiers());
		case inputEvent::type::F3: return handleF3(inputEvent->modifiers());
		case inputEvent::type::F4: return handleF4(inputEvent->modifiers());
		case inputEvent::type::F5: return handleF5(inputEvent->modifiers());
		case inputEvent::type::F6: return handleF6(inputEvent->modifiers());
		case inputEvent::type::F7: return handleF7(inputEvent->modifiers());
		case inputEvent::type::F8: return handleF8(inputEvent->modifiers());
		case inputEvent::type::F9: return handleF9(inputEvent->modifiers());
		case inputEvent::type::F10: return handleF10(inputEvent->modifiers());
		case inputEvent::type::F11: return handleF11(inputEvent->modifiers());
		case inputEvent::type::F12: return handleF12(inputEvent->modifiers());
		default: return handleUnspecified(inputEvent->modifiers());
	}
}

bool controller::handleKeyPressed(char ch, const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleEnter(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleBackspace(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleArrowUp(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleArrowDown(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleArrowLeft(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleArrowRight(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handlePageUp(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handlePageDown(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleHome(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleEnd(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleInsert(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleDelete(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF1(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF2(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF3(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF4(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF5(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF6(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF7(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF8(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF9(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF10(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF11(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleF12(const inputEvent::keyModifiers& mods)
{
	return false;
}

bool controller::handleUnspecified(const inputEvent::keyModifiers& mods)
{
	return false;
}
} // namespace core
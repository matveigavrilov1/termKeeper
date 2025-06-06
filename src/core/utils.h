#pragma once

#include "core/events.h"

namespace core
{
void pushInputEvent(inputEvent::type type, std::optional<char> ch = std::nullopt, bool shiftPressed = false, bool ctrlPressed = false, bool altPressed = false);
void pushExitEvent();
void showWindow(window::shared_ptr_t win);
} // namespace core
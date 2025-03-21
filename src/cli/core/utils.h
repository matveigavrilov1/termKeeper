#pragma once

#include "cli/core/events.h"

namespace tk
{
void pushInputEvent(inputEvent::type type, std::optional<char> ch = std::nullopt);
void pushExitEvent();
void showWindow(window::shared_ptr_type win);
} // namespace tkS
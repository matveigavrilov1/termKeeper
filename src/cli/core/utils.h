#pragma once

#include <string>

#include "cli/core/events.h"
#include "cli/core/window.h"
#include "utils/logger.h"

namespace tk
{
namespace utils
{
// window
void fillLine(window& wnd, size_t y, std::string str);
void fillColumn(window& wnd, size_t x, std::string str);
void insertString(window& wnd, size_t x, size_t y, const std::string& str, WORD attr = window::DEFAULT_COLOR);
void highlightLine(window& wnd, size_t y, WORD attr = window::HIGHLIGHT_COLOR);
void unhighlightLine(window& wnd, size_t y, WORD attr = window::DEFAULT_COLOR);
void clearLine(window& wnd, size_t y, WORD attr = window::DEFAULT_COLOR);

// events
void pushShowWindowEvent(const std::string& windowName);

template<typename T>
void pushInputEvent(T data)
{
	LOG_DBG("Push input event with data: " << data);
	cli::core::getEventManager().pushEvent(std::make_shared<inputEvent>(std::make_shared<inputEvent::inputData<T>>(data)));
}

template<typename T>
std::shared_ptr<inputEvent::inputData<T>> getInputEventData(event::shared_ptr_type event)
{
	auto inputEventPtr = std::dynamic_pointer_cast<inputEvent>(event);
	if (!inputEventPtr)
	{
		return nullptr;
	}
	auto data = std::dynamic_pointer_cast<inputEvent::inputData<char>>(inputEventPtr->data());
	return data;
};
} // namespace utils

} // namespace tk
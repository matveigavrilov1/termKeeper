#pragma once

#include <string>

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
template<typename EventT>
std::shared_ptr<typename EventT::event_data_type> extractEventData(event::shared_ptr_type event)
{
	if (!event || !event->data())
	{
		LOG_WRN("Event or event data is null");
		return nullptr;
	}

	auto data = std::dynamic_pointer_cast<typename EventT::event_data_type>(event->data());
	if (!data)
	{
		LOG_WRN("Failed to cast event data to the specified type");
		return nullptr;
	}

	return data;
}

} // namespace utils

} // namespace tk
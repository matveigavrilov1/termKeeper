#pragma once

#include <string>

#include "cli/core/window.h"
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
} // namespace utils

} // namespace tk
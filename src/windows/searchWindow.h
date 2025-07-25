#pragma once

#include <string>

#include "windows/utils/controllerWindow.h"

namespace wndws
{
class searchWindow final
: public controllerWindow
{
public:
	searchWindow(const std::string& name = "Search");
};
} // namespace wndws
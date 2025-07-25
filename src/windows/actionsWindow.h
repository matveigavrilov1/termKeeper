#pragma once

#include <string>

#include "windows/utils/controllerWindow.h"

namespace wndws
{
class actionsWindow final
: public controllerWindow
{
public:
	actionsWindow(const std::string& name = "Actions");
};
} // namespace wndws
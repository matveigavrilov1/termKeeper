#pragma once

#include <string>

#include "windows/utils/controllerWindow.h"


namespace wndws
{
class descriptionWindow final
: public controllerWindow
{
public:
	descriptionWindow(const std::string& name = "Description");
};
} // namespace wndws
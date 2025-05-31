#pragma once

#include <unordered_map>
#include <string>

#include "cli/core/window.h"
#include "os/console.h"

namespace tk
{
class screen
{
public:
	void show(os::console::shared_ptr_t console) const;
	bool showWindow(const std::string& name, os::console::shared_ptr_t console) const;

	bool registerWindow(window::shared_ptr_t win);
	bool unregisterWindow(const std::string& name);
	bool activateWindow(const std::string& name);
	bool deactivateWindow(const std::string& name);
	void deactivateAllWindows();

	window::shared_ptr_t controllerWindow();
	bool changeControllerWindow(const std::string& name);

	bool activated(const std::string& name) const;
	using windows_map_type = std::unordered_map<std::string, window::shared_ptr_t>;

private:
	windows_map_type windows_;
	std::vector<std::string> activatedWindows_;
	window::shared_ptr_t controllerWindow_ { nullptr };
};

} // namespace tk
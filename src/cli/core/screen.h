#pragma once

#include <unordered_map>
#include <string>

#include "cli/core/consolem.h"
#include "cli/core/window.h"

namespace tk
{
class screen
{
public:
	void show(consolem& console) const;
	bool showWindow(const std::string& name, consolem& console) const;

	bool registerWindow(const std::string& name, window::shared_ptr_type win);
	bool unregisterWindow(const std::string& name);
	bool activateWindow(const std::string& name);
	bool deactivateWindow(const std::string& name);
	void deactivateAllWindows();
	
	window::shared_ptr_type activeWindow();
	bool changeActiveWindow(const std::string& name);

	bool activated(const std::string& name) const;
	using windows_map_type = std::unordered_map<std::string, window::shared_ptr_type>;

private:
	windows_map_type windows_;
	std::vector<std::string> activatedWindows_;
	window::shared_ptr_type activeWindow_{nullptr};
};

} // namespace tk
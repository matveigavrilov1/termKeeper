#pragma once

#include <unordered_map>

#include "core/window.h"
#include "os/console.h"

namespace core
{
class screen
{
public:
	void show(os::console::shared_ptr_t console) const;
	bool showWindow(const uuids::uuid& name, os::console::shared_ptr_t console) const;

	bool registerWindow(window::shared_ptr_t win);
	bool unregisterWindow(const uuids::uuid& uuid);
	bool activateWindow(const uuids::uuid& uuid);
	bool deactivateWindow(const uuids::uuid& uuid);
	void deactivateAllWindows();

	window::shared_ptr_t controllerWindow();
	bool changeControllerWindow(const uuids::uuid& uuid);

	bool activated(const uuids::uuid& uuid) const;
	using windows_map_t = std::unordered_map<uuids::uuid, window::shared_ptr_t>;

	uuids::uuid findUpperNeighbour(const uuids::uuid& target) const;
	uuids::uuid findLowerNeighbour(const uuids::uuid& target) const;
	uuids::uuid findLeftNeighbour(const uuids::uuid& target) const;
	uuids::uuid findRightNeighbour(const uuids::uuid& target) const;

private:
	windows_map_t windows_;
	std::vector<uuids::uuid> activatedWindows_;
	window::shared_ptr_t controllerWindow_ { nullptr };
};

} // namespace core
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
	bool showWindow(const uuids::uuid& uuid, os::console::shared_ptr_t console) const;
	void updateWindow(const uuids::uuid& uuid);
	void updateAll();

	bool registerWindow(window::shared_ptr_t win);
	bool unregisterWindow(const uuids::uuid& uuid);
	bool activateWindow(const uuids::uuid& uuid);
	bool deactivateWindow(const uuids::uuid& uuid);
	void deactivateAllWindows();

	bool activated(const uuids::uuid& uuid) const;
	using windows_map_t = std::unordered_map<uuids::uuid, window::shared_ptr_t>;
	using windows_vec_t = std::vector<window::shared_ptr_t>;

	uuids::uuid findUpperNeighbour(const uuids::uuid& target) const;
	uuids::uuid findLowerNeighbour(const uuids::uuid& target) const;
	uuids::uuid findLeftNeighbour(const uuids::uuid& target) const;
	uuids::uuid findRightNeighbour(const uuids::uuid& target) const;

	window::position_on_screen getWindowPosition(const uuids::uuid& uuid) const;
	window::window_size getWindowSize(const uuids::uuid& uuid) const;

private:
	windows_map_t windows_;
	std::vector<uuids::uuid> activatedWindows_;
	std::vector<uuids::uuid> allWindows_;
};

} // namespace core
#include "core/screen.h"

#include <algorithm>

#include "core/utils.h"
#include "utils/logger.h"

namespace tk
{

static bool isColliding(const window& win1, const window& win2)
{
	bool x_overlap = (win1.realX() < win2.realX() + win2.realWidth()) && (win1.realX() + win1.realWidth() > win2.realX());
	bool y_overlap = (win1.realY() < win2.realY() + win2.realHeight()) && (win1.realY() + win1.realHeight() > win2.realY());

	LOG_DBG("Checking collision between '" << win1.name() << "' and '" << win2.name() << "' - " << (x_overlap && y_overlap ? "COLLISION" : "no collision"));
	return x_overlap && y_overlap;
}

void screen::show(os::console::shared_ptr_t console) const
{
	LOG_DBG("Showing all activated windows (count: " << activatedWindows_.size() << ")");
	for (const auto& uuid : activatedWindows_)
	{
		LOG_DBG("Showing window with UUID: " << uuid);
		showWindow(uuid, console);
	}
}

bool screen::showWindow(const uuids::uuid& uuid, os::console::shared_ptr_t console) const
{
	if (auto it = windows_.find(uuid); it != windows_.end())
	{
		if (activated(uuid))
		{
			const auto& activatedWindow = it->second;
			LOG_DBG("Rendering window '" << activatedWindow->name() << "' at (" << activatedWindow->realX() << "," << activatedWindow->realY() << ") " << "size "
																	 << activatedWindow->realWidth() << "x" << activatedWindow->realHeight());

			if (console)
				console->write(activatedWindow->buffer(), activatedWindow->realPos(), activatedWindow->realSize());
			return true;
		}
		else
		{
			LOG_DBG("Window '" << it->second->name() << "' is not activated");
		}
	}
	else
	{
		LOG_ERR("Window not found in registry: " << uuid);
	}
	return false;
}

bool screen::registerWindow(window::shared_ptr_t win)
{
	if (!win)
	{
		LOG_WRN("Attempt to register nullptr window");
		return false;
	}

	if (windows_.find(win->uuid()) == windows_.end())
	{
		LOG_DBG("Registering new window '" << win->name() << "' with UUID: " << win->uuid());
		windows_.emplace(win->uuid(), std::move(win));
		return true;
	}

	LOG_WRN("Window '" << win->name() << "' already registered with UUID: " << win->uuid());
	return false;
}

bool screen::unregisterWindow(const uuids::uuid& uuid)
{
	if (uuid.is_nil())
	{
		LOG_WRN("Attempt to unregister window with nil UUID");
		return false;
	}

	if (windows_.find(uuid) != windows_.end())
	{
		LOG_DBG("Unregistering window with UUID: " << uuid);
		windows_.erase(uuid);
		return true;
	}

	LOG_WRN("Window not found for unregistering: " << uuid);
	return false;
}

bool screen::activateWindow(const uuids::uuid& uuid)
{
	if (uuid.is_nil())
	{
		LOG_WRN("Attempt to activate window with nil UUID");
		return false;
	}

	auto it = windows_.find(uuid);
	if (it != windows_.end())
	{
		const auto& newWindow = it->second;
		LOG_DBG("Checking collisions for window '" << newWindow->name() << "'");

		for (const auto& activeWindowUuid : activatedWindows_)
		{
			const auto& activeWindow = windows_.find(activeWindowUuid)->second;
			if (isColliding(*newWindow, *activeWindow))
			{
				LOG_ERR("Cannot activate window '" << newWindow->name() << "' - collision with '" << activeWindow->name() << "'");
				return false;
			}
		}

		LOG_DBG("Activating window '" << newWindow->name() << "' with UUID: " << uuid);
		activatedWindows_.push_back(uuid);
		return true;
	}

	LOG_ERR("Cannot activate window - not registered: " << uuid);
	return false;
}

bool screen::deactivateWindow(const uuids::uuid& uuid)
{
	if (uuid.is_nil())
	{
		LOG_WRN("Attempt to deactivate window with nil UUID");
		return false;
	}

	if (auto it = std::find(activatedWindows_.begin(), activatedWindows_.end(), uuid); it != activatedWindows_.end())
	{
		LOG_DBG("Deactivating window with UUID: " << uuid);
		activatedWindows_.erase(it);
		return true;
	}

	LOG_WRN("Window not found in active list: " << uuid);
	return false;
}

void screen::deactivateAllWindows()
{
	LOG_DBG("Deactivating all windows (count: " << activatedWindows_.size() << ")");
	activatedWindows_.clear();
}

window::shared_ptr_t screen::controllerWindow()
{
	LOG_DBG("Getting controller window: " << (controllerWindow_ ? controllerWindow_->name() : "nullptr"));
	return controllerWindow_;
}

bool screen::changeControllerWindow(const uuids::uuid& uuid)
{
	if (uuid.is_nil())
	{
		LOG_WRN("Attempt to change controller to nil UUID");
		return false;
	}

	if (auto it = std::find(activatedWindows_.begin(), activatedWindows_.end(), uuid); it != activatedWindows_.end())
	{
		controllerWindow_ = windows_.find(uuid)->second;
		LOG_DBG("Changed controller window to '" << controllerWindow_->name() << "' with UUID: " << uuid);
		pushInputEvent(inputEvent::UNSPECIFIED);
		return true;
	}

	LOG_ERR("Cannot change controller - window not active: " << uuid);
	return false;
}

bool screen::activated(const uuids::uuid& uuid) const
{
	if (uuid.is_nil())
	{
		LOG_DBG("Checking activation for nil UUID - false");
		return false;
	}

	bool isActive = std::find(activatedWindows_.begin(), activatedWindows_.end(), uuid) != activatedWindows_.end();
	LOG_DBG("Window " << uuid << " is " << (isActive ? "active" : "not active"));
	return isActive;
}

uuids::uuid screen::findUpperNeighbour(const uuids::uuid& target) const
{
	LOG_DBG("Finding upper neighbour for window: " << target);

	if (!windows_.count(target) || activatedWindows_.empty())
	{
		LOG_DBG("Target window not found or no activated windows");
		return uuids::uuid();
	}

	const auto& targetWin = windows_.at(target);
	size_t targetBottom = targetWin->realY() + targetWin->realHeight();
	size_t targetX = targetWin->realX();
	size_t targetWidth = targetWin->realWidth();

	uuids::uuid result;
	size_t minDistance = std::numeric_limits<size_t>::max();

	LOG_DBG("Target window '" << targetWin->name() << "' at (" << targetX << "," << targetWin->realY() << ") " << "size " << targetWidth << "x"
														<< targetWin->realHeight());

	for (const auto& uuid : activatedWindows_)
	{
		if (uuid == target || !windows_.count(uuid))
			continue;

		const auto& win = windows_.at(uuid);
		size_t winBottom = win->realY() + win->realHeight();
		size_t winRight = win->realX() + win->realWidth();

		LOG_DBG(
			"Checking window '" << win->name() << "' at (" << win->realX() << "," << win->realY() << ") " << "size " << win->realWidth() << "x" << win->realHeight());

		if (winBottom <= targetWin->realY() && win->realX() < targetX + targetWidth && winRight > targetX)
		{
			size_t distance = targetWin->realY() - winBottom;
			LOG_DBG("Potential upper neighbour '" << win->name() << "' distance: " << distance);

			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
				LOG_DBG("New closest upper neighbour: '" << win->name() << "'");
			}
		}
	}

	LOG_DBG("Selected upper neighbour: " << result << (result.is_nil() ? " (none)" : ""));
	return result;
}

uuids::uuid screen::findLowerNeighbour(const uuids::uuid& target) const
{
	LOG_DBG("Finding lower neighbour for window: " << target);

	if (!windows_.count(target) || activatedWindows_.empty())
	{
		LOG_DBG("Target window not found or no activated windows");
		return uuids::uuid();
	}

	const auto& targetWin = windows_.at(target);
	size_t targetBottom = targetWin->realY() + targetWin->realHeight();
	size_t targetX = targetWin->realX();
	size_t targetWidth = targetWin->realWidth();

	uuids::uuid result;
	size_t minDistance = std::numeric_limits<size_t>::max();

	for (const auto& uuid : activatedWindows_)
	{
		if (uuid == target || !windows_.count(uuid))
			continue;

		const auto& win = windows_.at(uuid);
		size_t winY = win->realY();
		size_t winRight = win->realX() + win->realWidth();

		if (winY >= targetBottom && win->realX() < targetX + targetWidth && winRight > targetX)
		{
			size_t distance = winY - targetBottom;
			LOG_DBG("Potential lower neighbour '" << win->name() << "' distance: " << distance);

			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
				LOG_DBG("New closest lower neighbour: '" << win->name() << "'");
			}
		}
	}

	LOG_DBG("Selected lower neighbour: " << result << (result.is_nil() ? " (none)" : ""));
	return result;
}

uuids::uuid screen::findLeftNeighbour(const uuids::uuid& target) const
{
	LOG_DBG("Finding left neighbour for window: " << target);

	if (!windows_.count(target) || activatedWindows_.empty())
	{
		LOG_DBG("Target window not found or no activated windows");
		return uuids::uuid();
	}

	const auto& targetWin = windows_.at(target);
	size_t targetX = targetWin->realX();
	size_t targetY = targetWin->realY();
	size_t targetHeight = targetWin->realHeight();

	uuids::uuid result;
	size_t minDistance = std::numeric_limits<size_t>::max();

	for (const auto& uuid : activatedWindows_)
	{
		if (uuid == target || !windows_.count(uuid))
			continue;

		const auto& win = windows_.at(uuid);
		size_t winRight = win->realX() + win->realWidth();
		size_t winBottom = win->realY() + win->realHeight();

		if (winRight <= targetX && win->realY() < targetY + targetHeight && winBottom > targetY)
		{
			size_t distance = targetX - winRight;
			LOG_DBG("Potential left neighbour '" << win->name() << "' distance: " << distance);

			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
				LOG_DBG("New closest left neighbour: '" << win->name() << "'");
			}
		}
	}

	LOG_DBG("Selected left neighbour: " << result << (result.is_nil() ? " (none)" : ""));
	return result;
}

uuids::uuid screen::findRightNeighbour(const uuids::uuid& target) const
{
	LOG_DBG("Finding right neighbour for window: " << target);

	if (!windows_.count(target) || activatedWindows_.empty())
	{
		LOG_DBG("Target window not found or no activated windows");
		return uuids::uuid();
	}

	const auto& targetWin = windows_.at(target);
	size_t targetRight = targetWin->realX() + targetWin->realWidth();
	size_t targetY = targetWin->realY();
	size_t targetHeight = targetWin->realHeight();

	uuids::uuid result;
	size_t minDistance = std::numeric_limits<size_t>::max();

	for (const auto& uuid : activatedWindows_)
	{
		if (uuid == target || !windows_.count(uuid))
			continue;

		const auto& win = windows_.at(uuid);
		size_t winX = win->realX();
		size_t winBottom = win->realY() + win->realHeight();

		if (winX >= targetRight && win->realY() < targetY + targetHeight && winBottom > targetY)
		{
			size_t distance = winX - targetRight;
			LOG_DBG("Potential right neighbour '" << win->name() << "' distance: " << distance);

			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
				LOG_DBG("New closest right neighbour: '" << win->name() << "'");
			}
		}
	}

	LOG_DBG("Selected right neighbour: " << result << (result.is_nil() ? " (none)" : ""));
	return result;
}
} // namespace tk
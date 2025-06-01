#include "cli/core/screen.h"

#include <algorithm>

#include "cli/core/utils.h"
#include "utils/logger.h"

namespace tk
{

static bool isColliding(const window& win1, const window& win2)
{
	bool x_overlap = (win1.realX() < win2.realX() + win2.realWidth()) && (win1.realX() + win1.realWidth() > win2.realX());
	bool y_overlap = (win1.realY() < win2.realY() + win2.realHeight()) && (win1.realY() + win1.realHeight() > win2.realY());
	return x_overlap && y_overlap;
}

void screen::show(os::console::shared_ptr_t console) const
{
	for (const auto& uuid : activatedWindows_)
	{
		LOG_DBG("Show window: " << uuid);
		showWindow(uuid, console);
	}
}

bool screen::showWindow(const uuids::uuid& uuid, os::console::shared_ptr_t console) const
{
	if (auto it = windows_.find(uuid); it != windows_.end())
	{
		if (activated(uuid))
		{
			LOG_DBG("Show window: " << it->second->name());
			const auto& activatedWindow = it->second;
			if (console)
				console->write(activatedWindow->buffer(), activatedWindow->realPos(), activatedWindow->realSize());
		}
	}
	else
	{
		LOG_ERR("Window not found in activated: " << uuid);
		return false;
	}

	return true;
}

bool screen::registerWindow(window::shared_ptr_t win)
{
	if (!win)
	{
		LOG_WRN("Tried to register nullptr");
		return false;
	}
	if (windows_.find(win->uuid()) == windows_.end())
	{
		LOG_DBG("Registering window: " << win->name());
		windows_.emplace(win->uuid(), std::move(win));
		return true;
	}
	LOG_WRN("Window already registered: " << win->name());
	return false;
}

bool screen::unregisterWindow(const uuids::uuid& uuid)
{
	if (!uuid.is_nil())
	{
		return false;
	}
	if (windows_.find(uuid) != windows_.end())
	{
		windows_.erase(uuid);
		return true;
	}
	return false;
}

bool screen::activateWindow(const uuids::uuid& uuid)
{
	if (!uuid.is_nil())
	{
		return false;
	}
	auto it = windows_.find(uuid);
	if (it != windows_.end())
	{
		const auto& newWindow = it->second;

		for (const auto& activeWindowUuid : activatedWindows_)
		{
			const auto& activeWindow = windows_.find(activeWindowUuid)->second;
			if (isColliding(*newWindow, *activeWindow))
			{
				LOG_ERR("Cannot activate window: " << uuid << " because it collides with window: " << activeWindowUuid);
				return false;
			}
		}

		LOG_DBG("Activating window: " << uuid);
		activatedWindows_.push_back(uuid);
		return true;
	}
	LOG_ERR("Cannot activate window: " << uuid << " because it is not registered");
	return false;
}

bool screen::deactivateWindow(const uuids::uuid& uuid)
{
	if (!uuid.is_nil())
	{
		return false;
	}
	if (auto it = std::find(activatedWindows_.begin(), activatedWindows_.end(), uuid); it != activatedWindows_.end())
	{
		activatedWindows_.erase(it);
		return true;
	}
	return false;
}

void screen::deactivateAllWindows()
{
	activatedWindows_.clear();
}

window::shared_ptr_t screen::controllerWindow()
{
	return controllerWindow_;
}

bool screen::changeControllerWindow(const uuids::uuid& uuid)
{
	if (!uuid.is_nil())
	{
		return false;
	}
	if (auto it = std::find(activatedWindows_.begin(), activatedWindows_.end(), uuid); it != activatedWindows_.end())
	{
		LOG_DBG("Setting active window: " << uuid);
		controllerWindow_ = windows_.find(uuid)->second;
		pushInputEvent(inputEvent::UNSPECIFIED);
		return true;
	}
	return false;
}

bool screen::activated(const uuids::uuid& uuid) const
{
	if (!uuid.is_nil())
	{
		return false;
	}
	return std::find(activatedWindows_.begin(), activatedWindows_.end(), uuid) != activatedWindows_.end();
}

uuids::uuid screen::findUpperNeighbour(const uuids::uuid& target) const
{
	if (!windows_.count(target) || activatedWindows_.empty())
		return uuids::uuid();

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
		size_t winBottom = win->realY() + win->realHeight();
		size_t winRight = win->realX() + win->realWidth();

		// Проверяем пересечение по горизонтали и что окно выше целевого
		if (winBottom <= targetWin->realY() && win->realX() < targetX + targetWidth && winRight > targetX)
		{
			size_t distance = targetWin->realY() - winBottom;
			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
			}
		}
	}

	return result;
}

uuids::uuid screen::findLowerNeighbour(const uuids::uuid& target) const
{
	if (!windows_.count(target) || activatedWindows_.empty())
		return uuids::uuid();

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

		// Проверяем пересечение по горизонтали и что окно ниже целевого
		if (winY >= targetBottom && win->realX() < targetX + targetWidth && winRight > targetX)
		{
			size_t distance = winY - targetBottom;
			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
			}
		}
	}

	return result;
}

uuids::uuid screen::findLeftNeighbour(const uuids::uuid& target) const
{
	if (!windows_.count(target) || activatedWindows_.empty())
		return uuids::uuid();

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

		// Проверяем пересечение по вертикали и что окно левее целевого
		if (winRight <= targetX && win->realY() < targetY + targetHeight && winBottom > targetY)
		{
			size_t distance = targetX - winRight;
			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
			}
		}
	}

	return result;
}

uuids::uuid screen::findRightNeighbour(const uuids::uuid& target) const
{
	if (!windows_.count(target) || activatedWindows_.empty())
		return uuids::uuid();

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

		// Проверяем пересечение по вертикали и что окно правее целевого
		if (winX >= targetRight && win->realY() < targetY + targetHeight && winBottom > targetY)
		{
			size_t distance = winX - targetRight;
			if (distance < minDistance)
			{
				minDistance = distance;
				result = uuid;
			}
		}
	}

	return result;
}
} // namespace tk
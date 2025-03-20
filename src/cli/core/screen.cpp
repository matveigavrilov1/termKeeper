#include "cli/core/screen.h"

#include <algorithm>

#include "utils/logger.h"

namespace tk
{

static bool isColliding(const window& win1, const window& win2)
{
	bool x_overlap = (win1.x() < win2.x() + win2.width()) && (win1.x() + win1.width() > win2.x());
	bool y_overlap = (win1.y() < win2.y() + win2.height()) && (win1.y() + win1.height() > win2.y());
	return x_overlap && y_overlap;
}

void screen::show(consolem& console) const
{
	for (const auto& name : activatedWindows_)
	{
		LOG_DBG("Show window: " << name);
		showWindow(name, console);
	}
}

bool screen::showWindow(const std::string& name, consolem& console) const
{
	if (auto it = windows_.find(name); it != windows_.end())
	{
		if (activated(name))
		{
			LOG_DBG("Show window: " << name);
			const auto& activatedWindow = it->second;
			console.write(activatedWindow->buffer(), activatedWindow->x(), activatedWindow->y(), activatedWindow->width(), activatedWindow->height());
		}
	}
	else
	{
		LOG_ERR("Window not found in activated: " << name);
		return false;
	}

	return true;
}

bool screen::registerWindow(window::shared_ptr_type win)
{
	if (!win)
	{
		LOG_WRN("Tried to register nullptr");
		return false;
	}
	if (windows_.find(win->name()) == windows_.end())
	{
		LOG_DBG("Registering window: " << win->name());
		windows_.emplace(win->name(), std::move(win));
		return true;
	}
	LOG_WRN("Window already registered: " << win->name());
	return false;
}

bool screen::unregisterWindow(const std::string& name)
{
	if (windows_.find(name) != windows_.end())
	{
		windows_.erase(name);
		return true;
	}
	return false;
}

bool screen::activateWindow(const std::string& name)
{
	auto it = windows_.find(name);
	if (it != windows_.end())
	{
		const auto& newWindow = it->second;

		for (const auto& activeWindowName : activatedWindows_)
		{
			const auto& activeWindow = windows_.find(activeWindowName)->second;
			if (isColliding(*newWindow, *activeWindow))
			{
				LOG_ERR("Cannot activate window: " << name << " because it collides with window: " << activeWindowName);
				return false;
			}
		}

		LOG_DBG("Activating window: " << name);
		activatedWindows_.push_back(name);
		return true;
	}
	LOG_ERR("Cannot activate window: " << name << " because it is not registered");
	return false;
}

bool screen::deactivateWindow(const std::string& name)
{
	if (auto it = std::find(activatedWindows_.begin(), activatedWindows_.end(), name); it != activatedWindows_.end())
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

window::shared_ptr_type screen::controllerWindow()
{
	return controllerWindow_;
}

bool screen::changeControllerWindow(const std::string& name)
{
	if (auto it = std::find(activatedWindows_.begin(), activatedWindows_.end(), name); it != activatedWindows_.end())
	{
		LOG_DBG("Setting active window: " << name);
		controllerWindow_ = windows_.find(name)->second;
		return true;
	}
	return false;
}

bool screen::activated(const std::string& name) const
{
	return std::find(activatedWindows_.begin(), activatedWindows_.end(), name) != activatedWindows_.end();
}
} // namespace tk
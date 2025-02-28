#include "cli/core/screen.h"

#include <algorithm>

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
	for (const auto& name : avtivatedWindows_)
	{
		showWindow(name, console);
	}
}

bool screen::showWindow(const std::string& name, consolem& console) const
{
	if (auto it = windows_.find(name); it != windows_.end())
	{
		const auto& activeWindow = it->second;
		console.write(activeWindow->buffer(), activeWindow->x(), activeWindow->y(), activeWindow->width(), activeWindow->height());
	}
	else
	{
		return false;
	}

	return true;
}

bool screen::registerWindow(const std::string& name, window::shared_ptr_type win)
{
	if (windows_.find(name) != windows_.end())
	{
		windows_.emplace(name, std::move(win));
		return true;
	}
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

		for (const auto& activeWindowName : avtivatedWindows_)
		{
			const auto& activeWindow = windows_.find(activeWindowName)->second;
			if (isColliding(*newWindow, *activeWindow))
			{
				return false;
			}
		}

		avtivatedWindows_.push_back(name);
		return true;
	}
	return false;
}

bool screen::deactivateWindow(const std::string& name)
{
	if (auto it = std::find(avtivatedWindows_.begin(), avtivatedWindows_.end(), name); it != avtivatedWindows_.end())
	{
		avtivatedWindows_.erase(it);
		return true;
	}
	return true;
}

void screen::deactivateAllWindows()
{
	avtivatedWindows_.clear();
}

window::shared_ptr_type screen::activeWindow()
{
	return activeWindow_;
}

bool screen::changeActiveWindow(const std::string& name)
{
	if (auto it = std::find(avtivatedWindows_.begin(), avtivatedWindows_.end(), name); it != avtivatedWindows_.end())
	{
		activeWindow_ = windows_.find(name)->second;
		return true;
	}
	return false;
}

bool screen::activated(const std::string& name) const
{
	return std::find(avtivatedWindows_.begin(), avtivatedWindows_.end(), name) != avtivatedWindows_.end();
}
} // namespace tk
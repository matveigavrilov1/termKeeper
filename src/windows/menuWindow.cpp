#include "windows/menuWindow.h"

#include "core/interface.h"

#include "utils/generate_uuid.h"
#include "utils/logger.h"
#include "windows/utils/controllerWindow.h"

namespace wndws
{
menuWindow::menuWindow(const std::string& name)
: controllerWindow(name)
, form_ { false }
{
	LOG_DBG("Creating menuWindow with name: " << name);
	clear();

	form_.setRelativeSize({ 1, 1 });
	LOG_DBG("Form relative size set to {1, 1}");
}

void menuWindow::updateBordered()
{
	LOG_DBG("Updating menuWindow");
	isThisController() ? form_.showSelected() : form_.unshowSelected();

	LOG_DBG("Displaying form content");
	form_.show(*this);
}

bool menuWindow::handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods)
{
	LOG_DBG("Handling ARROW_UP event");
	auto selected = form_.getSelected();
	LOG_DBG("Deactivating current group: " << selected.uuid);

	deactivateGroup(groups_[selected.uuid]);

	LOG_DBG("Switching to previous item");
	form_.switchUp();

	selected = form_.getSelected();

	LOG_DBG("Activating new selected group: " << selected.uuid);
	activateGroup(groups_[selected.uuid]);
	for (const auto& window : groups_[selected.uuid].windows_)
	{
		core::screen().showWindow(window, os::console::get());
	}
	return true;
}

bool menuWindow::handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods)
{
	LOG_DBG("Handling ARROW_UP event");
	auto selected = form_.getSelected();
	LOG_DBG("Deactivating current group: " << selected.uuid);

	deactivateGroup(groups_[selected.uuid]);

	LOG_DBG("Switching to next item");
	form_.switchDown();

	selected = form_.getSelected();

	LOG_DBG("Activating new selected group: " << selected.uuid);
	activateGroup(groups_[selected.uuid]);
	for (const auto& window : groups_[selected.uuid].windows_)
	{
		core::screen().showWindow(window, os::console::get());
	}
	return true;
}

uuids::uuid menuWindow::createGroup(const std::string& title)
{
	auto newGroup = windowsGroup {};
	newGroup.title_ = title;
	auto uuid = utils::generate_uuid();
	groups_[uuid] = newGroup;
	form_.addItem({ .content = newGroup.title_, .uuid = uuid });
	LOG_DBG("Windows group with name " << title << "was successfully created");
	return uuid;
}

void menuWindow::addWindow(uuids::uuid groupUuid, uuids::uuid winUuid)
{
	LOG_DBG("Adding window to menu: " << win->name() << " (UUID: " << win->uuid() << ")");
	auto it = groups_.find(groupUuid);
	if (it != groups_.end())
	{
		it->second.windows_.push_back(winUuid);
		LOG_INF("Window: " << winUuid << " was added to group: " << it->second.name << " with uuid: " << it->first);
	}
	else
	{
		LOG_WRN("Failed to add window " << win->uuid() << ".Group with uuid: " << groupUuid << " was not found");
	}
}

void menuWindow::activateGroup(const windowsGroup& group)
{
	for (const auto& window : group.windows_)
	{
		core::screen().activateWindow(window);
	}
}

void menuWindow::deactivateGroup(const windowsGroup& group)
{
	for (const auto& window : group.windows_)
	{
		core::screen().deactivateWindow(window);
	}
}
} // namespace wndws
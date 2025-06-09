#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "windows/utils/controllerWindow.h"
#include "forms/selectionListForm.h"

namespace wndws
{
class menuWindow final
: public controllerWindow
, public std::enable_shared_from_this<menuWindow>
{
public:
	struct windowsGroup
	{
		std::vector<uuids::uuid> windows_;
		std::string title_ { "unknown" };
	};

	menuWindow(const std::string& name = "Menu");

	void updateBordered() override;

	uuids::uuid createGroup(const std::string& title);

	void addWindow(uuids::uuid groupUuid, uuids::uuid winUuid);

protected:
	bool handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods) override;

private:
	void activateGroup(const windowsGroup& group);
	void deactivateGroup(const windowsGroup& group);

private:
	forms::selectionListForm form_;
	std::map<uuids::uuid, windowsGroup> groups_;
};
} // namespace wndws
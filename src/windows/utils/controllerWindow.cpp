#include "windows/utils/controllerWindow.h"

#include "core/interface.h"
#include "core/screen.h"
#include "os/console.h"

namespace wndws
{

bool controllerWindow::handleInputEvent(core::inputEvent::shared_ptr_t inputEvent)
{
	setHighlightTitle(true);

	controller::handleInputEvent(inputEvent);

	updateBordered();
	core::screen().showWindow(uuid(), os::console::get());
	return false;
}

bool controllerWindow::isThisController()
{
	return core::controllerm().active(uuid());
}

void controllerWindow::giveControl(uuids::uuid uuid)
{
	if (core::controllerm().setActiveController(uuid))
	{
		core::screen().updateWindow(uuid);
		core::screen().showWindow(uuid, os::console::get());
		setHighlightTitle(false);
		update();
	}
}

void controllerWindow::giveControlToLeft()
{
	giveControl(core::screen().findLeftNeighbour(uuid()));
}

void controllerWindow::giveControlToRight()
{
	giveControl(core::screen().findRightNeighbour(uuid()));
}

void controllerWindow::giveControlToUpper()
{
	giveControl(core::screen().findUpperNeighbour(uuid()));
}

void controllerWindow::giveControlToLower()
{
	giveControl(core::screen().findLowerNeighbour(uuid()));
}

bool controllerWindow::handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods)
{
	return false;
}

bool controllerWindow::handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods)
{
	return false;
}

bool controllerWindow::handleArrowLeftDecorator(const core::inputEvent::keyModifiers& mods)
{
	return false;
}

bool controllerWindow::handleArrowRightDecorator(const core::inputEvent::keyModifiers& mods)
{
	return false;
}

bool controllerWindow::handleArrowUp(const core::inputEvent::keyModifiers& mods)
{
	if (mods.alt || !handleArrowUpDecorator(mods))
	{
		giveControlToUpper();
	}
	return true;
}

bool controllerWindow::handleArrowDown(const core::inputEvent::keyModifiers& mods)
{
	if (mods.alt || !handleArrowDownDecorator(mods))
	{
		giveControlToLower();
	}
	return true;
}

bool controllerWindow::handleArrowLeft(const core::inputEvent::keyModifiers& mods)
{
	if (mods.alt || !handleArrowLeftDecorator(mods))
	{
		giveControlToLeft();
	}
	return true;
}

bool controllerWindow::handleArrowRight(const core::inputEvent::keyModifiers& mods)
{
	if (mods.alt || !handleArrowRightDecorator(mods))
	{
		giveControlToRight();
	}
	return true;
}

} // namespace wndws
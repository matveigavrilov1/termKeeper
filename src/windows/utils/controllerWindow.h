#pragma once

#include "core/controller.h"
#include "windows/utils/borderedWindow.h"

namespace wndws
{
class controllerWindow
: public core::controller
, public wndws::borderedWindow
{
public:
	using wndws::borderedWindow::borderedWindow;

	bool handleInputEvent(core::inputEvent::shared_ptr_t inputEvent) override;

protected:
	bool isThisController();

	void giveControl(uuids::uuid uuid);
	void giveControlToLeft();
	void giveControlToRight();
	void giveControlToUpper();
	void giveControlToLower();

	virtual bool handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods);
	virtual bool handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods);
	virtual bool handleArrowLeftDecorator(const core::inputEvent::keyModifiers& mods);
	virtual bool handleArrowRightDecorator(const core::inputEvent::keyModifiers& mods);

	bool handleArrowUp(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowDown(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowLeft(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowRight(const core::inputEvent::keyModifiers& mods) override;
};
} // namespace wndws
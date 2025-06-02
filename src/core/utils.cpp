#include "core/utils.h"

#include <memory>

#include "core/events.h"
#include "core/interface.h"
#include "os/console.h"

namespace core
{
void pushInputEvent(inputEvent::type type, std::optional<char> ch, bool shiftPressed, bool ctrlPressed, bool altPressed)
{
	core::getEventManager().pushEvent(std::make_shared<inputEvent>(type, ch, shiftPressed, ctrlPressed, altPressed),
		[](event::shared_ptr_t event) { core::core::getScreen().controllerWindow()->handleInputEvent(event); });
}

void pushExitEvent()
{
	core::getEventManager().pushEvent(std::make_shared<exitEvent>(), [](event::shared_ptr_t event) { core::core::getEventManager().stop(); });
}

void showWindow(window::shared_ptr_t win)
{
	core::getEventManager().pushEvent(std::make_shared<windowEvent>(win),
		[](event::shared_ptr_t event)
		{
			auto win = std::static_pointer_cast<windowEvent>(event)->window();
			core::getScreen().showWindow(win->uuid(), os::console::get());
		});
}
} // namespace core
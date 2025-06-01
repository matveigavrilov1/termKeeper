#include "cli/core/utils.h"

#include <memory>

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "os/console.h"

namespace tk
{
void pushInputEvent(inputEvent::type type, std::optional<char> ch, bool shiftPressed, bool ctrlPressed, bool altPressed)
{
	tk::cli::core::getEventManager().pushEvent(std::make_shared<inputEvent>(type, ch, shiftPressed, ctrlPressed, altPressed),
		[](event::shared_ptr_type event) { tk::cli::core::getScreen().controllerWindow()->handleInputEvent(event); });
}

void pushExitEvent()
{
	tk::cli::core::getEventManager().pushEvent(std::make_shared<exitEvent>(), [](event::shared_ptr_type event) { tk::cli::core::getEventManager().stop(); });
}

void showWindow(window::shared_ptr_t win)
{
	cli::core::getEventManager().pushEvent(std::make_shared<windowEvent>(win),
		[](event::shared_ptr_type event)
		{
			auto win = std::static_pointer_cast<windowEvent>(event)->window();
			tk::cli::core::getScreen().showWindow(win->uuid(), os::console::get());
		});
}
} // namespace tk
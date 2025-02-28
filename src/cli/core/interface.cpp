#include "cli/core/interface.h"
#include "cli/core/eventm.h"

#include <stdexcept>

namespace tk
{
namespace cli
{
void core::init()
{
	if (isInitialized_)
		return;

	consolem_.emplace();
	if (!consolem_->init())
	{
		throw std::runtime_error("Failed to initialize console");
	}
	eventm_.emplace();
	screen_.emplace();

	initDefaultEventHandlers();

	isInitialized_ = true;
}

consolem& core::getConsoleManager()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");


	return *consolem_;
}

eventm& core::getEventManager()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");


	return *eventm_;
}

screen& core::getScreen()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");


	return *screen_;
}

void core::initDefaultEventHandlers()
{
	eventm_->registerHandler(EXIT_EVENT, [](event::shared_ptr_type event) { event->defaultHandler(event); });

	eventm_->registerHandler(INPUT_EVENT, [](event::shared_ptr_type event) { event->defaultHandler(event); });

	eventm_->registerHandler(WINDOW_ACTIVATE_EVENT, [](event::shared_ptr_type event) { event->defaultHandler(event); });

	eventm_->registerHandler(CHANGE_ACTIVE_WINDOW_EVENT, [](event::shared_ptr_type event) { event->defaultHandler(event); });

	eventm_->registerHandler(SHOW_WINDOW_EVENT, [](event::shared_ptr_type event) { event->defaultHandler(event); });

	eventm_->registerHandler(SHOW_SCREEN_EVENT, [](event::shared_ptr_type event) { event->defaultHandler(event); });
}

} // namespace cli
} // namespace tk
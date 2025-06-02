#include "core/eventm.h"
#include "core/controllerm.h"
#include "core/interface.h"
#include "core/event.h"
#include "utils/logger.h"
#include <memory>

namespace core
{

int eventm::run()
{
	if (running_)
	{
		return 1;
	}
	running_ = true;
	LOG_INF("Event manager started");
	loop();
	return 0;
}

void eventm::stop()
{
	running_ = false;
}

void eventm::loop()
{
	while (running_)
	{
		auto eventPair = eventQueue_.wait_and_pop();
		LOG_DBG("Got event: " << eventPair.first->type());
		eventPair.second(eventPair.first);
	}
}

void eventm::pushEvent(event::shared_ptr_t event, eventm::event_handler_t handler)
{
	LOG_DBG("Push event: " << event->type());
	eventQueue_.push(std::make_pair(event, handler));
}

void eventm::pushEvent(event::shared_ptr_t event)
{
	LOG_DBG("Push event: " << event->type());
	eventQueue_.push(std::make_pair(event, [this](event::shared_ptr_t event) { defaultHandler(event); }));
}

void eventm::defaultHandler(event::shared_ptr_t event)
{
	switch (event->type())
	{
		case core::eventType::EXIT_EVENT: handleExitEvent(std::static_pointer_cast<exitEvent>(event)); break;
		case core::eventType::INPUT_EVENT: handleInputEvent(std::static_pointer_cast<core::inputEvent>(event)); break;
		case core::eventType::WINDOW_EVENT: handleWindowEvent(std::static_pointer_cast<windowEvent>(event)); break;
		case core::eventType::SCREEN_EVENT: handleScreenEvent(std::static_pointer_cast<screenEvent>(event)); break;
		default: LOG_WRN("Unknown event type: " << event->type()); break;
	}
}

void eventm::handleExitEvent(exitEvent::shared_ptr_t exitEvent)
{
	// TODO
}

void eventm::handleInputEvent(inputEvent::shared_ptr_t inputEvent)
{
	auto controller = core::controllerm().getActiveController();

	if (controller)
		controller->handleInputEvent(inputEvent);
}

void eventm::handleWindowEvent(windowEvent::shared_ptr_t windowEvent)
{
	// TODO
}

void eventm::handleScreenEvent(screenEvent::shared_ptr_t screenEvent)
{
	// TODO
}


} // namespace core
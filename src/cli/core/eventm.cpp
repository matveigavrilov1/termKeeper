#include "cli/core/eventm.h"
#include "utils/logger.h"
namespace tk
{

void eventm::run()
{
	if (running_)
	{
		return;
	}
	running_ = true;
	LOG_INF("Event manager started");
	loop();
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

void eventm::pushEvent(event::shared_ptr_type event, eventm::event_handler_type handler)
{
	LOG_DBG("Push event: " << event->type());
	eventQueue_.push(std::make_pair(event, handler));
}
} // namespace tk
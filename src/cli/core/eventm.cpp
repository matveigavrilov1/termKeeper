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
		event::shared_ptr_type event = eventQueue_.wait_and_pop();
		LOG_DBG("Got event: " << event->type());
		if (auto it = eventHandlers_.find(event->type()); it != eventHandlers_.end())
		{
			LOG_DBG("Found handler" << event->type());
			it->second(event);
		}
	}
}

void eventm::pushEvent(event::shared_ptr_type event)
{
	LOG_DBG("Push event: " << event->type());
	eventQueue_.push(event);
}

void eventm::registerHandler(eventType type, event_handler_type handler)
{
	if (running_)
	{
		return;
	}
	eventHandlers_[type] = handler;
}
} // namespace tk